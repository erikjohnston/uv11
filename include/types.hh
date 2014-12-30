#pragma once

#include "uv.h"

#include <utility>
#include <type_traits>
#include <system_error>


namespace uvpp {
    using ::uv_handle_t;
    using ::uv_stream_t;
    using ::uv_req_t;

    using Error = std::error_code;

    template<typename T>
    class WrappedObject {
    public:
        T& Get() { return t; }
        T const& Get() const { return t; }

    protected:
        template<typename... Args>
        WrappedObject(Args&&... args) : t(std::forward<Args>(args)...) {}

    private:
        T t;
    };

    template<typename T, typename _=void> struct is_handle : std::false_type {};
    template<typename T, typename _=void> struct is_stream : std::false_type {};
    template<typename T, typename _=void> struct is_request : std::false_type {};

    template<typename From, typename To> struct is_handle_castable : std::false_type {};

    template<typename To, typename From>
    auto handle_cast(From& from)
    -> typename std::enable_if<is_handle_castable<From&, To&>::value, To&>::type {
        return reinterpret_cast<To&>(from.Get());
    }

    template<typename To, typename From>
    auto handle_cast(From const& from)
    -> typename std::enable_if<is_handle_castable<From const&, To const&>::value, To const&>::type {
        return reinterpret_cast<To const&>(from.Get());
    }

    // Helpers
    template<bool v> struct is_true : std::integral_constant<bool, v> {};
    template<typename A, typename B> struct Or : is_true<A::value || B::value> {};
    template<typename T> struct Void { using Type = void; };

    // Specializations
    template<typename T> struct is_handle<T, typename Void<typename T::IsHandle>::Type>
        : std::true_type {};
    template<typename T> struct is_stream<T, typename Void<typename T::IsStream>::Type>
        : std::true_type {};
    template<typename T> struct is_request<T, typename Void<typename T::IsRequest>::Type>
        : std::true_type {};

    template<typename From, typename To> struct is_handle_castable<From const&, To const&>
        : is_handle_castable<From&, To&> {};

    template<typename From> struct is_handle_castable<From&, uv_handle_t&>
        : is_handle<From> {};

    template<typename From> struct is_handle_castable<From&, uv_stream_t&>
        : is_stream<From> {};

    template<typename From> struct is_handle_castable<From&, uv_req_t&>
        : is_request<From> {};

    std::error_category const& uv_category();
    std::error_code make_error(int);
}
