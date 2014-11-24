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

    bool end = false;
	for (auto it = cb.begin(); it != cb.end();) {
		if ((*it)->handle()) {
            end = true;
			delete *it;
			it = cb.erase(it);
		}
		else it++;
	}

    if (end) {
        // TODO: No cleanup, need to find the way how to do it
//        callbacks.erase(fd);
//        e.remove(fd);
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
