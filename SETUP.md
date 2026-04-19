# Setup & Build Instructions

## Prerequisites

### Windows (Recommended)
- **Visual Studio 2022** Community Edition (Free) or higher
- **C++ Desktop Development** workload installed
- **SFML 2.5+** library

### macOS / Linux
- **CMake 3.16+**
- **GCC 9.0+** or **Clang 10.0+**
- **SFML 2.5+** library

## Installation Steps

### 1. Clone the Repository
```bash
git clone https://github.com/Talha-Qamar/Xonix-DSA-CPP.git
cd Xonix-DSA-CPP
```

### 2. Install SFML Library

#### Windows (Visual Studio)
1. Download SFML from [sfml-dev.org](https://www.sfml-dev.org/download/sfml/2.6.0/)
2. Extract to `C:\SFML` or your preferred location
3. Open `Xonix.vcxproj` in Visual Studio
4. Configure project properties:
   - **VC++ Directories → Include Directories**: Add `C:\SFML\include`
   - **VC++ Directories → Library Directories**: Add `C:\SFML\lib`
   - **Linker → Input → Additional Dependencies**: Add SFML libraries
     - `sfml-graphics.lib`, `sfml-window.lib`, `sfml-system.lib`

#### macOS (Homebrew)
```bash
brew install sfml
```

#### Linux (Ubuntu/Debian)
```bash
sudo apt-get install libsfml-dev
```

### 3. Build the Project

#### Windows (Visual Studio)
1. Open `Xonix.vcxproj` in Visual Studio
2. Select **Build → Build Solution** (Ctrl + Shift + B)
3. Executable created in `x64/Debug/Xonix.exe`

#### macOS/Linux (CMake - Optional)
```bash
mkdir build
cd build
cmake ..
make
./Xonix
```

## Troubleshooting

| Issue | Solution |
|-------|----------|
| **SFML headers not found** | Ensure SFML include path is added to Visual Studio Include Directories |
| **Linker errors** | Verify SFML library files (.lib) are properly linked in Linker settings |
| **.exe fails to run** | Copy SFML DLLs to Debug folder or add SFML bin directory to PATH |
| **Graphics don't display** | Ensure `fonts/` and `Images/` directories exist in run directory |

## Running the Game

The game will launch with:
1. Login/Sign-up screen
2. Main menu with game mode selection
3. Interactive gameplay with real-time rendering

Enjoy playing Xonix!
