# Xonix Game - Data Structures & Algorithms Implementation

## Overview

**Xonix** is a recreational implementation of the classic Xonix arcade game, developed as an academic project to demonstrate proficient use of **data structures and algorithms** in C++. The project combines game development with practical applications of advanced programming concepts.

## Features

### Core Gameplay
- **Single-player and Multiplayer modes** with real-time enemy AI
- **Score tracking system** with player leaderboard
- **Power-ups and special mechanics** for enhanced gameplay
- **User authentication system** with player profiles
- **Friend system** for multiplayer matchmaking

### Data Structures & Algorithms Used

| Data Structure | Application |
|---|---|
| **Linked Lists** | Player data management and friend connections |
| **Hash Tables** | Fast player lookup and leaderboard caching |
| **Min Heap** | Priority-based enemy AI movement and power-up scheduling |
| **2D Grid/Arrays** | Game board representation and collision detection |
| **File I/O** | Persistent player data storage (CSV format) |

### Technical Stack
- **Language**: C++
- **Graphics Engine**: SFML (Simple and Fast Multimedia Library)
- **IDE**: Visual Studio 2022 (Windows)
- **Platform**: Windows x64

## Project Structure

```
Xonix/
├── Main.cpp                 # Game entry point and main loop
├── Game.h/Game2.h          # Game state management (menus, gameplay, screens)
├── Player.h/PlayerData.*   # Player profile and persistent data storage
├── Enemy.h                 # AI enemy behavior and pathfinding
├── HashTable.h             # Custom hash table implementation
├── LinkedList.cpp/h        # Custom linked list implementation
├── MinHeap.h               # Priority queue for AI scheduling
├── FriendSystem.*          # Friend management and multiplayer logic
├── Text.*                  # UI text rendering
├── Global.*                # Global constants and utilities
├── fonts/                  # Game fonts
├── Images/                 # Game sprites and textures
└── Xonix.vcxproj          # Visual Studio project file
```

## Building & Running

### Prerequisites
- **Visual Studio 2022** with C++ development tools
- **SFML 2.5+** library installed

### Compilation
1. Open `Xonix.vcxproj` in Visual Studio
2. Build the solution (Ctrl + Shift + B)
3. Run the executable from `/x64/Debug/`

## Key Implementations

### 1. **Linked List** (LinkedList.cpp/h)
- Manages dynamic player connections
- Used for friend list traversal and player queue management

### 2. **Hash Table** (HashTable.h)
- O(1) average-case player lookup
- Stores player statistics for quick leaderboard generation
- Collision handling with chaining

### 3. **Min Heap** (MinHeap.h)
- Prioritizes enemy movements based on distance to player
- Schedules power-up spawns to optimize gameplay
- Efficient event scheduling with O(log n) insertion/deletion

### 4. **Game Grid System**
- 50x80 grid-based board representation
- Efficient tile-state management using integer arrays
- Collision detection algorithms

## How to Play

1. **Login/Sign Up** - Create or access your player profile
2. **Main Menu** - Select single-player or multiplayer mode
3. **Gameplay** - Control your character while avoiding enemies
4. **Leaderboard** - View top players and friend rankings

## Author

- **Talha Qamar**
- NUCES-FAST University, Semester 4
- Data Structures Course Project

## Learning Outcomes

This project demonstrates:
- ✅ Proficient C++ programming and OOP principles
- ✅ Custom data structure implementation and optimization
- ✅ Real-time algorithm efficiency (game loop optimization)
- ✅ File I/O and persistent data management
- ✅ Game development fundamentals with graphics library integration
- ✅ UI/UX design with text rendering and menu systems

## Future Enhancements

- [ ] Network multiplayer support (LAN/Online)
- [ ] Advanced collision detection algorithms
- [ ] Dynamic difficulty scaling with adaptive AI
- [ ] Leaderboard persistence with database integration
- [ ] Mobile platform support

## License

This project is created for academic purposes under NUCES-FAST University.

---

**Repository**: [Xonix-DSA-CPP](https://github.com/Talha-Qamar/Xonix-DSA-CPP)
