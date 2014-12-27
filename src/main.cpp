#include "types.hh"
#include "streams.hh"
#include "loop.hh"

#include <iostream>

int main() {
    std::cout
        << uvpp::is_stream<uvpp::Stream>::value
        << std::endl
    ;

    uvpp::Tcp tcp;

    return 0;
}
