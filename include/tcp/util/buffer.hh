#ifndef BUFFER_HH
#define BUFFER_HH

#include <cstddef>

namespace tcp
{
	namespace util
	{
		struct buffer
		{
			buffer(void*, size_t count);
		};
	}
}

#endif
