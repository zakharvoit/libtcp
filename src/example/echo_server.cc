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
		clients.push_front(move(c));
		auto it = clients.begin();
		auto& cl = *it;
        cl.read(service, 1, [=](maybe<buffer>&& b) {
			this->on_read(it, **(b.get()));
        });
    }

    void on_read(list<async::client>::iterator it, char c)
    {
		auto& cl = *it;
        cout << c << flush;
		cl.read(service, 1, [=](maybe<buffer>&& bm) {
			try {
			    auto b = bm.get();
				this->on_read(it, **b);
			} catch (...) {
				clients.erase(it);
				if (clients.empty()) {
					service.stop();
				}
				return;
			}
        });
    }

private:
    canceller listening;
    async::io_service service;
    async::server s;
    list<async::client> clients;
};

int main()
{
    try {
        echo_server(util::address("127.0.0.1:33333"));
    } catch (exception* e) {
        cerr << e->what() << endl;
		delete e;
    } catch (exception const& e) {
		cerr << e.what() << endl;
	}

    return 0;
}
