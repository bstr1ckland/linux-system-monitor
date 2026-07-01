# linux-hardware-monitor / lhmon
System hardware monitor for Linux AMD CPU + GPU systems.

This montoring app program to report statistics of hardware usage (%), temperature (°C), and more.

<img width="869" height="594" alt="lhmon-example" src="https://github.com/user-attachments/assets/2b0faf30-b896-42c7-b192-06ecc52c9d59" />

## Features
 
- **System** — kernel version, distro (with distro-aware coloring), uptime
- **CPU** — model name, max clock speed, overall usage, per-core usage grid, temperature (via `k10temp`)
- **GPU** — model name, usage, VRAM used/total, temperature (via `amdgpu` hwmon)
- **Memory** — used/total RAM
## Requirements
 
- Linux, with an **AMD CPU** (`k10temp` hwmon driver) and **AMD GPU** (`amdgpu` driver)
- C++20 compiler (e.g. `g++` 10+)
- [FTXUI](https://github.com/ArthurSonzogni/FTXUI) installed (`ftxui-component`, `ftxui-dom`, `ftxui-screen`)
- `lspci` available on `PATH` (used to detect the GPU model)
 
## Building
 
### With CMake
 
```bash
make
```
 
## Usage
 
Run the built binary from a terminal:
 
```bash
./lhmon
```
 
- Use arrow keys / mouse to navigate and toggle the collapsible `[ system ]`, `[ cpu ]`, `[ gpu ]`, and `[ memory ]` sections.
- Press `Ctrl+C` to quit (standard FTXUI fullscreen behavior).
The display refreshes continuously in the background — stats are read on a separate thread and pushed to the UI as they update.
 
## Project structure
 
```
.
├── include/        # Header files (cpu.h, gpu.h, ram.h, system.h, utils.h)
├── src/             # Implementation files + main.cpp (UI + thread orchestration)
├── CMakeLists.txt   # CMake build definition
├── Makefile         # Simple g++ build (outputs `lhmon`)
└── LICENSE          # MIT
```
 
## Known limitations
 
- GPU paths are hardcoded to `/sys/class/drm/card1`, so multi-GPU or non-standard setups may need a manual path change.
- Built and tested for AMD CPU/GPU systems specifically — Intel CPUs and Nvidia GPUs aren't supported.
## License
 
MIT — see [LICENSE](LICENSE).
