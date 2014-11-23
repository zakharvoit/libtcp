#ifndef SERVER_HH
#define SERVER_HH

#include "tcp/util/address.hh"
#include "tcp/util/async_value.hh"
#include "tcp/util/job.hh"
#include "tcp/async/io_service.hh"

#include <functional>

namespace tcp
{
	namespace async
	{
		struct server
		{
			typedef std::function<void(util::async_value<client>)> on_accept_cb;
			server(util::address const&);

			util::job listen(io_service const&, on_accept_cb);

			void close();
		};
	}
}

#endif
