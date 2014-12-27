#pragama once

#include "types.hh"

namespace uvpp {
    using ::uv_req_t;

    using ::uv_getaddrinfo_t;
    using ::uv_getnameinfo_t;
    using ::uv_shutdown_t;
    using ::uv_write_t;
    using ::uv_connect_t;
    using ::uv_udp_send_t;
    using ::uv_fs_t;
    using ::uv_work_t;

    class Request {
    public:
        using IsRequest = void;

        Request();
        virtual ~Request();

        virtual uv_req_t& GetRequest() = 0;
        virtual uv_req_t const& GetRequest() const = 0;
    };

    template<typename T>
    class RequestBase : public WrappedObject<T>, public Request {
    public:
        uv_req_t& GetRequest() {
            return handle_cast<uv_req_t&>(*this);
        }

        uv_req_t const& GetRequest() const {
            return handle_cast<uv_req_t const&>(*this);
        }
    };


    using WriteCb = std::function<void(WriteRequest&, int status)>;
    using ConnectCb = std::function<void(ConnectRequest&, int status)>;
    using ShutdownCb = std::function<void(ShutdownRequest&, int status)>;
    using FsCb = std::function<void(FsRequest&)>;
    using WorkCb = std::function<void(WorkRequest&)>;
    using AfterWorkCb = std::function<void(WorkRequest&, int status)>;
    using GetAddrInfoCb = std::function<void(GetAddrInfoRequest&, int status, ::addrinfo& res)>;
    using GetNameInfoCb = std::function<void(GetNameInfoRequest&, int status, std::string const& hostname, std::string const& service)>;


    class GetAddrInfoRequest : public RequestBase<uv_getaddrinfo_t> { public: GetAddrInfoRequest(); };
    class GetNameInfoRequest : public RequestBase<uv_getnameinfo_t> { public: GetNameInfoRequest(); };
    class ShutdownRequest : public RequestBase<uv_shutdown_t> { public: ShutdownRequest(); };
    class WriteRequest : public RequestBase<uv_write_t> { public: WriteRequest(); };
    class ConnectRequest : public RequestBase<uv_connect_t> { public: ConnectRequest(); };
    class UdpSendRequest : public RequestBase<uv_udp_send_t> { public: UdpSendRequest(); };
    class FsRequest : public RequestBase<uv_fs_t> { public: FsRequest(); };
    class WorkRequest : public RequestBase<uv_fs_t> { public: WorkRequest(); };
}