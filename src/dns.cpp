#include <loop.hh>
#include "dns.hh"

using namespace uvpp;

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


Error uvpp::getaddrinfo(
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

Error uvpp::getaddrinfo(
    Loop&,
    GetAddrInfoRequest&,
    GetAddrInfoCb const&,
    char const* node, char const* service
);

Error uvpp::ip4_addr(const char* ip, int port, sockaddr_in& addr) {
    int s = ::uv_ip4_addr(ip, port, &addr);
    return make_error(s);
}

Error uvpp::ip4_addr(std::string const& ip, int port, sockaddr_in& addr) {
    return ip4_addr(ip.c_str(), port, addr);
}
