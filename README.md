# Building ATmega128 code in MacOS

## Steps:
1. Install Visual Studio Code.
2. Install **PlatformIO** extension in VS Code.
3. Open this project.
4. Change the `src/Main.c` code, this is your program for ATmega128.
5. Build the code using `command + option + B` shortcut, and check `.pio/build/ATmega128/firmware.hex` that was generated automatically.
6. Now, upload the `.hex` file in SimulIDE.
7. Upload the build in ATmega128 using `command + option + U` shortcut.
