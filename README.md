Mini_JVM
=============
A Mini Java Virtual Machine.

### Futures

- class parser
- interpreter
- gc
- jit

### Build

1. Requirements
    * macOS, Linux(untested) or Windows(untested)
    * JDK (OpenJDK or OracleJDK) (>= 8)
    * CMake (>= 3.5)

2. Build instructions
    * Clone this repo.
    * `cd` into your directory that contains Mini_JVM source code.
    * Type `mkdir build && cd build && cmake .. && make` in your terminal app.
    * Enjoy it!

### Usage

#### javap
```bash
Usage: 
        mini_javap <class-name>
```
#### java
```bash
Usage: 
        mini_java <class-name> [<args>]...

Options:
        <class-name>        name of the class to run
```

### Credit
* Inspired by [KiVM](https://github.com/imkiva/KiVM), [JJVM](https://github.com/caoym/jjvm), [YVM](https://github.com/kelthuzadx/yvm)
* Logger using [SPDLOG](https://github.com/google/googletest)
* Test using [GTEST](https://github.com/gabime/spdlog)

### See Also

[The Java® Virtual Machine Specification](https://docs.oracle.com/javase/specs/jvms/se14/html/)

[HotSpot Virtual Machine Garbage Collection Tuning Guide](https://docs.oracle.com/en/java/javase/14/gctuning/preface.html)