#pragma once

#include <utility>

namespace uv11 {
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

}
