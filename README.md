# GROmEditor

GROmEditor is a 2D scene editor built on top of [GROmEngine](./GROmEngine). It is designed for assembling and editing game UI/layout scenes, visual objects, sprites, custom objects, particles, and reusable layouts directly inside an editor workflow.

The project is currently in active development. The editor already includes a scene hierarchy, property inspectors, undo/redo-based editing, transform tools, asset browsing, custom visual metadata, and layout-oriented workflows.

## Demo

[![GROmEditor Demo: 2D Character Editing and Transform Tools](https://img.youtube.com/vi/wo8Y5PJ7i1c/maxresdefault.jpg)](https://www.youtube.com/watch?v=wo8Y5PJ7i1c)

Watch the demo on YouTube: https://www.youtube.com/watch?v=wo8Y5PJ7i1c

## Features

- 2D scene and layout editing
- Scene hierarchy and object inspector
- Transform tools for positioning, scaling, and rotating objects
- Sprite, container, text, particle, polygon, and custom visual support
- Asset browser with layout and image workflows
- Undo/redo integration for editor mutations
- Project and session state serialization
- Built on GROmEngine with C++20 and CMake

## Building

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

For a debug build:

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
```

The resulting editor executable is generated in the build directory.

## Repository Layout

- `EditorSources/` - editor application, views, tools, models, and UI components
- `GROmEngine/` - engine submodule
- `assets/` - editor assets and default project resources
- `Materials/` - project materials/resources

## Status

GROmEditor is a work-in-progress internal editor/dev tool. APIs, workflows, and file formats may continue to evolve while the editor is being actively developed.
