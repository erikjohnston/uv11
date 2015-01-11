#pragma once

#include "uv.h"

#include "internals.hh"
#include "types.hh"

#include <cstdlib>
#include <functional>

namespace uv11 {
    class Handle;

    using AllocCb = std::function<void(Handle&, std::size_t suggested_size, uv_buf_t* buf)>;
    using CloseCb = std::function<void(Handle&)>;

    class Handle {
    public:
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


//    class Udp : public HandleBase<uv_udp_t> { public: Udp(); };



//    class Timer : public HandleBase<uv_timer_t> { public: Timer(); };
//    class Prepare : public HandleBase<uv_prepare_t> { public: Prepare(); };
//    class Check : public HandleBase<uv_check_t> { public: Check(); };
//    class Idle : public HandleBase<uv_idle_t> { public: Idle(); };
//    class Process : public HandleBase<uv_process_t> { public: Process(); };
//    class FsEvent : public HandleBase<uv_fs_event_t> { public: FsEvent(); };
//    class FsPoll : public HandleBase<uv_fs_poll_t> { public: FsPoll(); };
//    class Signal : public HandleBase<uv_signal_t> { public: Signal(); };


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


    // INTERNALS
    template<typename T>
    class HandleBase : public WrappedObject<T>, public Handle {
    public:
        HandleBase(void* data) : Handle(reinterpret_cast<uv_handle_t*>(&this->Get()), data) {}
    };

}
