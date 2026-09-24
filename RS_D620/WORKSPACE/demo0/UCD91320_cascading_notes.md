# UCD91320 Cascading Feature — Research Notes

Sources:
- `documents/ucd91320.pdf` — UCD91320 datasheet (SLVSIB0, December 2025)
- `documents/UCD91320_commands.pdf` — UCD91xxx Sequencer and System Health Controller PMBus Command Reference (SLVUCU5, December 2025)
- `documents/PMBus-Specification-Rev-1-3-1-Part-II-20150313.pdf` — PMBus Power System Management Protocol Specification Part II (Command Language), Rev 1.3.1

## 1. What cascading is (datasheet level)

Cascading links up to **4 UCD91320 devices** into one larger sequencer/system manager.
Each device handles 32 rails (24 analog-or-digital + 8 digital-only), so 4 devices give
**128 rails** total (datasheet §1, §6.1, §7.1).

Two headline benefits called out in the datasheet:
- **Single sequencing domain** — "offers convenient ways to manage up to 128 voltage rails" (§3, §6.1).
- **Synchronized fault response** — "a user defined fault pin coordinates the cascaded
  devices to take synchronized fault responses" (§3, §7.1).

Hardware hooks shown in the datasheet's application diagrams (Fig. 3-1 / 7-1):
- **`SYNC_OUT` pin** (pin 4 / B3) — "Synchronization clock I/O (5 kHz) for multiple chip
  cascading."
- A **GPIO handshake** labeled "Other sequencer done (cascade input)" between devices.
- Separate **PMBus addresses** per device via `PMBUS_ADDR0/1/2` (8 possible addresses from
  3 pins — Table 6-1).
- Configuration is done through the **TI Sequencer Studio** GUI, stored to on-chip NVM.

The datasheet does not give PMBus commands, register bits, or master/slave detail — that's
in the Command Reference.

## 2. How it's implemented (PMBus Command Reference, SLVUCU5 §7 "Cascading")

There are **three distinct cascading schemes**, which can be combined:

### 2.1 Power-On Cascading (§7.2)
1. Configure an LGPO on the controller device to output `POWER_GOOD`.
2. Wire that signal to the next device's `PMBUS_CNTRL` pin, **or** to one of its `MONx`/`GPIx`
   pins.
3. Optionally feed the *last* device's `POWER_GOOD` back to the *first* device's `MONx`/`GPIx`
   pin.

Behavior:
- **Startup:** targets start their own sequence only after the controller's rails are all in
  regulation.
- **Shutdown:** controller de-asserting `POWER_GOOD` (intentional or fault-driven) cascades
  shutdown to the targets.
- **Fault feedback:** if the *last* device in the chain faults, the first device treats the
  lost `POWER_GOOD` as a **UV fault** (if wired to `MONx`) or a **GPI fault** (if wired to
  `GPIx` — useful when `MONx` pins are scarce).

### 2.2 Power-On and Off Cascading (§7.3)
Adds a second LGPO per device configured to output `POWER_GOOD_OFF`, asserted once all of
that device's rails are below the `POWER_GOOD_OFF` threshold. The upstream device reads this
on a GPI as an off-sequencing dependency. Result: shutdown order is last-device-first,
controller-last.

### 2.3 Fault Cascading / Coordinated Fault Response (§7.4, §2.4.5)
- Up to **4 GPIs** per device can be configured as **Fault Pins**, each tied to a shared
  **Fault Bus** pulled up to 3.3 V through a 10 kΩ resistor.
- Idle: Fault Pins are inputs listening to the bus.
- On a fault, the detecting device's Fault Pin pulls the bus low; every other device on the
  bus sees this as a GPI fault and runs its own configured fault response. The pin reverts to
  an input once the fault clears.
- Faults that can assert a Fault Pin: `RESEQUENCE_ERROR`, `SEQ_ON_TIMEOUT`,
  `SEQ_OFF_TIMEOUT`, `OT_FAULT`, `IOUT_UC_FAULT`, `IOUT_OC_FAULT`, `VOUT_UV_FAULT`,
  `VOUT_OV_FAULT`, `TON_MAX_FAULT`.
- Timeout/`TON_MAX` faults latch the pin until the rail is commanded off and back on; other
  fault types latch until the underlying condition clears.
- GPI faults are **edge-triggered** — only the asserted→deasserted transition counts.

