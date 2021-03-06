#include "uv11/streams.hh"

using namespace uv11;


Stream::Stream(uv_stream_t* s, void* data)
    : Handle(reinterpret_cast<uv_handle_t*>(s), data), stream_ptr(s) {}

Stream::~Stream() {}

uv_stream_t& Stream::GetStream() { return *stream_ptr; }
uv_stream_t const& Stream::GetStream() const { return *stream_ptr; }


Error uv11::read_start(Stream& stream, AllocCb const& alloc_cb, ReadCb const& read_cb) {
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

    if (s) {
        stream.on_alloc = nullptr;
        stream.on_read = nullptr;
    }

    return make_error(s);
}

Error uv11::read_stop(Stream& stream) {
    stream.on_alloc = nullptr;
    stream.on_read = nullptr;

    int s = ::uv_read_stop(&stream.GetStream());
    return make_error(s);
}

Error uv11::listen(Stream& stream, int backlog, ConnectionCb const& connection_cb) {
    stream.on_connection = connection_cb;

    int s = ::uv_listen(
        &stream.GetStream(),
        backlog,
        [](uv_stream_t* server, int status) {
            Stream* strm = reinterpret_cast<Stream*>(server->data);
            return strm->on_connection(*strm, make_error(status));
        }
    );

    if (s) stream.on_connection = nullptr;

    return make_error(s);
}

Error uv11::accept(Stream& server, Stream& client) {
    int s = ::uv_accept(&server.GetStream(), &client.GetStream());
    return make_error(s);
}

Error uv11::write(WriteRequest& req, Stream& stream, Buffer const& buf,
    WriteCb const& write_cb)
{
    return uv11::write(req, stream, &buf, 1, write_cb);
}

Error uv11::write(WriteRequest& req, Stream& stream, Buffer const bufs[], unsigned int nbufs,
    WriteCb const& write_cb)
{
    req.write_cb = write_cb;
    int s = uv_write(
        &req.Get(),
        &stream.GetStream(),
        bufs, nbufs,
        [] (uv_write_t* r, int status) {
            WriteRequest* w = reinterpret_cast<WriteRequest*>(r->data);
            auto w_cb = w->write_cb;
            w->write_cb = nullptr;
            w_cb(*w, make_error(status));
        }
    );

    if (s) req.write_cb = nullptr;

    return make_error(s);
}

Error uv11::write2(WriteRequest& req, Stream& stream, Buffer const& buf, Stream& send_handle,
    WriteCb const& write_cb)
{
    return uv11::write2(req, stream, &buf, 1, send_handle, write_cb);
}

Error uv11::write2(WriteRequest& req, Stream& stream, Buffer const bufs[], unsigned int nbufs,
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
            auto w_cb = w->write_cb;
            w->write_cb = nullptr;
            w_cb(*w, make_error(status));
        }
    );

    if (s) req.write_cb = nullptr;

    return make_error(s);
}

Error uv11::try_write(Stream& stream, Buffer const& buf) {
    return uv11::try_write(stream, &buf, 1);
}

Error uv11::try_write(Stream& stream, Buffer const bufs[], unsigned int nbufs) {
    int s = ::uv_try_write(
        &stream.GetStream(),
        bufs,
        nbufs
    );

    return make_error(s);
}

bool uv11::is_readable(Stream const& stream) {
    return ::uv_is_readable(&stream.GetStream());
}

bool uv11::is_writable(Stream const& stream) {
    return ::uv_is_writable(&stream.GetStream());
}

Error uv11::stream_set_blocking(Stream& stream, bool blocking) {
    int s = ::uv_stream_set_blocking(&stream.GetStream(), blocking);

    return make_error(s);
}
