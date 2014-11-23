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
            buffer(size_t count);
            buffer(buffer&&);
            buffer(buffer const&);

            ~buffer();

            buffer& operator=(buffer const&);
            void reset();
            size_t rest_length() const;
            char* operator*();
            void operator+=(int x);

        private:
            char* data;
            char* begin;
            size_t count;
        };
    }
}

#endif
