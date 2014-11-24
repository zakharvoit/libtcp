#include "tcp/async/client.hh"

#include <sys/socket.h>
#include <unistd.h>

#include <stdexcept>
#include <cstring>
#include <cerrno>

using namespace std;
using namespace tcp::async;
using namespace tcp::util;

client::client()
    : fd(socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0))
{
    if (fd < 0) {
        throw runtime_error(strerror(errno));
    }
}

canceller client::connect(io_service& service,
        tcp::util::address const& addr,
        on_connect_cb cb)
{
    auto ev = new connect_event(fd, addr, cb);
    service.add_event(fd, ev);
    return make_canceller(ev);
}

canceller client::read(io_service& service,
        size_t count,
        on_read_cb cb)
{
    auto ev = new read_event(fd, count, cb);
    service.add_event(fd, ev);
    return make_canceller(ev);
}

canceller client::write(io_service& service, buffer buffer, on_write_cb cb)
{

    auto ev = new write_event(fd, buffer, cb);
    service.add_event(fd, ev);
    return make_canceller(ev);
}

client::client(client&& c)
{
    close(fd);
    fd = c.fd;
    c.fd = -1;
}

void client::operator=(client&& other)
{
    close(fd);
    fd = other.fd;
    other.fd = -1;
}

client::~client()
{
    close(fd);
}
