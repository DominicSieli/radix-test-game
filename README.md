# Radix

**(WIP)** — This is an example project to test the Radix game library. It is a work-in-progress personal project and is not intended to be a finished product.

This project was built and tested on **Arch Linux**.

## Dependencies

* SDL3
* SDL3_image
* SDL3_mixer
* SDL3_ttf
* clang-tidy

## How to Build

Clone the repository:

```bash
git clone https://github.com/DominicSieli/radix-test-game.git
```

Build the project and link it to the Radix library archive using either the debug or optimized configuration:

```bash
make link_lib_debug
```

or:

```bash
make link_lib_optimized
```

Build the Radix library along with the project using either the debug or optimized configuration:

```bash
make build_debug
```

or:

```bash
make build_optimized
```

## Utilities

Check for warnings and errors

```bash
make check
```

Clean project directory

```bash
make clean
```
