// TODO: Handle errors on all epoll_ctl calls

#include "tcp/async/c/epoll.hh"

#include <sys/epoll.h>
#include <sys/eventfd.h>

#include <cerrno>
#include <cstring>
#include <stdexcept>
#include <string>

using namespace std;
using namespace tcp::async::c;

static const size_t EVENTS_SIZE = 256;

static epoll_event make_event(int fd, uint32_t events)
{
    epoll_event ev;
    ev.data.fd = fd;
    ev.events = events;
    return ev;
};

epoll::epoll(on_fd_ready_cb cb)
        : efd(epoll_create(1)),
          on_ready(cb)
{
    if (efd < 0) {
        throw runtime_error(strerror(errno));
    }

    stop_event_fd = eventfd(0, 0);
    if (stop_event_fd < 0) {
        throw runtime_error(strerror(errno));
    }

    auto ev = make_event(stop_event_fd, EPOLLIN);
    epoll_ctl(efd, EPOLL_CTL_ADD, stop_event_fd, &ev);
}

void epoll::add(int fd, uint32_t events)
{
    if (data.count(fd)) {
        throw logic_error(to_string(fd) + " file descriptor was already added.");
    }

    data[fd] = events;
    epoll_event ev = make_event(fd, events);
    if (epoll_ctl(efd, EPOLL_CTL_ADD, fd, &ev) < 0) {
        throw logic_error(strerror(errno));
    }
}

void epoll::remove(int fd)
{
    epoll_ctl(efd, EPOLL_CTL_DEL, fd, nullptr);
    data.erase(fd);
}

uint32_t epoll::get_events(int fd) const
{
    auto it = data.find(fd);
    if (it != data.end()) {
        return it->second;
    } else {
        throw logic_error(to_string(fd) + " file descriptor wasn't added.");
    }
}

void epoll::add_events(int fd, uint32_t events)
{
    auto it = data.find(fd);
    if (it != data.end()) {
        it->second |= events;
        epoll_event ev = make_event(fd, it->second);
        epoll_ctl(efd, EPOLL_CTL_MOD, fd, &ev);
    } else {
        throw logic_error(to_string(fd) + " file descriptor wasn't added.");
    }
}

void epoll::remove_events(int fd, uint32_t events)
{
    auto it = data.find(fd);
    if (it != data.end()) {
        it->second &= ~events;
        epoll_event ev = make_event(fd, it->second);
        epoll_ctl(efd, EPOLL_CTL_MOD, fd, &ev);
    } else {
        throw logic_error(to_string(fd) + " file descriptor wasn't added.");
    }
}

void epoll::start()
{
    epoll_event events[EVENTS_SIZE];

    bool running = true;
    while (running) {
        int new_events = epoll_wait(efd, events, EVENTS_SIZE, -1);
        if (new_events < 0) {
            throw logic_error(strerror(errno));
        }

        for (int i = 0; i < new_events; i++) {
            if (events[i].data.fd == stop_event_fd) {
                running = false;
                continue;
            }
            on_ready(events[i].data.fd, events[i].events);
        }
    }
}

bool epoll::contains(int fd) const
{
    return data.find(fd) != data.end();
}
void epoll::stop()
{
    eventfd_write(stop_event_fd, 1);
}
