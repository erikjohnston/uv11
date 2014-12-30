#pragma once

#include "types.hh"

#include <cstdlib>
#include <functional>

namespace uvpp {
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

    using AllocCb = std::function<void(Handle&, std::size_t suggested_size, uv_buf_t* buf)>;
    using CloseCb = std::function<void(Handle&)>;

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
        HandleBase(void* data) : Handle(&this->Get(), data) {}

        uv_handle_t& GetHandle() {
            return handle_cast<uv_handle_t>(*this);
        }

        uv_handle_t const& GetHandle() const {
            return handle_cast<uv_handle_t>(*this);
        }
    };

    class Udp : public HandleBase<uv_udp_t> { public: Udp(); };
    class Poll : public HandleBase<uv_poll_t> { public: Poll(); };
    class Timer : public HandleBase<uv_timer_t> { public: Timer(); };
    class Prepare : public HandleBase<uv_prepare_t> { public: Prepare(); };
    class Check : public HandleBase<uv_check_t> { public: Check(); };
    class Idle : public HandleBase<uv_idle_t> { public: Idle(); };
    class Async : public HandleBase<uv_async_t> { public: Async(); };
    class Process : public HandleBase<uv_process_t> { public: Process(); };
    class FsEvent : public HandleBase<uv_fs_event_t> { public: FsEvent(); };
    class FsPoll : public HandleBase<uv_fs_poll_t> { public: FsPoll(); };
    class Signal : public HandleBase<uv_signal_t> { public: Signal(); };

//    template<> struct is_handle<Udp> : std::true_type {};
//    template<> struct is_handle<Poll> : std::true_type {};
//    template<> struct is_handle<Timer> : std::true_type {};
//    template<> struct is_handle<Prepare> : std::true_type {};
//    template<> struct is_handle<Check> : std::true_type {};
//    template<> struct is_handle<Idle> : std::true_type {};
//    template<> struct is_handle<Async> : std::true_type {};
//    template<> struct is_handle<Process> : std::true_type {};
//    template<> struct is_handle<FsEvent> : std::true_type {};
//    template<> struct is_handle<FsPoll> : std::true_type {};
//    template<> struct is_handle<Signal> : std::true_type {};

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
}
