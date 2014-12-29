#include "streams.hh"

#include "loop.hh"

using namespace uvpp;


Stream::Stream(uv_stream_t* s) : Handle(reinterpret_cast<uv_handle_t*>(s)), stream_ptr(s) {}

Stream::~Stream() {}

uv_stream_t& Stream::GetStream() { return *stream_ptr; }
uv_stream_t const& Stream::GetStream() const { return *stream_ptr; }


Tcp::Tcp() : Tcp(default_loop) {}

Tcp::Tcp(Loop& loop) {
    uv_tcp_init(&loop.Get(), &Get());
}

Tcp::~Tcp() {
    if (!is_closing()) close();
}

Error uvpp::read_start(Stream& stream, AllocCb const& alloc_cb, ReadCb const& read_cb) {
    stream.on_alloc = alloc_cb;
    stream.on_read = read_cb;

    int s = ::uv_read_start(
        &stream.GetStream(),
        [](uv_handle_t* handle, ::size_t suggested_size, ::uv_buf_t* buf) {
            Handle* h = reinterpret_cast<Handle*>(handle->data);
            return h->on_alloc(*h, suggested_size, buf);
        },
        [](uv_stream_t* stream_ptr, ::ssize_t nread, ::uv_buf_t const* buf){
            Stream* strm = reinterpret_cast<Stream*>(stream_ptr->data);
            // FIXME: How do we handle errors - i.e. if nread < 0
            if (nread >= 0) {
                return strm->on_read(
                    *strm,
                    *buf,
                    nread,
                    make_error(0)
                );
            } else {
                return strm->on_read(
                    *strm,
                    *buf,
                    0,
                    make_error(static_cast<int>(nread))
                );
            }
        }
    );

    return make_error(s);
}

Error uvpp::read_stop(Stream& stream) {
    stream.on_alloc = nullptr;
    stream.on_read = nullptr;

    int s = ::uv_read_stop(&stream.GetStream());
    return make_error(s);
}

Error uvpp::listen(Stream& stream, int backlog, ConnectionCb const& connection_cb) {
    stream.on_connection = connection_cb;

    int s = ::uv_listen(
        &stream.GetStream(),
        backlog,
        [](uv_stream_t* server, int status) {
            Stream* strm = reinterpret_cast<Stream*>(server->data);
            return strm->on_connection(*strm, make_error(status));
        }
    );

    return make_error(s);
}

Error uvpp::accept(Stream& server, Stream& client) {
    int s = ::uv_accept(&server.GetStream(), &client.GetStream());
    return make_error(s);
}

Error uvpp::write(WriteRequest& req, Stream& stream, Buffer const bufs[], unsigned int nbufs,
    WriteCb const& write_cb)
{
    req.write_cb = write_cb;
    int s = uv_write(
        &req.Get(),
        &stream.GetStream(),
        bufs, nbufs,
        [] (uv_write_t* r, int status) {
            WriteRequest* w = reinterpret_cast<WriteRequest*>(r->data);
            w->write_cb(*w, make_error(status));
            w->write_cb = nullptr;
        }
    );

    return make_error(s);
}

Error uvpp::write2(WriteRequest& req, Stream& stream, Buffer const bufs[], unsigned int nbufs,
    Stream& send_handle, WriteCb const& write_cb)
{
    req.write_cb = write_cb;
    int s = uv_write2(
        &req.Get(),
        &stream.GetStream(),
        bufs, nbufs,
        &send_handle.GetStream(),
        [] (uv_write_t* r, int status) {
            WriteRequest* w = reinterpret_cast<WriteRequest*>(r->data);
            w->write_cb(*w, make_error(status));
            w->write_cb = nullptr;
        }
    );

    return make_error(s);
}

Error uvpp::try_write(Stream& stream, Buffer const bufs[], unsigned int nbufs) {
    int s = ::uv_try_write(
        &stream.GetStream(),
        bufs,
        nbufs
    );

    return make_error(s);
}

bool uvpp::is_readable(Stream const& stream) {
    return ::uv_is_readable(&stream.GetStream());
}

bool uvpp::is_writable(Stream const& stream) {
    return ::uv_is_writable(&stream.GetStream());
}
