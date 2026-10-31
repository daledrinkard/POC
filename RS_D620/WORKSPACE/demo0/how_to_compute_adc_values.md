# How the UCD91xxx translates VOUT values to ADC counts

Source: *UCD91xxx Sequencer and System Health Controller PMBus Command Reference*
(SLVUCU5, `documents/UCD91320_commands.pdf`).

You never describe the resistor network to the chip directly. Instead you give it,
per rail, the divider **ratio**, and the firmware pre-converts every volts-type
setting into ADC counts when it is written. The ADC scan then compares raw counts.

## The settings involved (per rail = per PAGE)

1. **`MONITOR_CONFIG` (D5h)** — maps each MON pin to a rail (page) and a monitor
   type (e.g. type 1 = analog voltage monitor). This is how the chip knows which
   ADC channel belongs to which rail.

2. **`VOUT_SCALE_MONITOR` (2Ah)** — the divider ratio, LINEAR11, in V/V:

   ```
   VOUT_SCALE_MONITOR = V_pin / V_rail = R_bottom / (R_top + R_bottom)
   ```

   1.0 means no divider (pin tied directly to the rail). TI's sizing guidance is
   to place the nominal rail at ~1.875 V at the pin (75 % of a 2.5 V ADC window)
   so over-voltage remains measurable.

3. **`VOUT_MODE` (20h)** — the shared LINEAR16 exponent (bottom 5 bits) used by
   every volts-type value: VOUT_COMMAND, OV/UV fault & warn limits,
   POWER_GOOD_ON/OFF, margins, READ_VOUT. Choose it to fit the rail range
   (e.g. −13 → 0–4 V with 0.122 mV resolution). It must be written **before** any
   LINEAR16 value.

4. **ADC reference — `MISC_CONFIG` (FCh)** — bit 1 selects an external VREF+/VREF−
   reference (2.4–3.0 V, reset required); bytes 2–3 hold its voltage. The default
   is the internal V33A rail. This must be set **before** `MONITOR_CONFIG`, or the
   channels lock to the internal reference.

5. **`VOUT_CAL_MONITOR` (D1h)** — optional signed offset (in VOUT_MODE units)
   added to the measured voltage.

## The math

```
V_limit (volts)  = mantissa × 2^VOUT_MODE_exp            (LINEAR16)
V_pin            = V_limit × VOUT_SCALE_MONITOR
ADC_counts       = V_pin / V_ref × (2^N − 1)              (N = 12)
```

The firmware does this conversion once, when a limit is written, and stores the
count value (SLVUCU5 §18.4: internal variables are "scaled to take optimal
advantage of the native units such as ADC or DAC counts" — which is why
read-back values can differ very slightly from what was written).

READ_VOUT runs the inverse:

```
counts → V_pin → ÷ VOUT_SCALE_MONITOR → − VOUT_CAL_MONITOR → LINEAR16
```

### LINEAR formats

- **LINEAR16** (volts-type values): `V = mantissa × 2^X`, 16-bit unsigned
  mantissa, X = signed 5-bit exponent from VOUT_MODE.
- **LINEAR11** (everything else, incl. VOUT_SCALE_MONITOR): bits 15:11 = signed
  5-bit exponent, bits 10:0 = signed 11-bit mantissa, `R = Y × 2^X`.

## Worked example from `documents/pmbus_register_file.csv`

| Command            | Raw      | Decoded                                                         |
|--------------------|----------|-----------------------------------------------------------------|
| VOUT_MODE          | `0x13`   | mode 000 (linear), exp = 0b10011 = −13                          |
| VOUT_COMMAND       | `0x699A` | 27034 × 2⁻¹³ = **3.300 V**                                      |
| VOUT_SCALE_MONITOR | `0xBA00` | exp = 0b10111 = −9, mant = 0x200 = 512 → 512 × 2⁻⁹ = **1.0**    |

So this rail is a 3.3 V rail wired straight to its MON pin with no attenuation.

## Caveats

- The doc speaks of a "2.5 V ADC full-scale", yet the reference choices are V33A
  (~3.3 V) or an external 2.4–3.0 V. Treat 2.5 V as TI's conservative design
  window; size dividers against the reference actually used, keeping the nominal
  pin voltage at or below ~75 % of it.
- The summary table in the PDF appears to list VOUT_SCALE_MONITOR under `3E`,
  likely a text-extraction artifact; the rest of the doc, standard PMBus, and the
  CSV all use **0x2A**.

## Application to the simulator

A rail model needs only (VOUT_SCALE_MONITOR, VOUT_MODE exponent, V_ref) to convert
between `ControlPanel.adc_value[]` counts and the GUI-loaded volts values.
