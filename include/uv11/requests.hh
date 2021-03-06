#pragma once

#include "uv.h"

#include "types.hh"
#include "internals.hh"

#include <functional>

namespace uv11 {
    class Request {
    public:
        Request(uv_req_t*, void*);
        Request(Request const&) = delete;

        uv_req_t& GetRequest();
        uv_req_t const& GetRequest() const;
    private:
        uv_req_t* req_ptr;
    };

    template<typename T>
    class RequestBase : public WrappedObject<T>, public Request {
    public:
        RequestBase()
            : Request(reinterpret_cast<uv_req_t*>(&this->Get()), this) {}
    };

    class GetAddrInfoRequest;
    class GetNameInfoRequest;
    class ShutdownRequest;
    class WriteRequest;
    class ConnectRequest;
    class UdpSendRequest;
    class FsRequest;
    class WorkRequest;

    class UvAddrInfoPtr;

    using WriteCb = std::function<void(WriteRequest&, Error)>;
    using ConnectCb = std::function<void(ConnectRequest&, Error)>;
    using ShutdownCb = std::function<void(ShutdownRequest&, Error)>;
    using FsCb = std::function<void(FsRequest&)>;
    using WorkCb = std::function<void(WorkRequest&)>;
    using AfterWorkCb = std::function<void(WorkRequest&, int status)>;
    using GetAddrInfoCb = std::function<void(GetAddrInfoRequest&, Error, UvAddrInfoPtr res)>;
    using GetNameInfoCb = std::function<void(GetNameInfoRequest&, Error, std::string const& hostname, std::string const& service)>;


    class GetAddrInfoRequest : public RequestBase<uv_getaddrinfo_t> {
    public: GetAddrInfoCb getaddrinfo_cb;
    };

    class GetNameInfoRequest : public RequestBase<uv_getnameinfo_t> {
    public: GetNameInfoCb getnameinfo_cb;
    };

    class ShutdownRequest : public RequestBase<uv_shutdown_t> {
    public: ShutdownCb shutdown_cb;
    };

    class WriteRequest : public RequestBase<uv_write_t> {
    public: WriteCb write_cb;
    };

    class ConnectRequest : public RequestBase<uv_connect_t> {
    public: ConnectCb connect_cb;
    };

    class UdpSendRequest : public RequestBase<uv_udp_send_t> {};

    class FsRequest : public RequestBase<uv_fs_t> {
    public: FsCb fs_cb;
    };

    class WorkRequest : public RequestBase<uv_fs_t> {
    public:
        WorkCb work_cb;
        AfterWorkCb after_work_cb;
    };
}
