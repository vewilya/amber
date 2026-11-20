# Amber

[![CI](https://github.com/vewilya/amber/actions/workflows/ci.yml/badge.svg)](https://github.com/vewilya/amber/actions/workflows/ci.yml)

A real-time audio saturation effect driven by a tiny neural network. A simple proof of concept with a python-torch driven training part and the inference being done in C.

A small MLP is trained in PyTorch to mimic an **asymmetric soft-saturation curve**
(`tanh` on the positive half, `sin` on the negative half). Its weights are exported to a C header and run sample-by-sample inside a plain C audio pipeline built on PortAudio.

```
 PyTorch (train)  ──►  model_weights.h  ──►  torchline (C inference)  ──►  Amber plugin  ──►  PortAudio I/O
```

**DISCLAIMER:** This is work in progress. There's multiple parts of the pipeline that still need fixing and improving.

**DSP & model**

- Gain staging: drive is applied both in training and at runtime, and output peaks at about −9.5 dBFS.
- Only dense layers so far; RNN and convolution layers are needed for effects with memory.
- Weights are compiled in, so only one model per build and no loading at runtime.
- No oversampling yet, so the saturation aliases.
- No DC blocker after the asymmetric curve.
- The 2→16→2 model mixes left and right; a per-channel. Depending on what is modeled a 1→N→1 model would be cleaner.
- No parameter smoothing, so drive changes will cause zipper noise.

**Real-time**

- `isEffectOn` / `keepRunning` are shared with the audio thread without being atomic.
- Streams use PortAudio's high-latency defaults with 1024-frame buffers.

**Pipeline & tests**

- No test checking that the C inference matches PyTorch output.
- `torchline export` is a stub, and export paths depend on the working directory.
- `test_filter.c` is disabled.
- Device indices entered at startup are not validated.

## Project layout

```
.
├── main.c                  # PortAudio host: device selection, stream, effect on/off
├── CMakeLists.txt
├── CMakePresets.json       # dev / release / coverage / ci
├── cmake/                  # compiler options, format/docs/coverage targets
├── run.sh                  # build (dev preset) and run
└── libs/
    ├── dsp/                # plugin interface, plugin chain, effects, Q15 math
    │   ├── include/dsp/
    │   │   ├── plugin.h          # Plugin vtable (process / set_parameter / destroy)
    │   │   ├── plugin_handler.h  # global plugin chain
    │   │   ├── amber.h           # the neural saturator
    │   │   ├── one_pole.h        # one-pole lowpass
    │   │   ├── q15.h             # fixed-point helpers
    │   │   └── inline_test.h     # TEST() macros for tests living next to code
    │   ├── src/
    │   ├── tests/
    │   └── benchmarks/     # Google Benchmark (C++)
    └── torchline/          # minimal feed-forward inference engine in C
        ├── include/torchline/torchline.h
        ├── src/
        │   ├── engine.c
        │   └── model_weights.h   # generated — do not edit by hand
        ├── tests/
        ├── benchmarks/
        └── python/         # training + export (uv project)
            ├── src/torchline/
            └── models/     # .pt / .safetensors / .json snapshots
```

## Requirements

- macOS (Homebrew) or another Unix with a C23-capable compiler (recent Clang/GCC)
- CMake ≥ 3.25
- [PortAudio](https://www.portaudio.com/) (found via `pkg-config` as `portaudio-2.0`)
- [Criterion](https://github.com/Snaipe/Criterion) for unit tests
- [Google Benchmark](https://github.com/google/benchmark) (C++) for benchmarks
- clang-format, clang-tidy (from `llvm`) and Doxygen for the developer targets
- [uv](https://docs.astral.sh/uv/) and Python ≥ 3.15 for training (optional)

```sh
# macOS
brew install cmake pkg-config portaudio criterion google-benchmark llvm clang-format doxygen uv
# Ubuntu: see .github/workflows/ci.yml

# Optional, for clangd: point it at the dev build's compile database
ln -s build/dev/compile_commands.json compile_commands.json
```

## Commands

| Task | Command |
|---|---|
| Build and test (debug, sanitizers) | `cmake --preset dev && cmake --build --preset dev && ctest --preset dev` |
| Build and run the plugin | `./run.sh` (or `./build/dev/amber` after a build) |
| Run benchmarks (optimized) | `cmake --preset release && cmake --build --preset release && ./build/release/libs/dsp/dsp_benchmarks` |
| Coverage report | `cmake --preset coverage && cmake --build --preset coverage --target coverage` |
| Format code | `cmake --build --preset dev --target format` |
| Build docs | `cmake --build --preset dev --target docs`, then open `build/dev/docs/html/index.html` |
| Everything CI runs (clang-tidy, tests, format-check) | `cmake --preset ci && cmake --build --preset ci && ctest --preset ci && cmake --build --preset ci --target format-check` |

The same commands are available as Zed tasks (`.zed/tasks.json`).

## Running

At startup you pick an input and an output device by index. While running:

| Key | Action              |
| --- | ------------------- |
| `1` | Effect on           |
| `2` | Effect off (bypass) |
| `4` | Stop and quit       |

Audio runs at 48 kHz, stereo, 1024-frame buffers (see `main.c`).

## Tests

```sh
ctest --preset dev
```

Some tests live inline in headers via `inline_test.h`; they compile only with
`-DRUN_TESTS` (the Zed task _Test current file_ does this for a single file).

## Training a new model

Run from `libs/torchline/python` — the export paths are relative to that directory:

```sh
cd libs/torchline/python
uv run torchline train
```

This will:

1. train a `2 → 16 (tanh) → 2` network on the asymmetric saturation curve,
2. show a plot comparing the model against the target curve,
3. save snapshots to `models/` (`.pt`, `.safetensors`, `.json`),
4. regenerate `libs/torchline/src/model_weights.h`.

Rebuild the C project afterwards to pick up the new weights.

## Writing a plugin

Every effect implements the `Plugin` interface from `dsp/plugin.h`:

```c
struct Plugin_t {
    const char *name;
    void  (*process)(Plugin *self, float *buffer, size_t num_frames);   // interleaved, in-place
    Err_t (*set_parameter)(Plugin *self, const char *name, float value);
    void  (*destroy)(Plugin *self);
};
```

Create it with a `*_create()` function and add it to the chain:

```c
Plugin *amber = amber_create(48000.0, 2, 3.0f);   // drive: 0.0 – 3.0
register_plugin_to_chain(amber);
```

`shutdown_all_plugins()` destroys everything in the chain on exit.
