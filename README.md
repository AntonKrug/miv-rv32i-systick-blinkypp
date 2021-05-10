# miv-rv32i-systick-blinky++
 
This example project is based on **miv-rv32i-systick-blinky** project with partially ported parts of HAL, GPIO driver and application to C++ language. The miv-rv32i-systick-blinky was chosen to give an easy way to compare between C++ and C. If this would be a standalone and exclusive C++ project, then it would require speculation to compare it to a C implementation. Building it upon an existing C project allows quickly to switch between configurations and do comparisons.
 
Read the miv-rv32i-systick-blinky's for generic and target information (this readme focuses on the C vs C++ difference only).
 
 Recommending to use it with SoftConsole 6.5 or SoftConsole 2021.1. 
 
# Features
 
 Example project was made for showcasing some C++11, C++14 and C++17 features while on purpose avoiding some concepts such OOP to highlight that C++ can be written in a similar style to C, yet provide many advantages. 
 
## C++11 features
 
 - Namespaces https://www.cplusplus.com/doc/oldtutorial/namespaces/ 
 - Uniform Initialization https://digestcpp.com/cpp11/uniform/ https://docs.microsoft.com/en-us/cpp/cpp/initializers?view=msvc-160
 - Constructors and member initializer lists https://en.cppreference.com/w/cpp/language/constructor https://docs.microsoft.com/en-us/cpp/cpp/constructors-cpp?view=msvc-160
 - ``enum class`` https://en.cppreference.com/w/cpp/language/enum
 - ``auto`` https://en.cppreference.com/w/cpp/language/auto
 - ``static_assert`` https://en.cppreference.com/w/cpp/language/static_assert
 - ``static_cast`` https://en.cppreference.com/w/cpp/language/static_cast
 - ``constexpr`` https://en.cppreference.com/w/cpp/language/constexpr
 
## C++14 features
 
 - Binary literal https://en.cppreference.com/w/cpp/language/integer_literal
 - Digit separator https://en.wikipedia.org/wiki/C%2B%2B14#Digit_separators
 
## C++17 feature

 - Non-type template arguments https://en.cppreference.com/w/cpp/language/template_parameters

## Design pattern

Factory design pattern was used to restrict how *instance_s* structure can be created. Because *make_instance* does extra sanity checks, it was desirable to forbid any other way of making the structures. This can be achieved with a factory pattern. 

http://www.vishalchovatiya.com/factory-design-pattern-in-modern-cpp/

Doing these extra checkes are not possible from constructor as constructors can't be used directly with templates, while template arguments are essential for the static_assert.


## Disabled features
Despite the majority of C++ features being safe to use without a overhead (when converting project from C). Few features will cause overhead, in an embedded environment it's good to disable them and only enabled them when it's understood that they are a must-have.

- ```-fno-exceptions``` https://gcc.gnu.org/onlinedocs/libstdc++/manual/using_exceptions.html
- ```-fno-rtti``` https://gcc.gnu.org/onlinedocs/gcc/C_002b_002b-Dialect-Options.html
- ```-fno-use-cxa-atexit``` Search for -fuse-cxa-atexit https://gcc.gnu.org/onlinedocs/gcc/C_002b_002b-Dialect-Options.html
- ```-fno-threadsafe-statics``` https://gcc.gnu.org/onlinedocs/gcc/C_002b_002b-Dialect-Options.html
  
# Configurations

 The project contains both main.c and main.cpp implementations, the main.c depends fully on the original HAL, while main.cpp only partially as some HAL/Driver calls were replaced with their C++ implementation.
 The main.c and main.cpp files will get selectively excluded from the build depending on what configuration is selected.  
 
## C 
 The miv-rv32ima-Debug (-O0) and miv-rv32ima-Release (-Os) configurations are as original C implementation.
 
