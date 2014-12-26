#ifndef CLIENT_HH
#define CLIENT_HH

#include "tcp/util/address.hh"
#include "tcp/util/buffer.hh"
#include "tcp/util/canceller.hh"
#include "tcp/util/maybe.hh"
#include "tcp/util/nothing.hh"

#include <functional>

namespace tcp
{
	namespace async
	{
		using namespace util;
		using namespace std;
		struct io_service;
		
		struct client
		{
			client();
			~client();

			client(client const&) = delete;
			client(client&&);

			client& operator=(client&&);

			canceller connect(io_service&,
			                  address const&,
			                  function<void(maybe<nothing>&&)>);
			canceller read(io_service&,
			               size_t count,
			               function<void(maybe<buffer>&&)>);
			canceller write(io_service&,
			                util::buffer,
			                function<void(maybe<nothing>&&)>);

			int get_fd() const { return fd; }

		private:
			int fd;
			client(int fd)
				: fd(fd)
			{}

			friend struct accept_event;
		};
	}
}

#endif
