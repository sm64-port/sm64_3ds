# Super Mario 64 3DS Port

This repo contains a full decompilation of Super Mario 64 (J), (U), and (E), ported to the 3DS.

This repo does not include all assets necessary for compiling the port.
A prior copy of the game is required to extract the required assets.

## Installation

### Linux

#### 0. Cloning repo

Run `git clone https://github.com/sm64-port/sm64_3ds.git`.

#### 1. Copy baserom(s) for asset extraction

For each version (jp/us/eu) that you want to build a ROM for, put an existing ROM at
`./baserom.<version>.z64` for asset extraction.

#### 2. Install build dependencies

The build system has the following package requirements:
```
python3 >= 3.6
libaudiofile
devkitPro
├──devkitARM
├──devkitarm-crtls
├──devkitarm-rules
├──3dstools
├──citro3d
├──libctru
└──picasso
```

__Debian / Ubuntu__
```
sudo apt install build-essential pkg-config git python3 zlib1g-dev libaudiofile-dev
```

__Arch Linux__
```
sudo pacman -S base-devel python audiofile
```

Now follow https://devkitpro.org/wiki/devkitPro_pacman to install devkitPro. 
Run `sudo (dkp-)pacman -S 3ds-dev` after completing the `Updating Databases` section.

#### 3. Build ROM

Run `make` to build the ROM (defaults to `VERSION=us`). Make sure your path to the repo 
is not too long or else this process will error, as the emulated IDO compiler cannot 
handle paths longer than 255 characters.
Examples:
```
make -j4                  # build (U) version with 4 jobs
make VERSION=jp -j4       # build (J) version instead with 4 jobs
```

## Windows

For Windows, install devkitPro by following
[devkitPro Installation Guide for Windows](https://devkitpro.org/wiki/Getting_Started#Windows).

Then follow the directions in the [Linux](#linux) installation section above, using MSYS to execute commands.


## Project Structure

```
sm64
├── actors: object behaviors, geo layout, and display lists
├── asm: handwritten assembly code, rom header
│   └── non_matchings: asm for non-matching sections
├── assets: animation and demo data
│   ├── anims: animation data
│   └── demos: demo data
├── bin: asm files for ordering display lists and textures
├── build: output directory
├── data: behavior scripts, misc. data
├── doxygen: documentation infrastructure
├── enhancements: example source modifications
├── include: header files
├── levels: level scripts, geo layout, and display lists
├── lib: SDK library code
├── sound: sequences, sound samples, and sound banks
├── src: C source code for game
│   ├── audio: audio code
│   ├── buffers: stacks, heaps, and task buffers
│   ├── engine: script processing engines and utils
│   ├── game: behaviors and rest of game source
│   ├── goddard: Mario intro screen
│   └── menu: title screen and file, act, and debug level selection menus
├── text: dialog, level names, act names
├── textures: skybox and generic texture data
└── tools: build tools
```

## Contributing

Pull requests are welcome. For major changes, please open an issue first to
discuss what you would like to change.

Run clang-format on your code to ensure it meets the project's coding standards.
