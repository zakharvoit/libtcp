#ifndef EPOLL_HH
#define EPOLL_HH

#include <functional>

namespace tcp
{
	namespace async
	{
		namespace c
		{
			struct epoll
			{
				epoll();

				void add(int fd, uint32_t events, std::function<void()> cb);
				void rem(int fd, uint32_t events);
			};
		}
	}
}

#endif
