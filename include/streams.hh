#pragma once

#include "buffer.hh"
#include "loop.hh"
#include "handles.hh"
#include "requests.hh"
#include "types.hh"

namespace uvpp {
    using ::uv_tcp_t;
    using ::uv_pipe_t;
    using ::uv_tty_t;

    class Stream;

    using ReadCb = std::function<void(Stream&, uv_buf_t const&)>;
    using ConnectionCb = std::function<void(Stream&, int status)>;

    class Stream : public Handle {
    public:
        using IsStream = void;

        virtual uv_stream_t& GetStream() = 0;
        virtual uv_stream_t const& GetStream() const = 0;

        virtual ~Stream();

        ReadCb on_read;
        ConnectionCb on_connection;
    };

    template<typename T>
    class StreamBase : public WrappedObject<T>, public Stream {
    public:
        uv_handle_t& GetHandle() {
            return handle_cast<uv_handle_t&>(*this);
        }

        uv_handle_t const& GetHandle() const {
            return handle_cast<uv_handle_t const&>(*this);
        }

        uv_stream_t& GetStream() {
            return handle_cast<uv_stream_t&>(*this);
        }

        uv_stream_t const& GetStream() const {
            return handle_cast<uv_stream_t const&>(*this);
        }
    };

    class Tcp : public StreamBase<uv_tcp_t> { public: Tcp(); Tcp(Loop&); virtual ~Tcp(); };
    class Pipe : public StreamBase<uv_pipe_t> { public: Pipe(); };
    class Tty : public StreamBase<uv_tty_t> { public: Tty(); };

//    template<> struct is_stream<Tcp> : std::true_type {};
//    template<> struct is_stream<Pipe> : std::true_type {};
//    template<> struct is_stream<Tty> : std::true_type {};

    int read_start(Stream&, AllocCb const&, ReadCb const&);
    int read_stop(Stream&);
    int listen(Stream&, int backlog, ConnectionCb const&);
    int accept(Stream& server, Stream& client);

    // For write APIs, vector.size() must return a size that can be store in an
    // unsigned int.
    int write(WriteRequest&, Stream&, Buffer const&, WriteCb const&);
    int write(WriteRequest&, Stream&, Buffer const[], unsigned int, WriteCb const&);
    int write2(WriteRequest&, Stream&, Buffer const&, Stream&, WriteCb const&);
    int write2(WriteRequest&, Stream&, Buffer const[], unsigned int, Stream&, WriteCb const&);
    int try_write(Stream&, Buffer const&);
    int try_write(Stream&, Buffer const[], unsigned int);

    bool is_readable(Stream const&);
    bool is_writable(Stream const&);
}
