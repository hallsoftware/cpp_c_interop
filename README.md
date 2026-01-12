# cinter - A C++ Interop Library for C APIs

Utility classes to make working with C APIs safer in C++

## Table of Contents

- [Overview](#overview)
- [Design Philosophy](#design-philosophy)
- [Features](#features)
- [API Documentation](#api-documentation)
  - [safe_string](#safe_string)
  - [sentinel_result](#sentinel_result)
- [Examples](#examples)
  - [Using safe_string](#using-safe_string)
  - [Using sentinel_result](#using-sentinel_result)
- [License](#license)
- [Contributing](#contributing)

## Overview

`cpp_c_interop` is a lightweight, header-only C++ library that provides zero-overhead (or near-zero-overhead) wrapper classes to simplify and improve safety when working with C APIs. The library addresses common pain points when interfacing with C code, such as handling null pointers in strings and dealing with sentinel-based error codes.

The library consists of two main components:

- **`safe_string`**: A wrapper for C-style strings that safely handles null pointers by treating them as empty strings
- **`sentinel_result`**: A template class for wrapping return values that use sentinel values to indicate errors (e.g., `-1`, `NULL`, `INVALID_HANDLE_VALUE`)

## Design Philosophy

The library is built around several core principles:

1. **Zero-overhead abstraction**: The wrapper classes are designed to compile down to the same machine code as raw C API calls in optimized builds
2. **Type safety**: Leverage C++ type system to catch errors at compile-time when possible
3. **Explicit error handling**: Make error states visible and easy to check without exceptions
4. **Header-only**: No linking required, just include and use
5. **Modern C++ idioms**: Uses `constexpr`, `[[nodiscard]]`, and other modern C++ features while maintaining simplicity
6. **Non-invasive**: Works alongside existing C code without requiring refactoring

## Features

- ✅ Header-only library
- ✅ Zero runtime overhead in release builds
- ✅ `constexpr` support for compile-time operations
- ✅ `safe_string` treats null as empty, matching many C library conventions
- ✅ Only uses the standard library with no external dependencies
- ✅ Full character type support (`char`, `wchar_t`, `char8_t`, `char16_t`, `char32_t`)
- ✅ Compatible with standard library types (`std::string`, `std::string_view`)
- ✅ Range-based for loop support
- ✅ BSD 3-Clause License

## API Documentation

### safe_string

The `basic_safe_string<Char>` class template wraps a pointer to a character array (C-string) and treats null pointers as empty strings.

#### Template Parameters

- `Char`: Character type (default: `char`)

#### Type Aliases

basic_safe_string has the following type aliases:

- `const_iterator` = Pointer to constant character type

#### Member Functions

- `constexpr bool is_null() const`: Returns `true` if the wrapped pointer is null
- `explicit constexpr operator bool() const`: Returns `true` if the wrapped pointer is not null
- `constexpr const Char* c_str() const`: Returns the wrapped pointer, or an empty string if null
- `std::basic_string<Char> string() const`: Converts to `std::string`
- `constexpr std::basic_string_view<Char> view() const`: Returns a `std::string_view`
- `constexpr const_iterator begin() const`: Iterator support for range-based loops
- `constexpr const_iterator end() const`: Iterator support for range-based loops

#### Comparison Operators

All comparison operators (`==`, `!=`, `<`, `<=`, `>`, `>=`) are supported and compare the string contents.

#### Common Type Aliases

The following `basic_safe_string` aliases are provided for common character types:

- `safe_string`    = `basic_safe_string<char>`
- `safe_wstring`   = `basic_safe_string<wchar_t>`
- `safe_u8string`  = `basic_safe_string<char8_t>`
- `safe_u16string` = `basic_safe_string<char16_t>`
- `safe_u32string` = `basic_safe_string<char32_t>`

### sentinel_result

The `sentinel_result<T, sentinel, SuccessComp>` class template wraps return values from C APIs that use special sentinel values to indicate errors.

#### Template Parameters

- `T`: The value type returned by the C API
- `sentinel`: The sentinel value that indicates success or failure (default: `0`)
- `SuccessComp`: Comparator to determine success (default: `std::equal_to<T>`)

#### Member Functions

- `value_type value() const`: Returns the wrapped value
- `bool has_error() const`: Returns `true` if the value indicates an error
- `explicit operator bool() const`: Returns `true` if the operation succeeded
- `operator value_type() const`: Implicit conversion to the underlying type

## Examples

### Using safe_string

```
#include <iostream>
#include "cinter.hpp" 

extern "C" const char* get_user_name();  // May return nullptr

int main()
{
    // name is safe to use even if get_user_name() returns nullptr.
    cinter::safe_string name = get_user_name();
    std::cout << "User: " << name.c_str() << std::endl;

    // Check if the pointer was null
    if (name.is_null()) {
        std::cerr << "Warning: No user name available\n";
    }

    // Convert to std::string
    std::string cpp_name = name.string();

    // Use with string_view
    auto view = name.view();

    // Range-based for loop
    for (char c : name) {
        std::cout << c;
    }

    return 0;
}
```

### Using sentinel_result

```
#include "cinter.hpp"
#include <iostream>
#include <windows.h>

// Default value of zero (S_OK) is the success value.
using SentinelHResult = sentinel_result<HRESULT>;

int main()
{
    SentinelHResult hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
    if (hr.has_error()) {
        std::cerr << "COM initialization failed: 0x" 
                  << std::hex << hr.value() << std::endl;
        return 1;
    }

    // Implicit conversion to bool
    if (hr) {
        std::cout << "COM initialized successfully\n";
    }

    CoUninitialize();
    return 0;
}
```

## License

This project is licensed under the BSD 3-Clause License. See the license text in the source files for details.

Copyright (c) 2025, Kevin Hall. All rights reserved.

## Contributing

Contributions are welcome! Whether you're fixing bugs, improving documentation, or proposing new features, your help is appreciated.

### How to Contribute

1. **Report Issues**: Found a bug or have a suggestion? Open an issue on GitHub
2. **Submit Pull Requests**: Fork the repository, make your changes, and submit a PR
3. **Improve Documentation**: Help make the documentation clearer and more comprehensive
4. **Share Use Cases**: Let us know how you're using the library

### Guidelines

- Maintain the zero-overhead philosophy
- Keep the code header-only
- Ensure compatibility with C++17 and later
- Add tests for new features
- Follow the existing code style (see source files)
- Update documentation for API changes

We look forward to your contributions!
