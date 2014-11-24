#include "tcp/async/client.hh"

#include <sys/socket.h>
#include <unistd.h>

#include <stdexcept>
#include <cstring>
#include <cerrno>

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
