#include <iostream>

#include "uv11/dns.hh"
#include "uv11/types.hh"
#include "uv11/streams.hh"
#include "uv11/streams/tcp.hh"
#include "uv11/loop.hh"
#include "uv11/requests.hh"

#include <memory>

int main() {

    auto addrRequest = std::make_shared<uv11::GetAddrInfoRequest>();
    uv11::getaddrinfo(
        uv11::default_loop,
        *addrRequest,
        [] (uv11::GetAddrInfoRequest&, uv11::Error e, uv11::UvAddrInfoPtr res) {
            if (e) {
                std::cout << "Addr info error: " << e.message() << std::endl;
            } else {
                std::string ip;
                addrinfo& a = *res;
                while (true) {
                    void * in_addr;
                    if (a.ai_family == AF_INET) {
                        in_addr = &reinterpret_cast<sockaddr_in*>(a.ai_addr)->sin_addr;
                    } else if (a.ai_family == AF_INET6) {
                        in_addr = &reinterpret_cast<sockaddr_in6*>(a.ai_addr)->sin6_addr;
                    } else {
                        std::cout << "unrecognized family " << a.ai_family << std::endl;
                        break;
                    }

                    auto err = uv11::inet_ntop(
                        a.ai_family,
                        &reinterpret_cast<sockaddr_in*>(a.ai_addr)->sin_addr,
                        ip
                    );

                    if (err) {
                        std::cout << "Failed to parse " << err.message() << std::endl;
                        break;
                    }

                    auto p = ::getprotobynumber(a.ai_protocol);
                    std::cout << ((a.ai_family == AF_INET) ? "IPv4" : "IPv6") << " " << ip << " " << p->p_name<< std::endl;

                    if (a.ai_next) {
                        a = *a.ai_next;
                        continue;
                    } else {
                        break;
                    }
                }
            }
        },
        "www.google.com", nullptr
    );

    sockaddr_in addr;

    auto tcp = std::make_shared<uv11::Tcp>();
    auto connectRequest = std::make_shared<uv11::ConnectRequest>();

    uv11::ip4_addr("216.58.208.46", 80, addr);

    auto e = uv11::tcp_connect(
        *connectRequest, *tcp, reinterpret_cast<sockaddr&>(addr),
        [tcp, connectRequest] (uv11::ConnectRequest&, uv11::Error e) {
            std::cout << "Connected!" << std::endl;
            if (e) {
                std::cout << e.message() << std::endl;
            }

            auto write_req = std::make_shared<uv11::WriteRequest>();

            char temp[] =  "GET / HTTP/1.1\r\nContent-Length: 0\r\nConnection: Close\r\n\r\n";
            auto get_request = std::make_shared<std::vector<char>>(
                std::begin(temp), std::end(temp)
            );

            uv11::write(
                *write_req,
                *tcp,
                uv11::make_buffer(*get_request),
                [tcp, write_req, get_request] (uv11::WriteRequest&, uv11::Error e) {
                    std::cout << "written" << std::endl;
                    if (e) {
                        std::cout << "Write error: " << e.message() << std::endl;
                    }
                }
            );
        }
    );

    uv11::read_start(
        *tcp,
        [] (uv11::Handle&, std::size_t suggested_size, uv11::uv_buf_t* buf) {
            buf->base = new char[suggested_size];
            buf->len = suggested_size;
        },
        [](uv11::Stream&, uv11::Buffer const& buf, ::ssize_t nread, uv11::Error e) {
            if (e) {
                std::cout << "Read error: " << e.message() << std::endl;
            } else {
                std::cout.write(buf.base, nread);
                std::cout << std::endl;
            }

            delete[] buf.base;
        });

    if (e) {
        std::cout << e.message() << std::endl;
    }

    return uv11::run(uv11::default_loop, uv11::RunMode::UV_RUN_DEFAULT);
}
