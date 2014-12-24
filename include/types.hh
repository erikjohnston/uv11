#pragma once

#include "uv.h"

#include <type_traits>

namespace uvpp {
    using Handle = ::uv_handle_t;
    using Stream = ::uv_stream_t;



    template<typename T> struct is_stream : std::false_type {};
    template<typename T> struct is_handle : is_stream<T> {};

    template<> struct is_handle<Handle> : std::true_type {};
    template<> struct is_stream<Stream> : std::true_type {};

    template<typename From, typename To> struct is_handle_castable : std::false_type {};
    template<typename From> struct is_handle_castable<From&, Handle&> : is_handle<From> {};
    template<typename From> struct is_handle_castable<From&, Handle const&> : is_handle<From> {};
    template<typename From> struct is_handle_castable<From&, Stream&> : is_stream<From> {};
    template<typename From> struct is_handle_castable<From&, Stream const&> : is_stream<From> {};

    template<typename From, typename To>
    auto handle_cast(From&& from)
    -> typename std::enable_if<is_handle_castable<From, To>::value, To>::type {
        return reinterpret_cast<To const&>(std::forward<From>(from));
    }

    template<typename T>
    class Wrapper {
    public:
        T& Get() { return t; }
        T const& Get() const { return t; }

        operator T&() {
            return Get();
        }

        operator T const&() const {
            return Get();
        }
    private:
        T t;
    };

    template<typename From, typename To>
    auto handle_cast<Wrapper<From>, To>(Wrapper<From>&& from)
    -> typename std::enable_if<is_handle_castable<Wrapper<From>, To>::value, To>::type {
        return reinterpret_cast<To const&>(std::forward<From>(from.Get()));
    }

    template<typename T> struct is_stream<Wrapper<T>> : is_stream<T> {};

    template<typename T>
    class HandleWrapper : public Wrapper<T> {
    public:
        HandleWrapper() {}
        HandleWrapper(HandleWrapper const&) = delete;

        virtual ~HandleWrapper() {
            ::uv_close(&handle_cast(*this), nullptr);
        }

        operator Handle&() { return handle_cast(*this); }
        operator Handle const&() const { return handle_cast(*this); }
    };

    template<typename T> struct is_handle<HandleWrapper<T>> : std::true_type {};

    template<typename T>
    class StreamWrapper : public HandleWrapper<T> {
    public:
        StreamWrapper() {}
        StreamWrapper(StreamWrapper const&) = delete;

        operator Stream&() { return handle_cast(*this); }
        operator Stream const&() const { return handle_cast(*this); }
    };

    template<typename T> struct is_stream<StreamWrapper<T>> : std::true_type {};
};