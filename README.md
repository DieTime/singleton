<p align="center">
    <img src="assets/icon.png" width="400px" />
</p>

<p align="center">
    <img src="https://github.com/DieTime/singleton/actions/workflows/linux.yml/badge.svg" alt="Health Check Linux" />
    <img src="https://github.com/DieTime/singleton/actions/workflows/macos.yml/badge.svg" alt="Health Check MacOS" />
    <img src="https://github.com/DieTime/singleton/actions/workflows/windows-mingw.yml/badge.svg" alt="Health Check Windows MinGW" />
    <img src="https://github.com/DieTime/singleton/actions/workflows/windows-msvc.yml/badge.svg" alt="Health Check Windows MSVC" />
    <img src="https://img.shields.io/github/license/DieTime/singleton?color=%231cc727" alt="License" />
    <img src="https://img.shields.io/badge/supports-single%20header-green?color=%231cc727" alt="Supports" />
</p>

Library for creating easily mockable C++ singleton classes.

# Usage

For multithreaded programs, it is recommended to use thread-safe version of the singleton.

```cpp
#include <singleton>

struct ThreadSafe : singleton::threadsafe<ThreadSafe>
{
    void process() {...}
};

void foo()
{
    ThreadSafe::instance()->process();
}

struct ThreadSafeWithCtor : singleton::threadsafe<ThreadSafeWithCtor>
{
    ThreadSafeWithCtor(std::string) {...}
    void process() {...}
};

void bar()
{
    /*
     * If the class does not have a default constructor,
     * it is necessary to call the initialize() method
     * before the first use.
     */

    ThreadSafeWithCtor::initialize("tag");
    ThreadSafeWithCtor::instance()->process();
}
```

There is also a non thread-safe version of singleton without the overhead of thread synchronization.

```cpp
#include <singleton>

struct NonThreadSafe : singleton::nonthreadsafe<NonThreadSafe>
{
    void process() {...}
};

void foo()
{
    NonThreadSafe::instance()->process();
}

struct NonThreadSafeWithCtor : singleton::nonthreadsafe<NonThreadSafeWithCtor>
{
    NonThreadSafeWithCtor(std::string) {...}
    void process() {...}
};

void bar()
{
    /*
     * If the class does not have a default constructor,
     * it is necessary to call the initialize() method
     * before the first use.
     */

    NonThreadSafeWithCtor::initialize("tag");
    NonThreadSafeWithCtor::instance()->process();
}
```

For testing purposes, you can inherit a singleton from the interface and conveniently mock your singleton.

> For mocking you need to add the definition   `SINGLETON_MOCK_ENABLED=1` in your build system or before including the library.

```cpp
#define SINGLETON_MOCK_ENABLED 1
#include <singleton>

struct Interface {
    virtual ~Interface() = default;
    virtual void process() = 0;
}

struct Singleton : singleton::threadsafe<Singleton, Interface>
{
    void process() {...}
};

void usage()
{
    Singleton::instance()->process();
}

struct Mock : public Interface
{
    void process() {...}
};

void test()
{
    Singleton::mock<Mock>();
    Singleton::instance()->process();
}
```

# Install

You can use the [single-header](include/singleton.h) version of the library and just include it in your sources, also you can build and install the library on your computer and connect it using `pkgconfig`.

```bash
$ meson setup builddir
$ meson compile -C builddir
$ meson install -C builddir
```

Linking using `cmake`:

```cmake
cmake_minimum_required(VERSION 3.14)
project(example)

find_package(PkgConfig)
pkg_check_modules(Singleton REQUIRED IMPORTED_TARGET singleton)

add_executable(example ...)

target_link_libraries(example PUBLIC PkgConfig::Singleton)
```

Linking using `meson`:

```cpp
singleton = dependency('singleton')
```

# License

Copyright 2023 Denis Glazkov <glazzk.off@mail.ru>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

