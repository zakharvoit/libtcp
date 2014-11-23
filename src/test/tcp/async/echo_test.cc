#include "tcp/tcp.hh"

#include <gtest/gtest.h>

using namespace std;
using namespace tcp;

TEST(general, echo)
{
    static const string message = "Hello, World!";
    static const util::address addr("127.0.0.1:33333");

    async::io_service service;
    async::server server(addr);
    async::client peer;
    async::client client;

    server.listen(service, [&](int fd)
    {
        peer = fd;
        peer.write(service, util::buffer((void*) message.c_str(), message.length() + 1), [&]()
        {
            peer.read(service, message.length() + 1, [&](util::buffer buf)
            {
                ASSERT_EQ(string(*buf), message);
                service.stop();
            });
        });
    });

    client.connect(service, addr, [&]()
    {
        client.read(service, message.length() + 1, [&](util::buffer buf)
        {
            ASSERT_EQ(string(*buf), message);
            client.write(service, util::buffer((void*) message.c_str(), message.length() + 1), [&](){});
        });
    });

    service.start();
}
