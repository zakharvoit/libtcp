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

    server.listen(service,
            [&](util::maybe<async::client>&& c) {
        peer = c.get();
        peer.write(service, util::buffer((void*) message.c_str(), message.length() + 1),
                [&](util::maybe<util::nothing>&& e) {
            e.get();
            peer.read(service, message.length() + 1, [&](util::maybe<util::buffer>&& mbuf)
            {
                auto buf = mbuf.get();
                ASSERT_EQ(message, string(*buf));
                service.stop();
            });
        });
    });

    client.connect(service, addr,
            [&](util::maybe<util::nothing>&& e) {
        e.get();
        client.read(service, message.length() + 1,
                [&](util::maybe<util::buffer>&& mbuf) {
            auto buf = mbuf.get();
            ASSERT_EQ(message, string(*buf));
            client.write(service, util::buffer((void*) message.c_str(), message.length() + 1),
                    [&](util::maybe<util::nothing> e){ ASSERT_NO_THROW(e.get()); });
        });
    });

    service.start();
}
