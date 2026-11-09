# Programming external OSPI flash with Renesas Flash Programmer (RFP)

Target question: how RFP programs external OSPI flash, and what is needed to
program an Infineon **S26HL01GTFPBHB020** (1 Gb SEMPER HyperFlash, 3.0 V).

## How RFP does it

RFP does not natively know how to drive any external flash. It works through
the RA8's on-chip **Renesas Boot Firmware** plus a user-supplied **loader file**:

1. RFP connects via an **E2 / E2 Lite** emulator and talks to the boot firmware
   in the MCU's ROM.
2. It reads the loader's header (Information Definition Section) to learn the
   flash's base address, size, page size, sector map, timeouts, and the OSPI
   clock / VCC2 voltage to use.
3. It issues the boot firmware's "External Flash Memory Setting" command with
   those clock/voltage parameters.
4. It downloads the loader's driver code (Driver Function Data Section) into
   internal SRAM at a fixed address defined by the boot firmware.
5. It erases / programs / verifies by calling the driver's entry points
   (arguments in R0–R2, return value in R3):

| Offset | Entry point                                   | Used on RA8        |
|--------|-----------------------------------------------|--------------------|
| 0x000  | `R_Flash_Initialize(rfu)`                     | yes                |
| 0x040  | `R_Flash_EraseSector(addr)` → bytes erased    | yes                |
| 0x080  | `R_Flash_EraseChip()`                         | yes                |
| 0x0C0  | `R_Flash_ProgramData(addr, data, cnt)` → bytes written | yes       |
| 0x100 / 0x140 / 0x180 | Uninitialize / ReadData / CalcCRC     | reserved, unused   |

### Requirements

- RFP **V3.22 or later**
- E2 or E2 Lite emulator
- **RA8-series MCU only**
- Loader file in Intel HEX format
- The same loader also works in e² studio 2026-04+ / FSP 6.3.0+

### Existing loaders

- Built-in: **S28HL512T** on EK-RA8M1 / EK-RA8D1 (added in RFP V3.18).
- Renesas sample project: **MX25LW51245G** on EK-RA8P1.
- **Nothing exists for the S26HL01GT** — you build the loader yourself.

## What you need to do for the S26HL01GT

### 0. Feasibility checks

- **MCU family.** This repo targets an **RA6M5**. RFP's external-memory
  feature depends on the RA8 boot firmware, so **it won't work on an RA6M5
  board**. On RA6M5 the alternatives are a SEGGER J-Link custom flash loader
  (Open Flashloader) or an in-application programming routine.
- **Bus type.** The S26HL01GT is **HyperFlash** (HyperBus: differential CK/CK#,
  RWDS strobe), not Octal xSPI like the S28HL512T / MX25LW. HyperBus *Flash*
  support in the RA8 OSPI_B peripheral (FSP `r_ospi_b`) could not be confirmed —
  the only RA8 HyperBus examples found use HyperRAM. Verify HyperBus support in
  the RA8 hardware manual before committing.

### 1. Get and adapt the sample project

Download `r20an0822ref0100-ra-refl.zip` and import it into e² studio, then:

**`LoaderInfo.c`** — header block (strings: A-Z, a-z, 0-9, `-`, `_` only;
null-terminated):

- `name`, `devName` (the RA8 part number), `extFlashDevName = "S26HL01GT"`
- `devAdr` = memory-mapped base of the OSPI chip-select the flash is wired to
  (from the MCU memory map)
- `extFlashSize = 0x08000000` (1 Gb = 128 MiB)
- `pageSize` = the S26HL01GT program buffer size (512 bytes)
- `paramType = 1`; `parameter[0]` = OCTACLK frequency code, `parameter[1]` =
  VCC2 voltage code, per the target MCU's *Renesas Boot Firmware* app note.
  The S26H**L** is a **3.0 V** part (S26H**S** is 1.8 V).
  `parameter[2..7] = 0xFFFFFFFF`.
- `timeoutProg` / `timeoutErase` = datasheet maximum page-program and
  sector-erase times, plus margin
- `sectors[]` — must match the device's configured sector architecture:
  - Uniform 256 KB:
    `{ {0x40000, 512}, {0xFFFFFFFF, 0xFFFFFFFF} }`
  - Hybrid, 4 KB parameter sectors at the bottom (same pattern Renesas uses
    for the S28HL512T):
    `{ {0x1000, 32}, {0x20000, 1}, {0x40000, 511}, {0xFFFFFFFF, 0xFFFFFFFF} }`

**`LoaderFunc.c` + device header (replacing `MX25LW51245G.h`)** — rewrite the
driver:

- HyperFlash does not use SPI opcodes. It uses parallel-NOR-style unlock
  sequences over HyperBus: writes to word addresses 0x555 / 0x2AA, then 0xA0
  (program) or 0x80 / 0x30 (sector erase), with status polling via 0x70.
- `R_Flash_Initialize` needs a new OSPI configuration for HyperBus framing,
  latency, and RWDS.
- Don't duplicate setup the boot firmware already performed (clock / voltage
  from the setting command).

**`target.ld`** — keep the driver at the SRAM load address required by the
target RA8 part's boot firmware.

### 2. Build and install

1. Build and convert the output to Intel HEX.
2. Copy it to `<RFP install dir>\Resources\ExternalMemory\RA\`
   (may need admin rights).

### 3. Use it in RFP

1. Create a **new** project in V3.22+ (projects from older RFP versions won't
   run external-memory operations).
2. **External Memory Settings** tab → check **Enable External Memory
   Settings** → pick the loader under **Loader Settings** (shown by file name
   without extension).
3. Program a HEX / S-record file whose addresses fall in the `devAdr` window.
   From the command line: `rfp-cli ... -external-loader`.

The same loader also enables external-flash download in e² studio: in the debug
configuration set **Enable external flash download = Yes** and give the
loader's full path.

## Sources

- [RA Family External Flash Library app note, R20AN0822](https://www.renesas.com/en/document/apn/ra-familly-external-flash-library-tools?r=488806)
- [Renesas Flash Programmer V3.18.00 Tool News](https://renesas.com/en/document/tnr/upgrade-revision-renesas-flash-programmer-v31800)
- [Renesas Flash Programmer flyer](https://www.renesas.com/en/document/fly/flyer-renesas-flash-programmer?r=488871)
- [FSP OSPI_B documentation](https://renesas.github.io/fsp/group___o_s_p_i___b.html)
- [Infineon S26HL/S26HS SEMPER Flash datasheet](https://static.chipdip.ru/lib/816/DOC028816392.pdf)
- [RA8P1 HyperRAM usage guide (RT-Thread)](https://rt-thread-studio.github.io/sdk-bsp-ra8p1-titan-board/latest/driver/Titan_driver_hyperram/README.html)
