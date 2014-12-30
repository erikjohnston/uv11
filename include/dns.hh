#pragma once

#include "loop.hh"
#include "requests.hh"
#include "types.hh"

#include "uv.h"

namespace uvpp {
    using ::addrinfo;

    // Will automatically free the ptr if non null using ::uv_freeaddrinfo
    class UvAddrInfoPtr {
    public:
        UvAddrInfoPtr(addrinfo*);
        ~UvAddrInfoPtr();

        UvAddrInfoPtr(UvAddrInfoPtr const&) = delete;
        UvAddrInfoPtr(UvAddrInfoPtr&&);

        addrinfo& operator*();
        addrinfo const & operator*() const;

        addrinfo* operator->();
        addrinfo const * operator->() const;
    private:
        addrinfo* ptr;
    };

    Error getaddrinfo(
        Loop&,
        GetAddrInfoRequest&,
        GetAddrInfoCb const&,
        char const* node, char const* service,
        addrinfo& hints
    );

    Error getaddrinfo(
        Loop&,
        GetAddrInfoRequest&,
        GetAddrInfoCb const&,
        char const* node, char const* service
    );

    Error ip4_addr(const char* ip, int port, sockaddr_in& addr);
    Error ip4_addr(std::string const& ip, int port, sockaddr_in& addr);
}
