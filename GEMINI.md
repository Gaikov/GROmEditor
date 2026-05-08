# GROmEditor Project Context

## Project Overview
GROmEditor is a comprehensive cross-platform scene editor (GROm Scene Viewer) built as an application on top of the **GROmEngine**. It is designed for creating and managing game scenes, particle effects, and visual configurations.

### Key Technologies
- **Language:** C++20
- **Build System:** CMake (minimum 3.10, 3.13 used in root)
- **Graphics:** OpenGL 3+ / GLES3 (via glad), WebGL (for WASM)
- **Audio:** OpenAL (Desktop), OpenSLES (Android)
- **UI:** ImGui
- **Windowing:** GLFW (Desktop)
- **Platforms:** Windows, Linux, MacOS, Android, HTML5/WebAssembly

## Agent Workflow Constraints
- **Building and Testing:** The user handles building and testing the project manually. Do not attempt to run build or test commands (e.g., `cmake`, `nsLib_unit`, etc.) unless explicitly requested. Focus on code analysis, modification, and architectural guidance.

## Building and Running (Reference)
*Note: These commands are for reference; the user performs these steps manually.*
### Standard Desktop Build
```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```
The executable `GROmEditor` (or `GROmEditor.exe` on Windows) will be located in the `build` directory.

### WebAssembly Build
```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release -DWEB_ASM=1
cmake --build build
```

### Build Options
- `-DNO_NETWORKING=1`: Exclude the networking subsystem.
- `-DWEB_ASM=1`: Target WebAssembly.
- `-DCMAKE_BUILD_TYPE=Debug`: Enable debug definitions and symbols.

### Execution
The application expects the working directory to be the `assets/` folder. All asset paths in the code are relative to this directory.

## Testing (Reference)
*Note: The user performs testing manually.*
Tests are primarily located within the `GROmEngine` submodule.

```bash
./build/nsLib_unit   # Math, strings, events, locks
./build/Core_unit    # Parser, crypto, undo, memory
./build/TestsApp     # Functional and integration tests
```

## Architecture

### GROmEngine (Submodule)
A layered engine providing low-level utilities and high-level game systems:
1.  **nsLib:** Core utilities (math, data structures, event dispatcher).
2.  **Core:** Config, I/O, serialization, memory pooling, undo/redo service.
3.  **Platform:** Windowing and entry points for Desktop/Android.
4.  **Rendering:** IRenDevice implementations for GLES3 and legacy GL.
5.  **Audio:** ISoundDevice implementations for OpenAL and OpenSLES.
6.  **Engine:** Top-level subsystems (Display tree, Particles, Fonts, Input).

### GROmEditor
Built using an MVC pattern and implementing the `IGameApp` interface:
- **Entry Point:** `EditorSources/SceneViewerApp.cpp`
- **Model:** `nsAppModel` (singleton) manages project state and settings.
- **View:** `nsViewsRoot` manages ImGui panels. `BaseView` is the base class for all UI panels.
- **UI Components:** Found in `view/components/`, these widgets are integrated with `nsUndoService` for automatic undo/redo support.

## Development Conventions

### Naming Conventions
- **Classes:** Use `ns` prefix (e.g., `nsProjectModel`, `nsVec2`). Interfaces use `I` prefix (e.g., `IRenDevice`).
- **Methods:** PascalCase (e.g., `Shared()`, `AddChild()`, `IsValid()`).
- **Private/Protected Members:** Trailing underscore (e.g., `_appModel`, `_children`).
- **Public Members:** No prefix/suffix (e.g., `x`, `y`, `id`).

### Coding Style
- **Indentation:** 4 spaces.
- **Braces:** Open brace `{` on the same line as the declaration (functions, classes, control flow).
- **Header Guards:** Use `#pragma once`.
- **Smart Pointers:** Prefer `auto` and modern C++ memory management where possible.
- **Singletons:** Access singleton subsystems via `T::Shared()`.

### Patterns
- **Service Locator/Singleton:** Subsystems are often singletons initialized on first access.
- **Undo/Redo:** All editor mutations should use the `nsUndoService` command pattern. UI components in `view/components/` wrap this by default.
- **Event-Driven:** Use `nsEventDispatcher` for decoupled communication.

## Asset Management
- Editor-specific assets: `assets/`
- All paths should be relative to `assets/`.
- Project settings are serialized to `assets/editor.ggml`.
