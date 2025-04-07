# GameOfLifeGPU

A GPU-accelerated version of Conway's Game of Life, implemented using OpenGL shaders for computation. This project uses SDL3 for window management, GLAD for OpenGL function loading, GLM for mathematics, ImGui for GUI and stb_image for image loading.

## Building the Project

```bash
# Configure and build
mkdir build
cmake -S . -B build
cmake --build build

# Run
./build/GameOfLife

# Package into a ZIP-archive for releasing
cd build
cpack
```
*NOTE: On Linux you might have to install additional packages to build SDL3, please refer to the [SDL3 FAQ](https://github.com/libsdl-org/SDL/blob/main/docs/README-linux.md)*

## Controls
- **Left Click:** Toggle the state of a cell (alive or dead).
- **Right Click + Drag:** Move around the grid to view different areas.
- **Mouse Scroll:** Zoom in and out.
- **H** - Toggle GUI Visibility.
- **F** - Toggle Fullscreen Mode.
- **E** - Iterate Once

## Current TODOs
- [x] Implement basic Conway's Game Of Life running on GPU
- [x] Add dragging, zooming
- [x] Set up GitHub Actions for automated releases
- [x] Implement random field generation on GPU
- [x] Add dynamic field resizing
- [ ] Experiment with visual effects
- [ ] Implement drawing on the field
- [ ] Emit signed macOS Bundle  

## Credits
- [Conway's Game of Life](https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life)
- [SDL3](https://github.com/libsdl-org/SDL)
- [GLAD](https://github.com/Dav1dde/glad)
- [GLM](https://github.com/g-truc/glm)
- [ImGui](https://github.com/ocornut/imgui)
- [STB](https://github.com/nothings/stb)