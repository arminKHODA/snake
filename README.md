# Snake Game

This is a simple implementation of the classic Snake game using C++ and SDL2.

## Table of Contents

- [Introduction](#introduction)
- [Features](#features)
- [Requirements](#requirements)
- [Installation](#installation)
- [Usage](#usage)
- [Controls](#controls)
- [License](#license)

## Introduction

This project is a basic Snake game where the player controls a snake that grows in length each time it eats food. The goal is to make the snake as long as possible without colliding with the walls or itself.

## Features

- Simple and intuitive gameplay
- Score tracking
- Restart and exit options on game over

## Requirements

- C++11 or later
- [SDL2](https://www.libsdl.org/) library
- [SDL2_ttf](https://www.libsdl.org/projects/SDL_ttf/) library

## Installation

1. Clone the repository:
    ```bash
    git clone https://github.com/yourusername/snake-game.git
    cd snake-game
    ```

2. Install SDL2 and SDL2_ttf libraries. On Ubuntu, you can use:
    ```bash
    sudo apt-get install libsdl2-dev libsdl2-ttf-dev
    ```

3. Compile the game:
    ```bash
    g++ -o snake_game main.cpp -lSDL2 -lSDL2_ttf
    ```

## Usage

Run the compiled executable to start the game:
```bash
./snake_game
