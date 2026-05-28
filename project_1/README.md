

## How to Compile and Run

### Linux / macOS (g++ or clang++):
  ```bash
  g++ -std=c++11 -O2 -o project-1 project-1.cpp
  ./project-1
  ```

### Windows
  - Option 1 – MinGW / WSL with `g++`:
    ```bash
    g++ -std=c++11 -O2 -o project-1.exe project-1.cpp
    .\project-1.exe
    ```
  - Option 2 – Visual Studio Developer Command Prompt with `cl`:
    ```cmd
    cl /EHsc /O2 project-1.cpp /Fe:project-1.exe
    project-1.exe
    ```
