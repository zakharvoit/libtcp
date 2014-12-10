#include "tcp/util/error.hh"

using namespace tcp::util;

void error_code::raise()
{
    throw tcp_exception(*this);
}
