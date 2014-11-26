#include <tcp/tcp.hh>
#include <bits/stdc++.h>

using namespace std;
using namespace tcp;
using namespace tcp::util;

struct echo_server
{

    echo_server(address const& addr)
            : s(addr)
    {
        listening = s.listen(service, [=](maybe<async::client>&& c) {
            this->on_accept(move(c.get()));
        });
        service.start();
    }

    void on_accept(async::client&& c)
    {
        listening.cancel();
        this->c = move(c);
        this->c.read(service, 1, [=](maybe<buffer>&& b) {
            this->on_read(**(b.get()));
        });
    }

    void on_read(char c)
    {
        cout << c << flush;
        if (c == '-') {
            service.stop();
            return;
        }

        this->c.read(service, 1, [=](maybe<buffer>&& b) {
            this->on_read(**(b.get()));
        });
    }

private:
    canceller listening;
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