## C++ 
 The miv-rv32ima-cpp-O0-Debug, miv-rv32ima-cpp-O1, miv-rv32ima-cpp-O2, miv-rv32ima-cpp-O3, miv-rv32ima-cpp-Ofast and miv-rv32ima-cpp-Os configurations are C++ implementation.
 
# Size and performance

Short story: blinky++ is significantly smaller and faster compared to the original.

Long story:

| C++ Configuration         | ELF 'text' size (bytes) |
| ------------------------- | ----------------------- |
| miv-rv32ima-cpp-O0-Debug  | 5696                    |
| miv-rv32ima-cpp-O1        | 3200                    |
| miv-rv32ima-cpp-O2        | 3200                    |
| miv-rv32ima-cpp-O3        | 3392                    |
| miv-rv32ima-cpp-Ofast     | 3392                    |
| miv-rv32ima-cpp-Os        | 3136                    |

| C Configuration           | ELF 'text' size (bytes) |
| ------------------------- | ----------------------- |
| miv-rv32ima-Debug (-O0)   | 6256                    |
| miv-rv32ima-Release (-Os) | 3920                    |
 
 Comparing the same -Os builds (miv-rv32ima-cpp-Os vs miv-rv32ima-Release) shows that using the C++ features the footprint shrink by 784 bytes (20% of the C size). However the C++ has still some leftovers of the original C project, so it could be even smaller if applied fully to the whole project. When comparing just the necessary parts for the blinky to work, driver/HAL/application then the ~700 bytes (in C) shrinks to ~100 bytes (in C++).

Performance is significantly higher with C++ because the writes to peripheral registers are inlined and executed as writes to the memory-mapped registers, while C had to create calls to generic implementation with a lot of branching. Because GPIO is such a simple peripheral the inlining of the implementation is smaller than creating the calls for generic functions. 

Configuring the GPIO, setting output on ports and executing the blinky is just 23 instructions, there are no jump/branches except the infinite loop in the blinky itself and the calls to legacy parts of HAL (which was not ported to C++).

Assembly of the *main* function in the main.cpp
 ```asm
8000078c:   ff010113            addi    sp,sp,-16
80000790:   00112623            sw  ra,12(sp)
80000794:   700057b7            lui a5,0x70005
80000798:   00078023            sb  zero,0(a5) # 70005000 <RAM_SIZE+0x70001000>
8000079c:   00078223            sb  zero,4(a5)
800007a0:   fff00713            li  a4,-1
800007a4:   08e7a023            sw  a4,128(a5)
800007a8:   00500713            li  a4,5
800007ac:   00e7a023            sw  a4,0(a5)
800007b0:   00e7a223            sw  a4,4(a5)
800007b4:   0a07a023            sw  zero,160(a5)
800007b8:   fb9ff0ef            jal ra,80000770 <HAL_enable_interrupts>
800007bc:   02faf537            lui a0,0x2faf
800007c0:   08050513            addi    a0,a0,128 # 2faf080 <RAM_SIZE+0x2fab080>
800007c4:   c99ff0ef            jal ra,8000045c <MRV_systick_config>
800007c8:   00000717            auipc   a4,0x0
800007cc:   0b070713            addi    a4,a4,176 # 80000878 <main::val>
800007d0:   700056b7            lui a3,0x70005
800007d4:   00072783            lw  a5,0(a4)
800007d8:   00f7c793            xori    a5,a5,15
800007dc:   00f72023            sw  a5,0(a4)
800007e0:   0af6a023            sw  a5,160(a3) # 700050a0 <RAM_SIZE+0x700010a0>
800007e4:   ff1ff06f            j   800007d4 <main+0x48>
``` 

Thanks to constexpr and templates which in newer C++ can take non-type arguments now most of the implementation is done at compile time. Moving 'logic' to compile-time is surprisingly viable because the peripheral addresses are given at the synthesis of the design and do not need to be changed on runtime. When needing multiple instances to be muxed, then all of them can be instantiated, put into a lookup table on compile-time, and on runtime just switch between indexes in a lookup table.

