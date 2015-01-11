#pragma once

#include "buffer.hh"
#include "loop.hh"
#include "internals.hh"
#include "handles.hh"
#include "requests.hh"
#include "types.hh"

namespace uv11 {
    class Stream;

    using ReadCb = std::function<void(Stream&, Buffer const&, ::ssize_t nread, Error)>;
    using ConnectionCb = std::function<void(Stream&, Error)>;

    class Stream : public Handle {
    public:
        Stream(uv_stream_t*, void*);
        virtual ~Stream();

        uv_stream_t& GetStream();
        uv_stream_t const& GetStream() const;

        ReadCb on_read;
        ConnectionCb on_connection;

    private:
        uv_stream_t* stream_ptr;
    };


//    class Pipe : public StreamBase<uv_pipe_t> { public: Pipe(); };
//    class Tty : public StreamBase<uv_tty_t> { public: Tty(); };


    Error read_start(Stream&, AllocCb const&, ReadCb const&);
    Error read_stop(Stream&);
    Error listen(Stream&, int backlog, ConnectionCb const&);
    Error accept(Stream& server, Stream& client);

    // For write APIs, vector.size() must return a size that can be store in an
    // unsigned int.
    Error write(WriteRequest&, Stream&, Buffer const&, WriteCb const&);
    Error write(WriteRequest&, Stream&, Buffer const[], unsigned int, WriteCb const&);
    Error write2(WriteRequest&, Stream&, Buffer const&, Stream&, WriteCb const&);
    Error write2(WriteRequest&, Stream&, Buffer const[], unsigned int, Stream&, WriteCb const&);
    Error try_write(Stream&, Buffer const&);
    Error try_write(Stream&, Buffer const[], unsigned int);

    bool is_readable(Stream const&);
    bool is_writable(Stream const&);


    // INTERNALS
    template<typename T>
    class StreamBase : public WrappedObject<T>, public Stream {
    public:
        StreamBase(void * data) : Stream(reinterpret_cast<uv_stream_t*>(&this->Get()), data) {}
    };
}
