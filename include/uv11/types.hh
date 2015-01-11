#pragma once

#include <system_error>


namespace uv11 {
    using Error = std::error_code;

    std::error_category const& uv_category();
    std::error_code make_error(int);
}
