# Bad Apple in the Console (C, Windows)

A C program that plays the **Bad Apple!!** video right in the Windows console using ASCII characters, with sound.

## Demo

▶️ [Watch the result video on YouTube](https://www.youtube.com/watch?v=QY2z8OjYjCs)

## How it works

1. The video is converted with `ffmpeg` into raw black-and-white frames of 80×24 pixels (one brightness byte per pixel).
2. The program reads the frames one by one from the `frames.raw` file.
3. Every pixel brighter than the threshold is printed as `*`, the rest as a space.
4. Each frame is assembled in a buffer and printed in one go, so the picture doesn't flicker.
5. Frame timing is calculated with `GetTickCount64()`, so the video neither slows down nor speeds up.
6. Audio plays in parallel through `PlaySound` (WinMM).

## Requirements

- Windows
- gcc compiler (e.g. MinGW) or Visual Studio
- [ffmpeg](https://www.gyan.dev/ffmpeg/builds/) for preparing the files (`winget install Gyan.FFmpeg`)
- The `badapple.mp4` video file

## Project structure

```
bad_apple/
├── Skam.c          # source code
├── badapple.exe    # compiled program
└── system/
    ├── frames.raw  # frames (80x24, 15 fps)
    ├── badapple.wav # audio (optional)
    └── badapple.mp4 # original video
```

## Preparing the files

Create the `system` folder and generate the frames and audio:

```
mkdir system
ffmpeg -i badapple.mp4 -vf "fps=15,scale=80:24" -pix_fmt gray -f rawvideo system\frames.raw
ffmpeg -i badapple.mp4 system\badapple.wav
```

If `badapple.mp4` is somewhere else, just provide the path to it.

## Build and run

```
gcc Skam.c -o badapple.exe -lwinmm
.\badapple.exe
```

The `-lwinmm` flag is required for audio (`PlaySound`). In Visual Studio the library is linked automatically via `#pragma comment`.

Run the program from the folder that contains `system`, because the file paths are relative.

## Configuration

You can change these values in the code:

| Parameter | Default | Description |
|-----------|---------|-------------|
| `W`, `H`  | 80, 24  | frame size in characters |
| `FPS`     | 15      | frame rate |

If you change `W`, `H` or `FPS`, regenerate `frames.raw` with matching `fps=` and `scale=` parameters.

## Author

Anastasia Shestopal