### 2.4 PMBus commands involved
| Command | Code | Role |
|---|---|---|
| `FAULT_PIN_CONFIG` | **D0h** (TI `MFR_SPECIFIC_00`) | Configures a GPI as a fault **output**: Page Mask (which rails' faults assert it), GPI Mask (which GPI faults assert it), Other Mask (system watchdog / re-sequence error). |
| `GPI_CONFIG` | **F9h** (TI `MFR_SPECIFIC_41`) | Configures a GPI as a fault **input** (Fault Enable Flags bit must be set for that GPI). Mode must be Unused or Input or the command is rejected (Invalid Data). |
| `GPI_FAULT_RESPONSES` | **F4h** (TI `MFR_SPECIFIC_36`) | Configures the response executed when a fault arrives via the bus; only active if the matching Fault Enable Flag is set. |
| `MISC_CONFIG` | **FCh** (TI `MFR_SPECIFIC_44`) | Bit 3 "Slave" — set on devices that take the external `SYNC_CLK` instead of driving it; valid only in a multi-chip setup. |

Sequencer Studio issues `FAULT_PIN_CONFIG` + `GPI_CONFIG` + `GPI_FAULT_RESPONSES` together
automatically when configuring fault pins through the GUI. For bidirectional faults, the pin
config in `FAULT_PIN_CONFIG` and `GPI_CONFIG` must match.

### 2.5 Cascading requirements checklist (§7.5)
- LGPOs: `POWER_GOOD` (always), `POWER_GOOD_OFF` (if off-sequencing needed).
- Sequencing on/off dependencies referencing the incoming power-good signals.
- UV fault response (power-good fed back via `MONx`) or GPI fault response (fed back via GPI).
- Fault Pin configuration (`FAULT_PIN_CONFIG`/`GPI_CONFIG`/`GPI_FAULT_RESPONSES`) for fault
  cascading.
- `SYNC_CLK` wired between devices for fault cascading.

### 2.6 Gotchas
- **Debug mode** on a GPI disables Fault Pin bus-pulldown, fault responses/logging, and
  sequence dependencies — not for production use (§2.4.4).
- **Firmware checksum failure** makes `SYNC_CLK` output repeated ~250 ms pulses — externally
  observable symptom of a bad chip in the cascade (§11).
- **Pin budget:** cascading consumes LGPOs/GPIs out of the shared 32-pin GPIO pool, so
  practical max rail count is somewhat under 128 once cascade signaling is accounted for.
- Datasheet says PMBus 1.2 in the feature list but 1.3 in §6.2.2 — command reference/PMBus
  spec behavior should be treated as authoritative over the datasheet's version claim.

## 3. PMBus spec: manufacturer-specific command code range

From PMBus Power System Management Protocol Specification, Part II, Rev 1.3.1:

- **§24 "Manufacturer Specific Commands":** reserves **46 command codes**, **C4h–FDh**,
  named `MFR_SPECIFIC_00` through `MFR_SPECIFIC_45`. Contents are fully manufacturer-defined;
  behavior must be documented in the device's own product literature.
- **§23 "User Data and Configuration":** separate range, **B0h–BFh**, `USER_DATA_00`–
  `USER_DATA_15` (16 commands), for customer-stored data (e.g. inventory, control-loop
  coefficients). Each supports block write/read up to 255 bytes (max 4,080 bytes total).
- **§25.1 `MFR_SPECIFIC_COMMAND_EXT`** — **FEh** — extends the manufacturer command set past
  the 256-code limit, via the Extended Command Read/Write Byte/Word protocol (Part I).
- **§25.2 `PMBUS_COMMAND_EXT`** — **FFh** — reserved for future PMBus-defined extension,
  same Extended Command mechanism.

### TI UCD91xxx numbering vs. the PMBus spec's hex names
TI's command reference numbers its manufacturer commands `MFR_SPECIFIC_00`–`_44` as an
offset from **D0h** (not from the spec's own C4h-based table). So:

- `MFR_SPECIFIC_00` (TI) = D0h = `FAULT_PIN_CONFIG`
- `MFR_SPECIFIC_36` (TI) = F4h = `GPI_FAULT_RESPONSES`
- `MFR_SPECIFIC_41` (TI) = F9h = `GPI_CONFIG`
- `MFR_SPECIFIC_44` (TI) = FCh = `MISC_CONFIG`

The UCD91xxx family only uses D0h–FCh of the available C4h–FDh manufacturer range.
