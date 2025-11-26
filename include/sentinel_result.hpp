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
#include <cstdint>
#include <functional>

namespace cinter
{

/*
Near-zero-overhead (if not zero overhead) wrapper class
that helps make writing code using OS functions that use
special sentinel values for errors.

For example:
    SentinelHResult hr = SomeWin32Function();  // See example declaration of SentinelHResult below.
    if (hr.has_error())
    {
        LogError("Operation failure: '{}'", GetLastError(hr.value()));
    }

Example sentinel_result-based types:
    using SentinelHResult = sentinel_result<HRESULT>;
    using XenomaiResult   = sentinel_result<int, 0, std::less<int>>;
    using StringResult    = sentinel_result<const char*, nullptr, std::not_equal_to<const char*>>;  // Consider using cinter::safe_string though
*/
template <typename T, T sentinel = static_cast<T>(0), typename SuccessComp = std::equal_to<T>>
class sentinel_result
{
    T value_;

public:
    using value_type = T;

    // Implicit conversion is desired
    sentinel_result(const T value)
        : value_(value)
    {}

    sentinel_result(const sentinel_result& other) noexcept = default;
    sentinel_result(sentinel_result&& other) noexcept = default;

    ~sentinel_result() noexcept = default;

    sentinel_result& operator=(const sentinel_result& other) noexcept = default;
    sentinel_result& operator=(sentinel_result&& other) noexcept = default;

    value_type value() const
    {
        return value_;
    }

    bool is_ok() const
    {
        return SuccessComp{}(value_, sentinel);
    }

    bool has_error() const
    {
        return !is_ok();
    }
};

} // namespace cinter
