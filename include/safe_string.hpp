/*
BSD 3-Clause License

Copyright (c) 2025, Kevin Hall

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its
   contributors may be used to endorse or promote products derived from
   this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#pragma once
#include <cstddef>
#include <type_traits>
#include <string>
#include <string_view>

namespace cinter
{

// basic_safe_string wraps a pointer to character array (a C-string)
// that treats null pointers as empty strings.  operator bool and
// a is_null() member function are available if one chooses to do
// error checking in the case the class is instaniated with a null pointer.
template <typename Char = char>
class basic_safe_string
{
    const Char* ptr;

    static constexpr const Char* empty_string()
    {
        if constexpr (std::is_same_v<Char, char>)
        {
            return "";
        }
        else if constexpr (std::is_same_v<Char, wchar_t>)
        {
            return L"";
        }
#ifdef __cpp_char8_t
        else if constexpr (std::is_same_v<Char, char8_t>)
        {
            return u8"";
        }
#endif
        else if constexpr (std::is_same_v<Char, char16_t>)
        {
            return u"";
        }
        else if constexpr (std::is_same_v<Char, char32_t>)
        {
            return U"";
        }
        else
        {
            return nullptr;
        }
    }

    // Used to implement end()
    constexpr std::size_t length()
    {
        const Char* s = c_str();
        std::size_t len = 0;
        while (s[len] != Char{})
        {
            ++len;
        }
        return len;
    }

public:
    constexpr basic_safe_string() noexcept : ptr(nullptr) {}
    // Implicit conversion is desired
    constexpr basic_safe_string(const Char* src) : ptr(src) {}

    constexpr basic_safe_string(const basic_safe_string& other) noexcept = default;
    constexpr basic_safe_string(basic_safe_string&& other) noexcept = default;

    ~basic_safe_string() noexcept = default;

    basic_safe_string& operator=(const basic_safe_string& other) noexcept = default;
    basic_safe_string& operator=(basic_safe_string&& other) noexcept = default;

    constexpr bool is_null() const {return !ptr;}
    constexpr operator bool() const {return ptr != nullptr;}
    constexpr const Char* c_str() const {return ptr ? ptr : empty_string();}
    constexpr operator const Char*() const {return c_str();}

    std::basic_string<Char> string() const {return c_str();}
    operator std::basic_string<Char>() const {return c_str();}

    constexpr std::basic_string_view<Char> view() const {return c_str();}
    constexpr operator std::basic_string_view<Char>() const {return view();}

#ifdef __cpp_lib_three_way_comparison
    constexpr auto operator<=>(const basic_safe_string& other) const noexcept
    {
        return view() <=> other.view(); 
    }
#else
    constexpr bool operator==(const basic_safe_string& other) const noexcept
    {
        return view() == other.view();
    }

    constexpr bool operator!=(const basic_safe_string& other) const noexcept
    {
        return !(*this == other);
    }

    constexpr bool operator<(const basic_safe_string& other) const noexcept
    {
        return view().compare(other.view()) < 0;
    }

    constexpr bool operator<=(const basic_safe_string& other) const noexcept
    {
        return view().compare(other.view()) <= 0;
    }

    constexpr bool operator>(const basic_safe_string& other) const noexcept
    {
        return view().compare(other.view()) > 0;
    }

    constexpr bool operator>=(const basic_safe_string& other) const noexcept
    {
        return view().compare(other.view()) >= 0;
    }
#endif // __cpp_lib_three_way_comparison

    using const_iterator = const Char*;

    constexpr const_iterator begin() const noexcept { return c_str(); }
    constexpr const_iterator end() const noexcept { return c_str() + length(); }
    constexpr const_iterator cbegin() const noexcept { return begin(); }
    constexpr const_iterator cend() const noexcept { return end(); }    

    // While a constexpr length(), size(), and operator[] could be constructed, they
    // would only be usable from consteval'ed C++ code.  This class is intended to
    // only wrap C code.  Therefore these member function definitions are private or
    // deleted.
    constexpr std::size_t size() const = delete;
    constexpr const Char& operator[](std::size_t pos) const = delete;
};

using safe_string   = basic_safe_string<char>;
using safe_wstring  = basic_safe_string<wchar_t>;
#ifdef __cpp_char8_t
using safe_u8string = basic_safe_string<char8_t>;
#endif
using safe_u16string = basic_safe_string<char16_t>;
using safe_u32string = basic_safe_string<char32_t>;

} // namespace cinter
