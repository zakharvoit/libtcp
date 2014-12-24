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
				if (!c) c.raise();
				this->on_accept(move(c.get()));
			});
		service.start();
	}

	void on_accept(async::client&& c)
	{
		clients.push_front(move(c));
		auto it = clients.begin();
		auto& cl = *it;
		cl.read_some(service, [=](maybe<buffer>&& b) {
				if (!b) {
					handle_error(it);
					return;
				}
				this->on_read(it, b.get());
			});
	}

	void on_read(list<async::client>::iterator it, buffer b)
	{
		auto& cl = *it;
		while (b.rest_length()) {
			cout << **b;
			b += 1;
		}
		cout << flush;
		cl.read_some(service, [=](maybe<buffer>&& b) {
				if (!b) {
					handle_error(it);
					return;
				}
				this->on_read(it, b.get());
			});
	}

	void handle_error(list<async::client>::iterator const& it)
	{
		clients.erase(it);
		if (clients.empty()) {
			service.stop();
		}	
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
	} catch (exception const& e) {
		cerr << e.what() << endl;
	}
    
	return 0;
}
