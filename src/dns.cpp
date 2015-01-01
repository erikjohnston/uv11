#include <loop.hh>
#include "dns.hh"

using namespace uv11;

UvAddrInfoPtr::UvAddrInfoPtr(addrinfo* p) : ptr(p) {}

UvAddrInfoPtr::~UvAddrInfoPtr() {
    if (ptr) ::uv_freeaddrinfo(ptr);
}

addrinfo& UvAddrInfoPtr::operator*() { return *ptr; }
addrinfo const & UvAddrInfoPtr::operator*() const { return *ptr; }

addrinfo* UvAddrInfoPtr::operator->() { return ptr; }
addrinfo const * UvAddrInfoPtr::operator->() const { return ptr; }


UvAddrInfoPtr::UvAddrInfoPtr(UvAddrInfoPtr&& other) : ptr(other.ptr) {
    other.ptr = nullptr;
}


Error uv11::getaddrinfo(
    Loop& loop,
    GetAddrInfoRequest& req,
    GetAddrInfoCb const& callback,
    char const* node, char const* service,
    addrinfo& hints
) {
    req.getaddrinfo_cb = callback;
    int s = ::uv_getaddrinfo(
        &loop.Get(),
        &req.Get(),
        [](uv_getaddrinfo_t* req_ptr, int status, struct addrinfo* res){
            GetAddrInfoRequest* r = reinterpret_cast<GetAddrInfoRequest*>(req_ptr->data);
            r->getaddrinfo_cb(*r, make_error(status), UvAddrInfoPtr(res));
        },
        node, service,
        &hints
    );

    return make_error(s);
}

Error uv11::getaddrinfo(
    Loop& loop,
    GetAddrInfoRequest& req,
    GetAddrInfoCb const& callback,
    char const* node, char const* service
) {
    req.getaddrinfo_cb = callback;
    int s = ::uv_getaddrinfo(
        &loop.Get(),
        &req.Get(),
        [](uv_getaddrinfo_t* req_ptr, int status, struct addrinfo* res){
            GetAddrInfoRequest* r = reinterpret_cast<GetAddrInfoRequest*>(req_ptr->data);
            r->getaddrinfo_cb(*r, make_error(status), UvAddrInfoPtr(res));
        },
        node, service,
        nullptr
    );

    return make_error(s);
}

Error uv11::ip4_addr(const char* ip, int port, sockaddr_in& addr) {
    int s = ::uv_ip4_addr(ip, port, &addr);
    return make_error(s);
}

Error uv11::ip4_addr(std::string const& ip, int port, sockaddr_in& addr) {
    return ip4_addr(ip.c_str(), port, addr);
}

Error uv11::ip4_name(sockaddr_in const* src, std::string& dst) {
    dst.clear();
    dst.resize(16, 0);
    int s = ::uv_ip4_name(src, &dst.front(), dst.size());

    if (s) {
        dst.clear();
    } else {
        auto r = dst.find('\0');
        if (r == dst.npos) {
            dst.clear();
        } else {
            dst.resize(r);
        }
    }

    return make_error(s);
}

Error uv11::inet_ntop(int af, const void* src, std::string& dst) {
    dst.clear();
    dst.resize(45, 0);

    int s = ::uv_inet_ntop(af, src, &dst.front(), dst.size());

    if (s) {
        dst.clear();
    } else {
        auto r = dst.find('\0');
        if (r == dst.npos) {
            dst.clear();
        } else {
            dst.resize(r);
        }
    }

    return make_error(s);
}
