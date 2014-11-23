#ifndef ADDRESS_HH
#define ADDRESS_HH

#include <netinet/in.h>
#include <arpa/inet.h>

#include <cstdint>
#include <string>

namespace tcp
{
    namespace util
    {
        struct address
        {
            address(std::string const& addr);

            uint32_t get_ip_addr() const;
            uint16_t get_port() const;

            sockaddr_in get_sockaddr() const;

        private:
            sockaddr_in sockaddr;
        };
    }
}

#endif
