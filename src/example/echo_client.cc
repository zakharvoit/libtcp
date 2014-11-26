#include <tcp/tcp.hh>
#include <bits/stdc++.h>

using namespace std;
using namespace tcp;
using namespace tcp::util;


struct echo_client
{

    echo_client(address const& addr)
    {
        client.connect(service, addr, [&](maybe<nothing>&& e) {
            e.get();
            this->on_connect();
        });
        service.start();
    }

    void on_connect()
    {
        char c;
        cin >> noskipws >> c;
        client.write(service, buffer(&c, 1), [=](maybe<nothing>&& e) {
            e.get();
            this->on_write(c);
        });
    }

    void on_write(char written)
    {
        if (written == '-') {
            service.stop();
        } else {
            char c;
            cin >> c;
            client.write(service, buffer(&c, 1), [=](maybe<nothing>&& e) {
                e.get();
                this->on_write(c);
            });
        }
    }

private:
    async::io_service service;
    async::client client;
};

int main()
{
    try {
        echo_client(util::address("127.0.0.1:33333"));
    } catch (exception const& e) {
        cerr << e.what() << endl;
    }

    return 0;
}