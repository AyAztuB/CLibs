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
git clone https://github.com/AyAztuB/CLibs.git && cd CLibs && cmake -B build -DBUILD_TESTS=ON && cmake --build build --target tests && ctest --test-dir build
```

To get the unit test coverage report, you can use gcovr in the build directory as follow:
```sh
cd build && gcovr -r .. . --html-details coverage.html && firefox coverage.html
```

## Libraries

### Utils

- Assert
- Debug
- Util Attributes
- Util Macros

### Logger

A complete (and thread-safe) logging API designed to be *possibly* portable (event if not the case yet).


## Usage

Some code example are provided in `example/`.
In general, you can use it after installation like so:

```c
#include <ayaztub.h>
// or a more specific header like <ayaztub/logger.h>

// ...
```

and compile it with:
```sh
gcc your_code.c -layaztub <AND_LIB_DEPENDENCIES>
```

## Lib dependencies

The lib requires some dependencies:
- pthread (windows is not yet supported...)
