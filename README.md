# CLibs

An union of some of C utils library.

## Documentation

A Doxyfile is provided to generate a doxygen documentation.
The doxygen documentation is hosted on [github pages](https://ayaztub.github.io/CLibs/).

## Installation

```sh
git clone https://github.com/AyAztuB/CLibs.git && cd CLibs && cmake -B build && cmake --build build --target libayaztub && cmake --build build --target install
```

## Tests

```sh
git clone https://github.com/AyAztuB/CLibs.git && cd CLibs && cmake -B build && cmake --build build && ctest --test-dir build
```

## Libraries

### Core Utils

- Assert
- Debug
- Logger
- Util Attributes


## Usage

Some code example are provided in `example/`.
In general, you can use it after installation like so:

```c
#include <ayaztub.h>
// or a more specific header like <ayaztub/core_utils/logger.h>

// ...
```

and compile it with:
```sh
gcc your_code.c -layaztub <AND_LIB_DEPENDENCIES>
```

## Lib dependencies

The lib requires some dependencies:
- pthread
