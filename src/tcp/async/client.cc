#include "tcp/async/client.hh"

#include <sys/socket.h>
#include <unistd.h>

#include <stdexcept>
#include <cstring>
#include <cerrno>
#include <bits/stl_algo.h>
#include <ldap.h>
#include <ksba.h>

using namespace std;
using namespace tcp::async;

client::client()
    : fd(socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0))
{
    if (fd < 0) {
        throw runtime_error(strerror(errno));
    }
}

void client::connect(io_service& service,
        tcp::util::address const& addr,
        on_connect_cb cb)
{
    service.add_event(fd, new connect_event(fd, addr, cb));
}

void client::read(io_service& service,
        size_t count,
        on_read_cb cb)
{
    service.add_event(fd, new read_event(fd, count, cb));
}

void client::write(io_service& service, tcp::util::buffer buffer, on_write_cb cb)
{
    service.add_event(fd, new write_event(fd, buffer, cb));
}

void client::close()
{
    ::close(fd);
}
