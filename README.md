# Things may still change
# Very untested as of now

# c-vector
Compile Time Generic Dynamic Lookup Table in C.

## First Things First
On it's own, no code is being compiled. You first have to include and implement a lookup table of your desires. For that, there are two macros.
```c
#include "vec.h"
LUTD_INCLUDE(N, A, T, M);
LUTD_IMPLEMENT(N, A, T, M, H, C, F);
```
1. `N` - **N**ame - the resulting name of the vector struct
2. `A` - **A**bbreviation - functions get prefixed with that
3. `T` - **T**ype - the type your elements within the vector
4. `M` - **M**ode - storage type, either `BY_VAL` (by value) or `BY_REF` (by reference)
4. `H` - **H**ash - provide a hashing function for your elements (required)
4. `C` - **C**ompare - provide a comparing function for your elements - if none provided, default to `memcmp`
5. `F` - **F**ree - provide a freeing function for your elements, if available
 
## Examples
- [example int](examples/example_int.c)
- [example person](examples/example_person.c)
- I also used this lookup table in my [Timers interpreter](https://github.com/rphii/TimersInterpreter)

## Design Considerations
- Both `BY_VAL` and `BY_REF` can be used on either basic types or (complex) structs.
- Generally speaking, I'd use the latter when dealing with structs where `sizeof struct > sizeof(void *)`.
- I went ahead and compared the very basic performance between the two approaches [here](https://github.com/rphii/vec_test).

## Metaprogramming
### Advantages
- Compiler optimized code
- Flexibility, reusability (generics / templates)
- Type safety
### Disadvantages
- Maintenance challenges
- Debugging difficulties
- Compilation overhead

## How to use it

### Available Functions
The `A##` means the `A` specified in the two macros.
- `A##_init` initialize lookup table
- `A##_add` add a value
- `A##_add_count` add a specified number of counts to a value
- `A##_del` delete a value
- `A##_has` check if a value exists
- `A##find` find position `i` and `j` of a value (to access in lookup table: `N.buckets[i].items[j]`)
- `A##_free` free lookup table
- `A##_clearn` clear lookup table (reset but don't free)
- `A##_dump` dump everything unsorted (`arr` is required, `counts` is optional)

# todo so I don't forget
- `A##_join` and `A##_join_threaded` ...
