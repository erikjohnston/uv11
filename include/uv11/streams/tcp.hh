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
}
