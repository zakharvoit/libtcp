#ifndef EPOLL_HH
#define EPOLL_HH

#include <functional>
#include <map>

namespace tcp
{
    namespace async
    {
        namespace c
        {
            struct epoll
            {
                typedef std::function <void(int, uint32_t)> on_fd_ready_cb;

                epoll(on_fd_ready_cb);

                bool contains(int fd) const;
                void add(int fd, uint32_t events);
                void remove(int fd);

                uint32_t get_events(int fd) const;
                void add_events(int fd, uint32_t events);
                void remove_events(int fd, uint32_t events);

                void start();
                void stop();

            private:
                int efd;
                int stop_event_fd;

                on_fd_ready_cb on_ready;
                std::map <int, uint32_t> data;
            };
        }
    }
}

#endif
