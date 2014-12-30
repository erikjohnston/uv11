#include <iostream>

#include "dns.hh"
#include "types.hh"
#include "streams.hh"
#include "loop.hh"
#include "requests.hh"

#include <memory>

auto tcp = std::make_shared<uvpp::Tcp>();
auto connectRequest = std::make_shared<uvpp::ConnectRequest>();

int main() {
    std::cout
        << uvpp::is_stream<uvpp::Stream>::value
        << std::endl
    ;

    sockaddr_in addr;

    uvpp::ip4_addr("216.58.208.46", 80, addr);

    auto e = uvpp::tcp_connect(
        *connectRequest, *tcp, reinterpret_cast<sockaddr&>(addr),
        [] (uvpp::ConnectRequest&, uvpp::Error e) {
            std::cout << "Connected!" << std::endl;
            if (e) {
                std::cout << e.message() << std::endl;
            }

            auto write_req = std::make_shared<uvpp::WriteRequest>();

            char temp[] =  "GET / HTTP/1.1\r\nContent-Length: 0\r\nConnection: Close\r\n\r\n";
            auto get_request = std::make_shared<std::vector<char>>(
                std::begin(temp), std::end(temp)
            );

            uvpp::write(
                *write_req,
                *tcp,
                uvpp::make_buffer(*get_request),
                [write_req, get_request] (uvpp::WriteRequest&, uvpp::Error e) {
                    std::cout << "written" << std::endl;
                    if (e) {
                        std::cout << "Write error: " << e.message() << std::endl;
                    }
                }
            );
        }
    );

    uvpp::read_start(
        *tcp,
        [] (uvpp::Handle&, std::size_t suggested_size, uvpp::uv_buf_t* buf) {
            buf->base = new char[suggested_size];
            buf->len = suggested_size;
        },
        [](uvpp::Stream&, uvpp::Buffer const& buf, ::ssize_t nread, uvpp::Error e) {
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

    return uvpp::run(uvpp::default_loop, uvpp::RunMode::UV_RUN_DEFAULT);
}
