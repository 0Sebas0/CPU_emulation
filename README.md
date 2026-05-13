# 🖥️ CPU_Emulation

A software emulator of a custom-designed 16-bit CPU architecture, written in C++. The emulator implements a complete fetch-decode-execute cycle, an 8-register file, a 20-instruction ISA, and a hardware flags register — all from scratch.

Built as a low-level systems programming exercise to explore how real CPUs work at the architectural level.

---

## Architecture Overview

| Component        | Details                                       |
|------------------|-----------------------------------------------|
| Word size        | 16-bit instructions                           |
| Registers        | 8 general-purpose (R0–R7), 8-bit each         |
| RAM              | 256 bytes                                     |
| Program Counter  | Byte pointer into RAM                         |
| Flags register   | 8-bit EFLAGS (carry, overflow, zero, EOF)     |
| I/O              | File-based (binary program + text input → text output) |

---

## Instruction Set (ISA)

### Data Transfer

| Opcode | Mnemonic | Description                                 |
|--------|----------|---------------------------------------------|
| `0x00` | `NOP`    | No operation                                |
| `0x01` | `IN`     | Read one byte from input file into register |
| `0x02` | `OUT`    | Write register value to output file         |
| `0x03` | `MOV`    | Copy register to register                   |
| `0x04` | `MOVC`   | Load 8-bit constant into register           |

### Control Flow

| Opcode | Mnemonic | Description                       |
|--------|----------|-----------------------------------|
| `0x05` | `JMP`    | Unconditional relative jump       |
| `0x06` | `JZ`     | Jump if zero flag set             |
| `0x07` | `JNZ`    | Jump if zero flag not set         |
| `0x08` | `JC`     | Jump if carry flag set            |
| `0x09` | `JO`     | Jump if overflow flag set         |
| `0x0A` | `JFE`    | Jump if EOF flag set              |
| `0x0B` | `HLT`    | Halt execution                    |

### Arithmetic & Bitwise

| Opcode | Mnemonic | Description                        |
|--------|----------|------------------------------------|
| `0x0C` | `INC`    | Increment register by 1            |
| `0x0D` | `DEC`    | Decrement register by 1            |
| `0x0E` | `ADD`    | Add two registers (two modes)      |
| `0x0F` | `SUB`    | Subtract two registers (two modes) |
| `0x10` | `XOR`    | Bitwise XOR                        |
| `0x11` | `OR`     | Bitwise OR                         |
| `0x12` | `AND`    | Bitwise AND                        |
| `0x13` | `LSR`    | Logical shift right by 1           |
| `0x14` | `LSL`    | Logical shift left by 1            |

---

## Instruction Encoding

Each instruction is **16 bits wide**:

```
Bits 15–11  │ Bits 10–8   │ Bits 7–4    │ Bits 3–0
  Opcode    │  Dest reg   │  Src1 reg   │  Src2 / constant / mode
  (5 bits)  │  (3 bits)   │  (4 bits)   │  (4 bits)
```

**ADD and SUB** support two modes selected by bit 7:
- **Mode 0:** `dest = dest OP src1`
- **Mode 1:** `dest = src1 OP src2`

---

## Flags Register (EFLAGS)

| Bit | Flag     | Set when...                              |
|-----|----------|------------------------------------------|
| 0   | Carry    | Unsigned overflow or underflow occurred  |
| 1   | Reserved | Always 1                                 |
| 2   | EOF      | Input file reached end of file           |
| 4   | Zero     | Result of last arithmetic/logic op is 0  |
| 5   | Overflow | Signed overflow occurred                 |

---

## How It Works

1. The emulator loads a compiled `.bin` program into RAM
2. It reads input data from a `.txt` file (used by `IN` instructions)
3. It executes instructions in a loop: **fetch → decode → execute**
4. Output from `OUT` instructions is written to `rez.txt`
5. Execution halts on `HLT` (opcode `0x0B`)

---

## Requirements

- **OS:** Windows, Linux, or macOS
- **Compiler:** Any C++11-compatible compiler (g++, clang++, MSVC)

---

## Building

```bash
g++ -std=c++11 -o CPU_Emulation CPU_Emulation.cpp
```

---

## Running

Place the following files in the same directory as the executable:

| File              | Description                         |
|-------------------|-------------------------------------|
| `MYDECRYPTOR.bin` | Compiled binary program             |
| `MYTEXT.txt`      | Input text for the `IN` instruction |

Then run:

```bash
./CPU_Emulation
```

Output will be written to `rez.txt`.

---

## Writing Programs

Programs must be hand-assembled into binary and saved as `.bin` files. Each instruction occupies exactly **2 bytes**.

**Example — copy one byte from input to output:**

```
; Read into R0, write R0 to output, halt
0x08 0x00   ; IN  R0
0x10 0x00   ; OUT R0
0x58 0x00   ; HLT
```

> ⚠️ An assembler for this architecture is not yet included. Programs must currently be assembled by hand.

---

## Limitations

- RAM is capped at 256 bytes, limiting program size to ~128 instructions
- No assembler — programs must be written directly in binary
- No stack or subroutine support (no CALL/RET instructions)
- Single-level I/O only (one input file, one output file)
