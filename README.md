# MAL


## Introduction
This is a Lisp interpreter following the [Make-A-Lisp](https://github.com/kanaka/mal/tree/master) challenge. It supports functions, closures, recursion,
conditionals, arrays, hashmaps, strings, and more. The end-goal is to have it to be self-hosting.

## Quick Start

1. Clone the repo

`git clone https://github.com/kanaka/mal.git`

2. The run the following commands to build the system:  

```
$ mkdir build
$ cd build
$ cmake .. 
$ make
```

3. Run the executable
```
 ./MAL
```


## Usage

```
user> (+ 2 2)
4
```

```
user> (def! add (fn* (a b) (+ a b)))
user> (add 3 4)
7
```

## TODO
* Tail-call optimization
* File loading
* Quoting
* Macros

## Project Status
The status of this project is: _In Progress_