Algorithms are possible in compile-time as well (with recursion too). Instantiating new templates in a *for loops* is possible when loops are rewritten as recursion, often functional programming can be applied to a problem and suddenly many things are possible at compile-time.

In contrast to consteval (which is guaranteed to be compile-time), the constexpr can be both, making functions that can get optimized away whenever it's feasible, but still allow run-time implementation whenever compile-time is not possible.

# Safety

```enum class``` are safer (strongly typed, no implicit casting done behind user's back)

Underlying types are checked for overflows, setting large value into a smaller type will notify the user
```c++
enum class irqClearing_e: int32_t {
    clearAll32 = 0xFFFFFFFF,
    clearAll16 = 0xFFFF,
    clearAll8  = 0xFF
};
```

```
../main.cpp:121:22: error: enumerator value '4294967295' is outside the range of underlying type 'int32_t' {aka 'long int'}
         clearAll32 = 0xFFFFFFFF,
                      ^~~~~~~~~~
```

And checked for conversions from other types:
```C++
enum class irqClearing_e: uint32_t {
    clearAll32 = 1.0f,
    clearAll16 = 0xFFFF,
    clearAll8  = 0xFF
};
```

```
../main.cpp:121:22: error: narrowing conversion of '1.0e+0f' from 'float' to 'uint32_t' {aka 'long unsigned int'} inside { } [-Wnarrowing]
         clearAll32 = 1.0f,
                      ^~~~
```

Duplicate names in the enums are possible without creating conflicts, because they are properly scoped:

```C++
enum class outOffset_e: uint32_t {
    offset0 = 0xA0,
    offset1 = 0xA4,
    offset2 = 0xA8,
    offset3 = 0xAC
};

enum class irqOffset_e: uint32_t {
    offset0 = 0x80,
    offset1 = 0x84,
    offset2 = 0x88,
    offset3 = 0x8C
};
```
Yet they provide type safety  even when they are using the same underlying type. Mixing different enum types by accident without explicit casting is not possible:

```C++
if (GPIO::irqOffset_e::offset0 == GPIO::outOffset_e::offset0) {
    GPIO::set<&gpioOut>(0u);
}
```

```
../main.cpp:257:36: error: no match for 'operator==' (operand types are 'GPIO::irqOffset_e' and 'GPIO::outOffset_e')
     if (GPIO::irqOffset_e::offset0 == GPIO::outOffset_e::offset0) {
         ~~~~~~~~~~~~~~~~~~~~~~~~~~~^~~~~~~
```

```static_assert``` are done at compile time and doing a lot of tests will not cause any overhead. Just some arbitary asserts here:

```C++
template<uint32_t baseAddress, apbBusWidth_e apbWidth, uint32_t numberOfIOs>
constexpr static instance_s makeInstance() {
    // Check for valid base address
    static_assert((baseAddress % 4) == 0, 
        "The GPIO base port address needs to be 32-bit aligned");
    static_assert((baseAddress >= 0x7000'0000UL) && (baseAddress < 0x8000'2000UL),
        "The GPIO has to be within the APB bus range");

    // Check for correct bus width
    static_assert(apbWidth != apbBusWidth_e::unknownWidth,
        "The APB bus width has to be 8-bit, 16-bit or 32-bit");

    // Check for valid number of IOs
    static_assert(numberOfIOs>0 && numberOfIOs<=32, 
        "The number of IOs in this GPIO port_s needs to be between 1-32");

    return {baseAddress, apbWidth, numberOfIOs};
}
```

Then using functions with plenty of sanity checks and asserts will make harder for users to introduce accidental bugs:
```C++
constexpr auto gpioOut = GPIO::instance_s::makeInstance<0x7000'5002UL, GPIO::apbBusWidth_e::bits32, 2>();
```

```
../main.cpp:170:41: error: static assertion failed: The GPIO base port address needs to be 32-bit aligned
```                       

Providing different enums types or literals by accident (even when they have exact same value as the valid enum) will fail as well:

Different enum:
```C++
constexpr auto gpioOut = GPIO::instance_s::makeInstance<0x7000'5000UL, GPIO::irqOffset_e::offset0, 2>();
```

```
../main.cpp:248:91: error: could not convert template argument 'offset0' from 'GPIO::irqOffset_e' to 'GPIO::apbBusWidth_e'
 constexpr auto gpioOut = GPIO::instance_s::makeInstance<0x7000'5000UL, GPIO::irqOffset_e::offset0, 2>();
                                                                                           ^
 ```

Literal number:
```C++
constexpr auto gpioOut = GPIO::instance_s::makeInstance<0x7000'5000UL, 1, 2>();
````

 ```
 ../main.cpp:248:66: error: could not convert template argument '1' from 'int' to 'GPIO::apbBusWidth_e'
 constexpr auto gpioOut = GPIO::instance_s::makeInstance<0x7000'5000UL, 1, 2>();
                                                                  ^
 ```

 It's not possible to change the values of the *instance_s* after it was instanciated as constexpr (as it's just read only). And it's not possible to create the *instance_s* without the use of the GPIO::instance_s::makeInstance function and not possible to circumvent all the asserts/checks.

Trying to make the structure directly without using its factory function:
```c++
constexpr auto rogueStructure = GPIO::instance_s{0x7000'5000UL, GPIO::apbBusWidth_e::bits32, 2};
```
Will fail to complie:
```
../main.cpp:267:95: error: 'constexpr GPIO::instance_s::instance_s(uint32_t, GPIO::apbBusWidth_e, uint32_t)' is private within this context
```

 If the enum needs to be converted to a number, then it needs to be explicitly cast:
 ```C++
HAL::set32bitReg(thisGpio->baseAddress, static_cast<uint32_t>(outOffset_e::offset0), value);
 ```

 Overall avoiding #define macros, replacing them with enums or constexpr functions, editors and tools understand these much better than depending on some search&replace done in the pre-processor. Allowing to write more explicit, safer and easier to understand code, no implicit casting is done behind the user's back.

# Redability and ease of use

Everything is in their own structure, enum and namespace, no freestanding #defines, no implied functionality.

Properly organized code is easier to work with, because IDEs, understand it fully, instead of having 100s of #defines which could be function/code, enum value or just some constant. Autocomplete is more useful as it lists less, but more relevant information which is correct (instead of listing everything with the given GPIO_ prefix). Refactoring works correctly as well, overall requiring less time to use the code.

Using ``auto`` sometimes improves readability/portability if it's not overused.

With ``constexpr`` functional macros can be replaced with proper functions, they are easier to read and can't contain some side effects. For example:
```
#define ADD (a, b) (a) + (b)
```
Forgetting to wrap each argument in () might work in the tests, yet sometimes might not work as expected. What types can be used with it is and what it will return is not explicit as well. Coding standards discourage using function like macros which could produce code: https://analyst.phyzdev.net/documentation/help/reference/misra.define.func.htm and some strict standards completely ban their use. A similar example with ```constexpr```:

```C++
constexpr int add(int a, int b) {
    return a + b;
}

constexpr int five = add(2, 3);
```

If it's given a compile-time known value, it will be evaluated at compile-time, if it's given runtime variable it will be just as a regular function.

Implementations for different types (similarly to the macro) can be achieved with templates:

```C++
template <typename TYPE>
constexpr TYPE add(TYPE a, TYPE b) {
    return a + b;
}

constexpr auto five = add(2, 3);
constexpr auto six  = add<long>(3, 3);
```

The use of the ```add``` can stay the same when the type can be inferred or can be explicitly specified to make sure it's the desired type.



# Conclusion

Of course, there are some constraints and caveats. However, the C++ is not slow/bloated/heavy as common myths state and on the contrary, can be very efficient. When carefully deciding what features are used, then the application can be much smaller and faster than C.

While providing safer and easier to use/read code.
