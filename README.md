# Sharp

### What is it?

Sharp is a C API for easily creating and managing window contexts. This includes rendering to a window, and polling window events such as key presses, mouse movements, ect.

Currently lacking many features and compatibility, there isn't much to be said right now. Keep in mind, Sharp is fully compatible with C++ as well.

#### Planned

1. Full suite of software rendering tools. Currently, only rendering rectangles is available, however rendering **points, lines, arcs, and more** are planned.
2. Hardware rendering. Once software rendering is complete, hardware rendering through OpenGL and possibly Vulkan will be implemented.
3. Cross platform. Currently, this API is only compatible with X Server on Linux. In the future, compatibility with Wayland and Windows is planned. MacOS compatibility is not currently planned.
4. UI tooling. Similar to that of Qt, UI tooling is planned, but this will most likely be after a 1.0 release.
5. Lua API. After a 1.0 release, a Lua compatible library will be released alongside the main API.
6. Example programs will eventually be part of this repository.

#### To Note

This API is still heavily under development, and design goals and choices are still rapidly changing. Currently, the scope is based around shape rendering, future goals include text rendering, audio and image loading, and UI/Widget elements.

Also, currently all rendering is done as _Software Rendering_. This is much slower and more cumbersome than _Hardware Accelerated_ rendering, as pixel data is stored and read through RAM, and processed on the CPU.
**Shared Memory (SHM)** is being used on X11 to mitigate the performace bottleneck, and is currently _REQUIRED_. This will change shortly to accomodate those without access to **SHM** on X11 platforms.

## Building

> [!NOTE]
> Currently, this API is ONLY available for Linux users. It will NOT compile on a non-linux machine. This will be addressed in the future.

##### Dependencies

This project only requires `libc` and `Xlib`, which should be available on any Linux system running a Desktop Environment or Window Manager.

It also uses **Shared Memory (SHM)**, which should be included in `libc`.
Keep in mind however that behaviour for **SHM** may differ between `glibc` and `musl`, but the scope of this project shouldn't be affected.

Another thing to keep in mind is that this project uses **AVX and SIMD** through `immintrin.h`. `glibc` includes **AVX**, **SSE**, and other **SIMD** intrinsics, but `musl` does not.
These should still be available through **compiler headers** when using `gcc` or `clang`.

---

To build, clone this repository and run

```bash
cd Sharp
cmake -B build
cd build
make
```

This builds a shared library (`libSharp.so`) in the build directory.

Currently, this project does _NOT_ install the library or headers to the user's system. Due to lack of completeness, I feel it's best not to yet. To use this in a project, either copy the headers and library to a project and/or specify the header in library location in a `CMakeLists.txt`, `Makefile`, or as part of your compiler's command-line arguments.

## Using Sharp

Sharp is meant to be as simple as possible for the user to interact with. This may change as the feature set grows, but for now it's very simple to get a context running and rendered to.

- The only required header to include is `Sharp.h`
- Windows are created as a `SharpWindow` pointer.
- Surfaces (rendering targets) are created as `SharpSurface` pointers.
- Shapes are/will be created as variables of their given type.
  - Currently, only `SharpRect` exists, which is a struct that contains an x, y, width, and height.
- Drawing is done to a surface, and is shown by blitting it to the window surface.
  - Drawing can be (currently can ONLY be) done to the window surface directly
- Event polling is done through a `SharpEvent`.

#### Example Program

```C
#include <Sharp.h>

int main() {
    SharpWindow* window = SharpCreateWindow(800, 600, "title");
    SharpSurface* surface = SharpGetWindowSurface(window);

    SharpRect rect = {100, 100, 40, 40};

    int running = 1;

    SharpEvent event;
    while (running) {
       SharpPollEvents(window, &event);
       switch (event.type) {
            case SharpKeyPress:
                switch (e.keyPressed) {
                    case SK_ESC:
                        running = 0;
                        break;
                    default:
                        break;
                }
                break;
            default:
                break;
        }

    // Colors are in the format ARGB.
    SharpClearSurfaceColor(surface, 0xff000000); // Clear the surface color to Black.

    SharpFillRect(surface, &rect, 0xff0000ff);   // Fill the rect with Green.

    SharpUpdateWindowSurface(surface);           // Puts the image created on the surface to
                                                 // the window.
    }
}
```

This will create a green rectangle at `(100, 100)` of size `40x40`.
