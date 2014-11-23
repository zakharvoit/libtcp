#ifndef CLIENT_HH
#define CLIENT_HH

#include "tcp/util/address.hh"
#include "tcp/util/async_value.hh"
#include "tcp/util/nothing.hh"
#include "tcp/async/io_service.hh"

#include <functional>

namespace tcp
{
	namespace async
	{
		struct client
		{
			typedef std::function<void(util::async_value<util::nothing>)> on_connect_cb;
			typedef std::function<void(util::async_value<util::buffer>)> on_read_cb;
			typedef std::function<void(util::async_value<util::nothing>)> on_write_cb;

			client();

			job connect(io_service const&, address const&, on_connect_cb);

			job read(io_service const&, size_t count, on_read_cb);
			job write(io_service const&, util::buffer, on_write_cb);

			void close();
		};
	}
}

#endif
