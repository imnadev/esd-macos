# Building ATmega128 code in MacOS

## Steps:
1. Install Visual Studio Code.
2. Install **PlatformIO** extension in Visual Studio Code.
3. Open this project in VS Code.
4. Change the `src/Main.c` code, this is your ATmega128 program.
5. Build the code using `control + option + B` shortcut.
6. Check `.pio/build/ATmega128/firmware.hex` file that was generated automatically.
a. Upload the `.hex` file in SimulIDE ATmega128 simulator.
b. Upload the build in ATmega128 real hardware using `control + option + U` shortcut.
