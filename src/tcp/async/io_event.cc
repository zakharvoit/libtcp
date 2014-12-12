#include "tcp/async/io_event.hh"
#include "tcp/async/client.hh"

#include <unistd.h>
#include <netinet/in.h>
#include <sys/epoll.h>

#include <stdexcept>
#include <cstring>
#include <iostream>

using namespace std;
using namespace tcp::async;
using namespace tcp::util;

read_event::read_event(int fd,
		       size_t count,
		       on_read_cb cb)
    : fd(fd), buf(count), on_read(cb)
{
}

read_some_event::read_some_event(int fd,
				 on_read_cb cb)
    : fd(fd), buf(new char[MAX_BUFFER_SIZE]), on_read(cb)
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

bool io_event::handle()
{
    return cancelled;
}

bool read_event::handle()
{
    if (io_event::handle()) return true;

    ssize_t read = ::read(fd, *buf, buf.rest_length());
    if (read < 0) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            return false;
        } else {
	    on_read(error<buffer>(errno));
	    return true;
        }
    } else if (read == 0) {
	on_read(error<buffer>(ECONNABORTED));
	return true;
    }

    buf += read;
    if (buf.rest_length() == 0) {
        buf.reset();
        on_read(success(buf));
        return true;
    }

    return false;
}

bool read_some_event::handle()
{
    if (io_event::handle()) return true;

    ssize_t read = ::read(fd, buf,
			  MAX_BUFFER_SIZE);
    if (read < 0) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            return false;
        } else {
	    on_read(error<buffer>(errno));
	    return true;
        }
    } else if (read == 0) {
	on_read(error<buffer>(ECONNABORTED));
	return true;
    }

    buffer result(buf, read);
    on_read(success(result));
    return true;
}

bool write_event::handle()
{
    if (io_event::handle()) return true;

    ssize_t written = write(fd, *buf, buf.rest_length());
    if (written < 0) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            return false;
        } else {
	    on_write(error<nothing>(errno));
	    return true;
        }
    }

    buf += written;
    if (buf.rest_length() == 0) {
        on_write(success());
        return true;
    }

    return false;
}

bool accept_event::handle()
{
    if (io_event::handle()) return true;

    sockaddr_in addr;
    socklen_t size = sizeof(addr);
    int peer_fd = accept4(fd, (sockaddr*) &addr, &size, SOCK_NONBLOCK);
    if (peer_fd < 0) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            return false;
        } else {
	    on_accept(error<client>(errno));
	    return true;
        }
    }

    on_accept(success(client(peer_fd)));

    // Return false allows to accept many connections by one event
    // So the only way to stop listening is to use cancel.
    return false;
}

bool connect_event::handle()
{
    if (io_event::handle()) return true;

    sockaddr_in sock_addr = addr.get_sockaddr();
    socklen_t size = sizeof(sock_addr);
    int res = connect(fd, (sockaddr*) &sock_addr, size);
    if (res < 0) {
        if (errno == EINPROGRESS) {
            return false;
        } else {
	    on_connect(error<nothing>(errno));
	    return false;
        }
    }

    on_connect(success());
    return true;
}

uint32_t read_event::events_flag()
{
    return EPOLLIN;
}

uint32_t read_some_event::events_flag()
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

void io_event::cancel()
{
    cancelled = true;
}

io_event::io_event()
    : cancelled(false)
{

}
