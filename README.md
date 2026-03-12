# Assembly Language Simulator — Computer Architecture Project in C++

A beginner-friendly C++ project that simulates how a CPU executes 
assembly language instructions. Built as a Computer Architecture project.

---

## 🔧 How It Works

The program simulates the core cycle of every CPU:
```
FETCH → DECODE → EXECUTE → repeat
```

You type assembly instructions one by one, and the simulator 
executes them just like a real CPU would.

---

## 💻 Supported Instructions

| Instruction | Example | What it does |
|---|---|---|
| `MOV` | `MOV A 5` | Store value 5 in register A |
| `ADD` | `ADD A B` | A = A + B |
| `SUB` | `SUB A B` | A = A - B |
| `MUL` | `MUL A B` | A = A * B |
| `DIV` | `DIV A B` | A = A / B |
| `PRINT` | `PRINT A` | Display value of register A |
| `HALT` | `HALT` | Stop the program |

---

## ▶️ How to Run

### Compile
```bash
g++ -o simulator main.cpp
```

### Run
```bash
./simulator
```

### Example Input & Output
```
>> MOV A 5
>> MOV B 3
>> ADD A B
>> PRINT A
>> HALT

[MOV] A = 5
[MOV] B = 3
[ADD] A = 8
[PRINT] Register A = 8
[HALT] Program stopped.

Final Register State:
  A = 8
  B = 3
  C = 0
  D = 0
```

---

## 💡 Concepts Covered

| Concept | Description |
|---|---|
| Registers | Simulated using `map<string, int>` |
| Instruction Memory | Stored in `vector<string>` |
| Program Counter (PC) | `int pc` that moves forward each step |
| Fetch | Reading instruction using PC index |
| Decode | Parsing opcode using `stringstream` |
| Execute | Performing operation using if/else |
| ALU | Simulated using +, -, *, / operators |

---

## 🛠️ Tech Stack
- Language: C++
- Compiler: g++ (GCC)
- IDE: VS Code / IntelliJ

---

## 📚 What I Learned
- How a real CPU executes instructions internally
- Fetch-Decode-Execute cycle simulation
- How registers and program counter work
- String parsing and instruction decoding in C++

---

## 🔗 Related Project
- [Code Generator — Compiler Design](https://github.com/Poornachandra-creater/code-generator-cpp)
  > The Code Generator writes assembly instructions that this simulator can execute!
