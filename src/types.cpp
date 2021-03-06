#include "uv11/types.hh"

#include "uv.h"

using namespace uv11;


class UvErrorCategory : public std::error_category {
public:
    virtual const char* name() const noexcept;
    virtual std::string message( int condition ) const;
};

static UvErrorCategory uv_error_category;

std::error_category const& uv11::uv_category() {
    return uv_error_category;
}

std::error_code uv11::make_error(int s) {
    return Error(s, uv_category());
}

const char* UvErrorCategory::name() const noexcept { return "uv"; }
std::string UvErrorCategory::message( int condition ) const {
    if (condition >= 0) return "Not an error";
    return ::uv_strerror(condition);
}
