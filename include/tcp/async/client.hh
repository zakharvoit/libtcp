#ifndef CLIENT_HH
#define CLIENT_HH

#include "tcp/util/address.hh"
#include "tcp/util/async_value.hh"
#include "tcp/util/nothing.hh"
#include "tcp/async/io_service.hh"
#include "tcp/async/io_event.hh"

#include <functional>

namespace tcp
{
	namespace async
	{
		struct client
		{
			client();
            client(int fd) : fd(fd) {} // TODO: this is stub
			void connect(io_service&, util::address const&, on_connect_cb);
			void read(io_service&, size_t count, on_read_cb);
			void write(io_service&, util::buffer, on_write_cb);
			void close();

        private:
            int fd;
		};
	}
}

#endif
