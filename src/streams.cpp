#include "streams.hh"

using namespace uvpp;

Tcp::Tcp() : Tcp(default_loop) {}

Tcp::Tcp(Loop& loop) {
    uv_tcp_init(&loop.Get(), &Get());
}

Tcp::~Tcp() {
    if (!is_closing()) close();
}

int uvpp::read_start(Stream& stream, AllocCb const& alloc_cb, ReadCb const& read_cb) {
    stream.on_alloc = alloc_cb;
    stream.on_read = read_cb;

    return ::uv_read_start(
        &stream.GetStream(),
        [](uv_handle_t* handle, ::size_t suggested_size, ::uv_buf_t* buf) {
            Handle* h = reinterpret_cast<Handle*>(handle->data);
            return h->on_alloc(*h, suggested_size, buf);
        },
        [](uv_stream_t* stream, ::ssize_t nread, ::uv_buf_t const* buf){
            Stream* s = reinterpret_cast<Stream*>(stream->data);
            // FIXME: How do we handle errors - i.e. if nread < 0
            return s->on_read(*s, make_buffer(buf->base, nread));
        }
    );
}

int uvpp::read_stop(Stream& stream) {
    stream.on_alloc = nullptr;
    stream.on_read = nullptr;

    return ::uv_read_stop(&stream.GetStream());
}

int uvpp::listen(Stream& stream, int backlog, ConnectionCb const& connection_cb) {
    stream.on_connection = connection_cb;

    return ::uv_listen(
        &stream.GetStream(),
        backlog,
        [](uv_stream_t* server, int status) {
            Stream* s = reinterpret_cast<Stream*>(server->data);
            return s->on_connection(*s, status);
        }
    );
}

int uvpp::accept(Stream& server, Stream& client) {
    return ::uv_accept(&server.GetStream(), &client.GetStream());
}

int uvpp::write(WriteRequest& req, Stream& stream, Buffer const bufs[], unsigned int nbufs,
    WriteCb const& write_cb)
{
    req.write_cb = write_cb;
    return uv_write(
        &req.Get(),
        &stream.GetStream(),
        bufs, nbufs,
        [] (uv_write_t* r, int status) {
            WriteRequest* w = reinterpret_cast<WriteRequest*>(r->data);
            w->write_cb(*w, status);
            w->write_cb = nullptr;
        }
    );
}

int uvpp::write2(WriteRequest& req, Stream& stream, Buffer const bufs[], unsigned int nbufs,
    Stream& send_handle, WriteCb const& write_cb)
{
    req.write_cb = write_cb;
    return uv_write2(
        &req.Get(),
        &stream.GetStream(),
        bufs, nbufs,
        &send_handle.GetStream(),
        [] (uv_write_t* r, int status) {
            WriteRequest* w = reinterpret_cast<WriteRequest*>(r->data);
            w->write_cb(*w, status);
            w->write_cb = nullptr;
        }
    );
}

int uvpp::try_write(Stream& stream, Buffer const bufs[], unsigned int nbufs) {
    return ::uv_try_write(
        &stream.GetStream(),
        bufs,
        nbufs
    );
}

bool uvpp::is_readable(Stream const& stream) {
    return ::uv_is_readable(&stream.GetStream());
}

bool uvpp::is_writable(Stream const& stream) {
    return ::uv_is_writable(&stream.GetStream());
}
