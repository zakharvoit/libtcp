#include "tcp/util/canceller.hh"
#include "tcp/async/io_event.hh"

using namespace tcp::util;
using namespace tcp::async;

canceller::canceller(async::io_event* event)
	: event(event)
{
}

void canceller::cancel()
{
	if (event) event->cancel();
}

canceller tcp::util::make_canceller(io_event* event)
{
	return event;
}
