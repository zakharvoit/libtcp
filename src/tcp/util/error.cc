#include "tcp/util/error.hh"

using namespace tcp::util;

void error_code::raise() const
{
	throw tcp_exception(*this);
}
