#ifndef BUFFER_HH
#define BUFFER_HH

#include <cstddef>
#include <string>

namespace tcp
{
	namespace util
	{
		struct buffer
		{
			buffer(void*, size_t count);
			buffer(size_t count);
			buffer(buffer&&);
			buffer(buffer const&);
			buffer(std::string const& s)
				: buffer((char*) s.c_str(), s.length() + 1)
			{}

			~buffer();

			buffer& operator=(buffer const&);
			buffer& operator=(buffer&&);

			void reset();
			size_t rest_length() const;
			char* operator*() const;
			void operator+=(int x);

		private:
			char* data;
			char* begin;
			size_t count;
		};
	}
}

#endif
