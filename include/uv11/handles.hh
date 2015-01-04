#pragma once

#include "types.hh"

#include <cstdlib>
#include <functional>

namespace uv11 {
    using ::uv_udp_t;
    using ::uv_poll_t;
    using ::uv_timer_t;
    using ::uv_prepare_t;
    using ::uv_check_t;
    using ::uv_idle_t;
    using ::uv_async_t;
    using ::uv_process_t;
    using ::uv_fs_event_t;
    using ::uv_fs_poll_t;
    using ::uv_signal_t;

    class Handle;
    class Poll;

    using AllocCb = std::function<void(Handle&, std::size_t suggested_size, uv_buf_t* buf)>;
    using CloseCb = std::function<void(Handle&)>;
    using PollCb = std::function<void(Poll&, Error, int events)>;
    using AsyncCb = std::function<void(Async&)>;

    class Handle {
    public:
        using IsHandle = void;

        Handle(uv_handle_t*, void*);
        virtual ~Handle();

        uv_handle_t& GetHandle();
        uv_handle_t const& GetHandle() const;

        void close();
        void close(CloseCb const&);

        bool is_closing() const;  // uv_is_closing cannot be called after receiving the close_cb

        AllocCb on_alloc;
        CloseCb on_close;

    protected:
        ::uv_handle_t* handle_ptr;
        bool closing = false;
    };

    template<typename T>
    class HandleBase : public WrappedObject<T>, public Handle {
    public:
        HandleBase(void* data) : Handle(reinterpret_cast<uv_handle_t*>(&this->Get()), data) {}

        uv_handle_t& GetHandle() {
            return handle_cast<uv_handle_t>(*this);
        }

        uv_handle_t const& GetHandle() const {
            return handle_cast<uv_handle_t>(*this);
        }
    };

    class Udp : public HandleBase<uv_udp_t> { public: Udp(); };

    class Poll : public HandleBase<uv_poll_t> {
    public:
        Poll(int fd);
        Poll(Loop&, int fd);
        virtual ~Poll();

        PollCb poll_cb;
    };

    class Async : public HandleBase<uv_async_t> {
    public:
        Async(Loop&, AsyncCb const&);
        virtual ~Async();

        AsyncCb async_cb;
    };

    class Timer : public HandleBase<uv_timer_t> { public: Timer(); };
    class Prepare : public HandleBase<uv_prepare_t> { public: Prepare(); };
    class Check : public HandleBase<uv_check_t> { public: Check(); };
    class Idle : public HandleBase<uv_idle_t> { public: Idle(); };
    class Process : public HandleBase<uv_process_t> { public: Process(); };
    class FsEvent : public HandleBase<uv_fs_event_t> { public: FsEvent(); };
    class FsPoll : public HandleBase<uv_fs_poll_t> { public: FsPoll(); };
    class Signal : public HandleBase<uv_signal_t> { public: Signal(); };


    bool is_active(Handle const&);
    bool is_closing(Handle const&);
    void close(Handle&);
    void close(Handle&, CloseCb const&);
    void ref(Handle&);
    void unref(Handle&);
    bool has_ref(Handle const&);

    Error send_buffer_size(Handle&, int* value);
    Error recv_buffer_size(Handle&, int* value);
    Error fileno(Handle const&, uv_os_fd_t*);

    Error poll_start(Poll&, int events, PollCb const&);
    Error poll_stop(Poll&);

    Error async_send(Async&);
}
