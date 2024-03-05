# ctfp (Compile-Time Find-Pattern)

ctfp is a small, header-only C++ library that allows you to build signatures at compile-time.

### Example Usage

```cpp
#include <ctfp.hpp>
#include <vector>

int main() {
    const auto result = ctfp::find<"aabbccdd?">(haystack);
    // Do something...
    return 0;
}
```

### Building

ctfp requires a C++23-compatible compiler.

```cmake
target_link_libraries(my_project PRIVATE ctfp::ctfp)
```
## Format

The pattern format is rather straightforward: each byte in the pattern string must always match, whereas a wildcard (represented by a single '?') matches any byte.

For example, the pattern `"aabbccdd?"` states that the following must be true:
The first byte is `0xAA`,
The second byte is `0xBB`,
The third byte is `0xCC`,
The fourth byte is `0xDD`,
The fifth byte may be anything.
