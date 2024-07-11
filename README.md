# CLibs

An union of some of C utils library.

## Instalation

```sh
git clone https://github.com/AyAztuB/CLibs.git && cd CLibs && cmake -B build && cmake --build build --target libayaztub && cmake --build build --target install
```

## Libraries

### Core Utils

- Logger


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
gcc your_code.c -layaztub
```
