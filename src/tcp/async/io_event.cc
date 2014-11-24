#include "tcp/async/io_event.hh"
#include "tcp/async/client.hh"

#include <unistd.h>
#include <netinet/in.h>
#include <sys/epoll.h>

#include <stdexcept>
#include <cstring>
#include <ldap.h>

using namespace std;
using namespace tcp::async;
using namespace tcp::util;

read_event::read_event(int fd,
        size_t count,
        on_read_cb cb)
        : fd(fd), buf(count), on_read(cb)
{
}

write_event::write_event(int fd,
        buffer b,
        on_write_cb cb)
        : fd(fd), buf(b), on_write(cb)
{
}

accept_event::accept_event(int fd,
        on_accept_cb cb)
        : fd(fd), on_accept(cb)
{
}

connect_event::connect_event(int fd,
        util::address addr,
        on_connect_cb cb)
        : fd(fd), addr(addr), on_connect(cb)
{
}

bool read_event::handle()
{
    ssize_t read = ::read(fd, *buf, buf.rest_length());
    if (read < 0) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            return false;
        } else {
            throw runtime_error(strerror(errno));
        }
    }

    buf += read;
    if (buf.rest_length() == 0) {
        on_read(buf);
        return true;
    }

    return false;
}

bool write_event::handle()
{
    ssize_t written = write(fd, *buf, buf.rest_length());
    if (written < 0) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            return false;
        } else {
            throw runtime_error(strerror(errno));
        }
    }

    buf += written;
    if (buf.rest_length() == 0) {
        on_write();
        return true;
    }

    return false;
}

bool accept_event::handle()
{
    sockaddr_in addr;
    socklen_t size = sizeof(addr);
    int peer_fd = accept4(fd, (sockaddr*) &addr, &size, SOCK_NONBLOCK);
    if (peer_fd < 0) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            return false;
        } else {
            throw runtime_error(strerror(errno));
        }
    }

    on_accept(client(peer_fd));
    return true;
}

bool connect_event::handle()
{
    sockaddr_in sock_addr = addr.get_sockaddr();
    socklen_t size = sizeof(sock_addr);
    int res = connect(fd, (sockaddr*) &sock_addr, size);
    if (res < 0) {
        if (errno == EINPROGRESS) {
            return false;
        } else {
            throw runtime_error(strerror(errno));
        }
    }

    on_connect();
    return true;
}

uint32_t read_event::events_flag()
{
    return EPOLLIN;
}

uint32_t write_event::events_flag()
{
    return EPOLLOUT;
}

uint32_t accept_event::events_flag()
{
    return EPOLLIN;
}

uint32_t connect_event::events_flag()
{
    return EPOLLOUT;
}
