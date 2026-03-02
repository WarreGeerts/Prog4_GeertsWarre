# Minigin

Minigin is a very small project using [SDL3](https://www.libsdl.org/) and [glm](https://github.com/g-truc/glm) for 2D c++ game projects. It is in no way a game engine, only a barebone start project where everything sdl related has been set up. It contains glm for vector math, to aleviate the need to write custom vector and matrix classes. We as students have to make an engine from this barebone project for our final programming 4 exam.

[![CMake](https://github.com/WarreGeerts/Prog4_GeertsWarre/actions/workflows/cmake.yml/badge.svg)](https://github.com/WarreGeerts/Prog4_GeertsWarre/actions/workflows/cmake.yml)
[![Emscripten](https://github.com/WarreGeerts/Prog4_GeertsWarre/actions/workflows/emscripten.yml/badge.svg)](https://github.com/WarreGeerts/Prog4_GeertsWarre/actions/workflows/emscripten.yml)
<!--
For later, currently no versions
[![GitHub Release](https://img.shields.io/github/v/release/avadae/minigin?logo=github&sort=semver)](https://github.com/avadae/minigin/releases/latest) -->

# Goal

Minigin can/may be used as a start project for the exam assignment in the course [Programming 4](https://youtu.be/j96Oh6vzhmg) at DAE. In that assignment students need to recreate a popular 80's arcade game with a game engine they need to program themselves. During the course we discuss several game programming patterns, using the book '[Game Programming Patterns](https://gameprogrammingpatterns.com/)' by [Robert Nystrom](https://github.com/munificent) as reading material. 

# Disclaimer

Minigin is, despite perhaps the suggestion in its name, **not** a game engine. It is just a very simple SDL3 ready project with some of the scaffolding in place to get started. None of the patterns discussed in the course are used yet (except singleton which use we challenge during the course). It is up to the students to implement their own vision for their engine, apply patterns as they see fit, create their game as efficient as possible.

## Github Actions

This project is build with github actions.
- The CMake workflow builds the project in Debug and Release for Windows and serves as a check that the project builds on that platform.
- The Emscripten workflow generates a web version of the project and publishes it as a [github page](https://warregeerts.github.io/Prog4_GeertsWarre/). 

## Emscripten web version

<iframe style="position: absolute; top: 0px; left: 0px; width: 1024px; height: 576px;" src="https://warregeerts.github.io/Prog4_GeertsWarre/" loading="lazy"></iframe>

