#ifndef SERVICE_HH
#define SERVICE_HH

#include "tcp/async/c/epoll.hh"
#include "tcp/async/io_event.hh"

#include <unordered_map>
#include <functional>
#include <list>

namespace tcp
{
    namespace async
    {
        struct io_service
        {
            io_service();

            void start();
            void stop();

        private:
            c::epoll e;
            std::unordered_map <int, std::list <io_event*> > callbacks;

            void add_event(int fd, io_event* ev);

            void on_ready(int fd, uint32_t events);

            friend struct client;
            friend struct server;
        };
    }
}

#endif
