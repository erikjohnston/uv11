#include "uv11/requests.hh"

using namespace uv11;

Request::Request(uv_req_t * r, void* data) : req_ptr(r) {
    r->data = data;
}


uv_req_t& Request::GetRequest() { return *req_ptr; }
uv_req_t const& Request::GetRequest() const { return *req_ptr; }
