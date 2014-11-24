#include "tcp/async/c/epoll.hh"

#include <gtest/gtest.h>

#include <sys/socket.h>
#include <sys/epoll.h>
#include <unistd.h>

#include <array>

using namespace std;
using namespace tcp::async::c;

TEST(epoll, storage)
{
    static const size_t count = 100;

    epoll e([](int, uint32_t) {});

    array <int, count> socks;
    for (int& sock : socks) {
        sock = socket(AF_INET, SOCK_STREAM, 0);
        e.add(sock, 0);
    }

    for (int& sock : socks) {
        ASSERT_TRUE(e.contains(sock));
    }

    for (int& sock : socks) {
        e.add_events(sock, EPOLLIN | EPOLLOUT);
    }

    for (int& sock : socks) {
        ASSERT_TRUE(e.get_events(sock) == (EPOLLIN | EPOLLOUT));
    }

    for (int& sock : socks) {
        e.remove_events(sock, EPOLLIN);
    }

    for (int& sock : socks) {
        ASSERT_TRUE(e.get_events(sock) == EPOLLOUT);
    }

    for (int& sock : socks) {
        close(sock);
        e.remove(sock);
    }

    for (int& sock : socks) {
        ASSERT_FALSE(e.contains(sock));
    }
}