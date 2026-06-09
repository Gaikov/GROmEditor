# AGENTS.md

## Build

CMake (minimum 3.13), C++20. The executable on Windows link with `-mwindows` and `-static-libgcc -static-libstdc++`.

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

Debug builds define `DEBUG` (`-DCMAKE_BUILD_TYPE=Debug`). WebAssembly: add `-DWEB_ASM=1`. Exclude networking: `-DNO_NETWORKING=1`.

The resulting editor executable is `build/GROmEditor` (Windows: `build/GROmEditor.exe`, WASM: `build/index.html`). See `GROmEngine/CLAUDE.md` for the full engine architecture, dependencies, rendering/sound backends, and code style.

## Tests

Tests live in the `GROmEngine` submodule. After building:

```bash
./build/nsLib_unit   # math, strings, events, locks
./build/Core_unit    # parser, crypto, undo, memory
./build/TestsApp     # functional/integration tests
```

## Architecture

GROmEditor is a **scene editor** (GROm Scene Viewer) built as a game on top of GROmEngine. It follows MVC and implements `IGameApp` from the engine.

### Entry point

`EditorSources/SceneViewerApp.cpp` — implements `IGameApp`. Initializes `nsAppModel`, `nsMainView`, and routes input/draw into the UI layer.

### Model layer

- `models/AppModel` — top-level singleton (`nsAppModel::Shared()`); owns project state, settings, undo service wiring
- `models/project/ProjectModel` — active project data: scenes, particles, custom visual types
- `models/AppSettings` — editor preferences serialized to/from `editor.ggml`

### View layer (ImGui-based)

- `view/ViewsRoot` — root view manager subsystem; owns and ticks all panels
- `view/BaseView` — abstract base for all panels
- `view/library/LibraryView` — asset/scene tree (SceneTreeView) and property inspector (ScenePropsView)
- `view/popups/PopupsStack` — modal dialog manager
- `view/MainMenuBar`, `view/TopBarView` — top-level UI chrome

### UI components

`view/components/` (~70 files) — reusable input widgets that integrate with `nsUndoService`:
- `*InputUndo` — text, int, float, bool, color, angle, Vec2 inputs that push undo commands
- `*SelectUndo` — asset pickers (texture, font, particles, render state, layout)

All user edits go through these components to ensure undo/redo coverage.

### Display / visual system

- `display/VisualRefBuilder` + `display/VisualRefFactory` — construct engine visual objects from model data
- `display/lifecycle/` — per-type property views (inspector panels) for visual objects in the scene

### Engine submodule

`GROmEngine/` is a git submodule. Refer to `GROmEngine/CLAUDE.md` for the full engine architecture (nsLib → Core → Platform → Rendering → Audio → Engine layers), key patterns (`nsSubSystem<T>`, `nsEventDispatcher`, `nsResourcesCache<T>`, `IGameApp`), and external dependencies.

## Assets

Editor-specific assets live in `assets/`. The application working directory is set to `assets/`, so all asset paths in code are specified **relative to the `assets/` folder** (e.g., `default/editor/font.ttf`).

## Key patterns

**Singleton subsystems** — access via `T::Shared()` (e.g., `nsAppModel::Shared()`, `nsViewsRoot::Shared()`). Subsystems are registered and resolved through the service locator.

**Undo/Redo** — every editor mutation uses `nsUndoService` command pattern. UI components in `view/components/` wrap this automatically; new editor actions must do the same.

**Naming** — classes use `ns` prefix (e.g., `nsProjectModel`); private members use trailing `_` (e.g., `_appModel`).
