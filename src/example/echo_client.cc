#include <tcp/tcp.hh>
#include <bits/stdc++.h>

using namespace std;
using namespace tcp;

struct echo_client
{
    echo_client(util::address const& addr)
    {
        client.connect(service, addr, [&]() { this->on_connect(); });
        service.start();
    }

    void on_connect()
    {
        char c;
        cin >> noskipws >> c;
        client.write(service, util::buffer(&c, 1), [=]()
        {
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
            client.write(service, util::buffer(&c, 1), [=]() {
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