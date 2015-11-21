jsmn++
======

jsmn++ is a tiny json parser embedded in your C++ project for configuration. The Tokenizer is from [jsmn][jsmn].

### Embedded in your C++ project

If you are using [CMake][cmake], simply use `include(jsmnpp/libjsmn.cmake)`. If not, add `jsmn.h`, `jsmn.c`, `jsmn.hpp`, `jsmn.cpp` files to your build system.

### Usage

```c++
#include <jsmn.hpp>
using namespace std;

// parse
Object o = parse("/path/to/config.json");

// get number
double num = o["num"].unwrap<Number>();
// get boolean
bool b = o["bool"].unwrap<Boolean>();
// get string
string s = o["str"].unwrap<String>();
// get array
Array a = o["array"].unwrap<Array>();
// get object
Object obj = o["object"].unwrap<Object>();

// serialize objcet
string json_string = serialize(o);

```

### License

BSD 3-Clause


[jsmn]: https://github.com/zserge/jsmn
[cmake]: https://cmake.org/
