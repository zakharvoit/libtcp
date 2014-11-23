#include "tcp/util/address.hh"

#include <stdexcept>
#include <algorithm>

using namespace std;
using namespace tcp::util;

/**
 * Parses address of the following format:
 *
 * address:port
 * where port is integer from interval [0, 65535]
 *       address specified by inet_aton (man 3 inet_aton)
 */
static void make_address(string const& addr_s, struct sockaddr_in& addr)
{
	auto colon_it = find(addr_s.begin(), addr_s.end(), ':');
	if (colon_it == addr_s.end()) throw invalid_argument(addr_s + " has no colon.");
	addr.sin_port = htons((uint16_t) stoul(string(colon_it + 1, addr_s.end())));

	auto addr_base_part = string(addr_s.begin(), colon_it).c_str();
	if (!inet_aton(addr_base_part, &addr.sin_addr)) throw invalid_argument("Can't parse base part: " + addr_s);
}

address::address(string const& addr)
{
	sockaddr.sin_family = AF_INET;
	make_address(addr, sockaddr);
}
uint32_t address::get_ip_addr() const
{return sockaddr.sin_addr.s_addr;}
uint16_t address::get_port() const
{return ntohs(sockaddr.sin_port);}
sockaddr_in address::get_sockaddr() const
{return sockaddr;}