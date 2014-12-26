#include "tcp/async/io_service.hh"

#include <sys/epoll.h>

using namespace std;

using namespace tcp::async;

io_service::io_service()
	: e([&](int fd, uint32_t events) {
			this->on_ready(fd, events);
		})
{
}

void io_service::start()
{
	e.start();
}

void io_service::stop()
{
	e.stop();
}

void io_service::on_ready(int fd, uint32_t)
{
	auto& cb = callbacks[fd];

	uint32_t epoll_flags = 0;
	uint32_t new_epoll_flags = 0;
	for (auto it = cb.begin(); it != cb.end();) {
		epoll_flags |= (*it)->events_flag();
		if ((*it)->handle()) {
			delete *it;
			it = cb.erase(it);
		} else {
			new_epoll_flags |= (*it)->events_flag();
			it++;
		}
	}

	if (cb.empty()) {
		callbacks.erase(fd);
		e.remove(fd);
	} else {
		uint32_t removed_flags = epoll_flags & ~new_epoll_flags;
		if (removed_flags) {
			e.remove_events(fd, removed_flags);
		}
	}
}

void io_service::add_event(int fd, io_event* ev)
{
	if (e.contains(fd)) {
		e.add_events(fd, ev->events_flag());
	} else {
		e.add(fd, ev->events_flag());
	}

	callbacks[fd].push_back(ev);
}
