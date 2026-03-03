##💣 Minesweeper – C++ / SFML

A fully-featured Minesweeper game built in C++ using SFML, featuring multiple game modes, difficulty levels, and a custom polymorphic effects system.

Built using CMake + SFML 3 with cross-platform support.


##📸 Screenshots
#Title Screen
<img width="764" alt="Title Screen" src="https://github.com/user-attachments/assets/427e3039-cf47-4fb6-8597-7e1a054d630f" />
#Difficulty Selection
<img width="1101" alt="Difficulty Screen" src="https://github.com/user-attachments/assets/f59536b0-d3d9-4d2d-a7a5-8775cc1d06f3" />
#Game Mode
<img width="1279" alt="Game Mode" src="https://github.com/user-attachments/assets/cb02b185-796d-4e71-8e53-8906d6f7d9b4" />
#Demolition Mode
<img width="976" alt="Demolition Mode" src="https://github.com/user-attachments/assets/80f5b398-98ba-44ef-b2e0-9ff5327777d5" />
#Effects System
<img width="999" alt="Effects System" src="https://github.com/user-attachments/assets/3ab919ea-9408-4503-b0b3-04939f70f630" />
#Gameplay Example
<img width="1803" alt="Gameplay" src="https://github.com/user-attachments/assets/085a0b79-c904-4aaa-833e-693355775eca" />

##🎮 Features
#🏠 Title Screen

Play

Demolition Mode

Exit

#🎯 Difficulty Selection

Easy

Medium

Hard

Back to Title

#💣 Classic Mode

Left-click to reveal tiles

Right-click to flag tiles

Automatic flood fill for empty cells

Score system

Win when all safe tiles are revealed

Lose when clicking a mine

#🔥 Demolition Mode (Arcade Variant)

Start with 5 lives

Clicking a mine reduces a life

Game ends when lives reach 0

Win when all mines are identified

Includes explosion visual & sound effects

#✨ Effects System (Polymorphism)

The game includes a modular, object-oriented effects system.

Abstract Base Class
class Effect {
public:
    virtual bool update(float dt) = 0;
    virtual void draw(sf::RenderWindow& window) = 0;
    virtual ~Effect() = default;
};
Implemented Effects

RingWaveEffect – Expanding ring animation

ScreenFlashEffect – Flash overlay

ExplosionSoundEffect – Explosion audio

Lifecycle

Spawned dynamically when a mine is triggered

Updated every frame

Automatically removed when finished

##🧠 Architecture Overview

The project follows a screen/state-driven structure:

Title
 ├── Difficulty
 │     ├── Easy
 │     ├── Medium
 │     └── Hard
 └── Demolition Mode

Each mode:

Creates its own window

Handles events

Loads UI assets

Manages transitions independently

##📂 Project Structure

Based on your current repository:

.
├── .github/workflows/      # GitHub Actions CI
├── cmake-build-debug/      # CMake build output
├── cmake-build-debug-even... 
├── src/                    # Source files
│   ├── main.cpp
│   ├── Title.h
│   ├── Difficulty.h
│   ├── Easy.h
│   ├── Medium.h
│   ├── Hard.h
│   ├── Demolition.h
│   └── Effects.h
│
├── imagesAudio/            # Game images & sounds
├── CascadiaCode.ttf        # Font
├── CMakeLists.txt
├── LICENSE.md
└── README.md
##🛠 Technologies Used

C++17

SFML 3

Graphics

Window

Audio

CMake

##GitHub Actions (CI workflow included)

🚀 Building the Project
#🔧 Requirements

CMake 3.16+

C++17 compatible compiler

Git

Internet connection (SFML fetched automatically)

#🖥 Build Instructions

From the project root:

cmake -B build
cmake --build build

Run the executable:

./build/<your-executable-name>

On Windows (Visual Studio generator):

cmake -B build -G "Visual Studio 17 2022"
cmake --build build --config Release
🐧 Linux Dependencies (Ubuntu/Debian)

If using Linux, install SFML dependencies:

sudo apt update
sudo apt install \
    libxrandr-dev \
    libxcursor-dev \
    libxi-dev \
    libudev-dev \
    libfreetype-dev \
    libflac-dev \
    libvorbis-dev \
    libgl1-mesa-dev \
    libegl1-mesa-dev
🎮 Controls
Action	Input
Reveal tile	Left Click
Flag tile	Right Click
Reset	Reset Button
Back	Back Button
Exit	ESC
🏆 Win / Lose Conditions
#Classic Mode

✅ Win → All safe cells revealed

❌ Lose → Mine clicked

#Demolition Mode

✅ Win → All mines identified

❌ Lose → Lives reach 0

📈 Learning Objectives Demonstrated

Object-Oriented Programming

Polymorphism

Abstract Base Classes

Event-driven architecture

Real-time rendering loop

Resource management

CMake project configuration

SFML Graphics & Audio integration

🔮 Future Improvements

High score saving

Timer

Custom board sizes

Tile animations

Particle system

Save/load functionality

Refactor into explicit state manager class

#📜 License

This project uses the SFML CMake template.

The template is dual licensed under:

Public Domain

MIT License

Choose whichever you prefer.


