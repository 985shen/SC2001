# SC2001 Algorithm Design and Analysis

## Projects

| # | Topic | Algorithms |
|---|-------|------------|
| 1 | Sorting | Merge Sort, Insertion Sort & Hybrid Sort |
| 2 | Shortest Paths | Dijkstra's Algorithm |
| 3 | Dynamic Programming | — |

## How to Compile and Run

Each project is a standalone `.cpp` file. Replace `project-1` below with the file you want to build.

### Linux / macOS

Using `g++` or `clang++`:

```bash
g++ -std=c++11 -O2 -o project-1 project-1.cpp
./project-1
```

### Windows

**Option 1 — MinGW / WSL with `g++`:**

```bash
g++ -std=c++11 -O2 -o project-1.exe project-1.cpp
.\project-1.exe
```

**Option 2 — Visual Studio Developer Command Prompt with `cl`:**

```cmd
cl /EHsc /O2 project-1.cpp /Fe:project-1.exe
project-1.exe
```
