#ifndef CANCELLER_HH
#define CANCELLER_HH

#include "tcp/async/io_event.hh"

namespace tcp
{
    namespace util
    {
        struct canceller
        {
            canceller() = default;

            void cancel();

        private:
            async::io_event* event;

            canceller(async::io_event*);

            friend canceller make_canceller(async::io_event*);
        };

        canceller make_canceller(async::io_event*);
    }
}

#endif
