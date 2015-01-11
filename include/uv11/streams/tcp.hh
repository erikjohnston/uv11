#pragma once

#include "../streams.hh"
#include "../internals.hh"


namespace uv11 {
    using ::uv_tcp_t;

    class Tcp : public StreamBase<uv_tcp_t> {
    public:
        Tcp();
        Tcp(Loop&);
        virtual ~Tcp();
    };

    Error tcp_connect(ConnectRequest&, Tcp&, sockaddr const&, ConnectCb const&);
    Error tcp_open(Tcp&, ::uv_os_sock_t);
    Error tcp_nodely(Tcp&, bool);
    Error tcp_keepalive(Tcp&, bool, unsigned int delay);
    Error tcp_simultaneous_accepts(Tcp&, bool enable);
    Error tcp_bind(Tcp&, sockaddr const&, unsigned int flags);
    Error tcp_getsockname(Tcp const&, sockaddr&, int*);
    Error tcp_getpeername(Tcp const&, sockaddr&, int*);
}
