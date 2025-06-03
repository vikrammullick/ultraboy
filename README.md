# ultraboy

ultraboy consists of multiple emulators:

- Virtual Boy (in-progress)
- Game Boy (in-progress)
- Game Boy Advance (upcoming)

The eventual goal of this project is to support FPGA implementations for each emulator (similar to my [CHIP-8 emulator](https://github.com/vikrammullick/chip8)), alongside physical cartridge support

to build: `make`

usage: `./ultraboy [ROM PATH]`

## Virtual Boy

WIP but supports enough logic to render the following

![ultraboy_virtual_boy](https://github.com/user-attachments/assets/985ece47-cef4-46f1-9f05-9eddc62b6aa5)

Additionally, I generated JSON tests to help catch bugs while implementing my CPU. While incomplete (only up to the CPU instructions I currently have implemented), they are thorough and account for all relevant CPU flag scenarios.

## Game Boy

WIP but supports rendering to display and passes Blargg's test ROMs

<img width="160" alt="ultraboy_game_boy" src="https://github.com/user-attachments/assets/73a8e9f1-ad12-4963-b07d-4213cff8328f" />
