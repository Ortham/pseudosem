Pseudosem
=========

Pseudosem is a header-only version string comparison library, written in C++11, that conforms to the [Semantic Versioning](http://semver.org/) specification, and also supports extended version syntaxes:

* Leading zeroes (eg. `0.05.1`)
* Arbitrary number of release version parts (eg. `1.2.3.4.5`)
* Space, colon, hyphen and underscore prerelease separators (eg. `1.0.0 alpha:1-2_3`)
* Case insensitivity

See Pseudosem's test suite for a full set of examples.

## Usage

Example:

```
#include <pseudosem.h>
#include <iostream>

int main() {
    std::string v1("1.0.0");
    std::string v2("2.4.0");

    int result = pseudosem::compare(v1, v2);

    if (result < 0)
        std::cout << "v1 is an earlier version than v2";
    else if (result == 0)
        std::cout << "v1 is an equivalent version to v2";
    else
        std::cout << "v1 is a later version than v2";

    return 0;
}
```

## Tests

Pseudosem has a test suite built on [Google Test](https://github.com/google/googletest), and uses [CMake](http://www.cmake.org/) to support cross-platform building. From the Pseudosem directory root:

```
mkdir build
cd build
cmake ..
./tests
```

If CMake can't find Google Test, set its root directory by passing `GTEST_ROOT`:

```
cmake .. -DGTEST_ROOT=<path>
```
