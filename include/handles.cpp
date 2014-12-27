#include "handles.hh"

using namespace uvpp;

Handle::Handle() {
    GetHandle().data = this;
}