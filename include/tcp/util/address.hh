#ifndef ADDRESS_HH
#define ADDRESS_HH

#include <string>

namespace tcp
{
	struct address
	{
		address(std::string const&);
		address(std::string const&, uint16_t port);
		address(uint32_t addr, uint16_t port);
	};
}

#endif
