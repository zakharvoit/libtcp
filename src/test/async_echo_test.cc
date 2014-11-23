#include <gtest/gtest.h>

#include <tcp/async/client.hh>
#include <tcp/async/server.hh>
#include <tcp/async/io_service.hh>
#include <tcp/util/address.hh>

#include <iostream>
#include <ldap.h>

using namespace std;
using namespace tcp;

TEST(tcp_async, echo)
{
    string message = "Hello, World!";
    util::address addr = util::address("127.0.0.1:33332");

    async::io_service service;

    async::server server(addr);

    async::client client1;
    server.listen(service, [&](int fd)
    {
        client1 = fd;
        cerr << "Accepted" << endl;
        client1.write(service,
                util::buffer((void*) message.c_str(),
                        message.length() + 1),
                [&]() {
                    cerr << "Written " << message << endl;
                });
    });

    async::client client;
    client.connect(service, addr, [&]()
    {
        cerr << "Connected" << endl;
        client.read(service, message.length() + 1, [&](util::buffer b)
        {
            cerr << "Read " << *b << endl;
        });
    });

    service.start();
}