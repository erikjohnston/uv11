#include "types.hh"

using namespace uvpp;


class UvErrorCategory : public std::error_category {
public:
    virtual const char* name() const noexcept;
    virtual std::string message( int condition ) const;
};

static UvErrorCategory uv_error_category;

std::error_category const& uvpp::uv_category() {
    return uv_error_category;
}

std::error_code uvpp::make_error(int s) {
    return Error(s, uv_category());
}

const char* UvErrorCategory::name() const noexcept { return "uv"; }
std::string UvErrorCategory::message( int condition ) const {
    if (condition >= 0) return "Not an error";
    return ::uv_strerror(condition);
}
