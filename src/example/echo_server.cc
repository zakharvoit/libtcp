#include <tcp/tcp.hh>
#include <bits/stdc++.h>

using namespace std;
using namespace tcp;

struct echo_server
{
    echo_server(util::address const& addr)
            : s(addr)
    {
        s.listen(service, [=](async::client&& c) { this->on_accept(move(c)); });
        service.start();
    }

    void on_accept(async::client&& c)
    {
        this->c = move(c);
        this->c.read(service, 1, [=](util::buffer b) { this->on_read(**b); });
    }

    void on_read(char c)
    {
        cout << c << flush;
        if (c == '-') {
            service.stop();
            return;
        }

        this->c.read(service, 1, [=](util::buffer b) { this->on_read(**b); });
    }

private:
    async::io_service service;
    async::server s;
    async::client c;
};

int main()
{
    try {
        echo_server(util::address("127.0.0.1:33333"));
    } catch (exception const& e) {
        cerr << e.what() << endl;
    }

    return 0;
}
