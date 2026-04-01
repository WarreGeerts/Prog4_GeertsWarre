# BurgerTime - Game Engine & Implementation

[![CMake](https://github.com/WarreGeerts/Prog4_GeertsWarre/actions/workflows/cmake.yml/badge.svg)](https://github.com/WarreGeerts/Prog4_GeertsWarre/actions/workflows/cmake.yml)
[![Emscripten](https://github.com/WarreGeerts/Prog4_GeertsWarre/actions/workflows/emscripten.yml/badge.svg)](https://github.com/WarreGeerts/Prog4_GeertsWarre/actions/workflows/emscripten.yml)

This project is a recreation of the arcade classic **BurgerTime**, developed for the **Programming 4** course. This is the last programming course of the major Game Development (GD) at Digital Arts & Entertainment (DAE). It features a custom engine built upon the "Minigin" bootstrap, focused on implementing standard game programming patterns in C++.

**[Play the Web Version Here](https://warregeerts.github.io/Prog4_GeertsWarre/)**

---

## Engine Architecture

The project is split into two main sections: the **Minigin Engine** (core framework) and the **BurgerTime** (game-specific logic).

### Core Systems & Patterns
* **Component System:** Logic is encapsulated into reusable components (e.g., `RenderComponent`, `TextComponent`, `SpriteComponent`).
* **Scene Graph & Dirty Flag:** Implemented a hierarchical transform system. The **Dirty Flag** ensures that world positions are only recomputed when a local transform changes, optimizing the update loop.
* **Event Bus:** A centralized messaging system that allows decoupled communication between game objects and systems (e.g., UI updating when the player's score changes).
* **Command Pattern:** Used to decouple input handling from game logic, enabling support for both Keyboard and Gamepad.

### Cross-Platform Hardware Abstraction
* **Pimpl Idiom:** Specifically applied to the **Controller/Input** classes. This was chosen to "hide" the `XInput` dependencies required for Windows builds, allowing the **Emscripten** (Web) build to seamlessly swap in SDL3 controller logic without header conflicts or compilation errors.

---

## BurgerTime Gameplay

* **Game Modes:** Supports Single Player, Co-op, and Versus modes.
* **Input:** Fully playable via Keyboard and Gamepad.
* **Persistency:** Highscore list is saved locally and remains permanent between sessions.
* **Audio:** Music and SFX implementation with a global mute toggle (**F2**).
* **Level Management:** Level layouts are loaded from external data files, with a level-skip feature (**F1**) for debugging.

---

## Technical Requirements
* **Language:** C++ 20
* **Libraries:** SDL3 (Rendering/Input), GLM (Math), ImGui (Debugging)
* **Warning Level:** Compiled at **Level 4 (/W4)** with **Warnings as Errors (/WX)**.

---

## Credits
Developed by **Warre Geerts** as part of the Programming 4 Course at Howest DAE.
* Base Framework: [Minigin](https://github.com/avadae/minigin)
* References: [Game Programming Patterns](https://gameprogrammingpatterns.com/) by Robert Nystrom.
