#include "tcp/util/address.hh"

#include <gtest/gtest.h>

using namespace std;
using namespace tcp::util;

TEST(address, correctness)
{
    address addr("127.0.0.1:8080");
    ASSERT_EQ(addr.get_ip_addr(), inet_addr("127.0.0.1"));
    ASSERT_EQ(addr.get_port(), 8080);

    address addr_hex("0xFF.0xFF.0xFF.0xFF:8080");
    ASSERT_EQ(addr_hex.get_ip_addr(), -1);
}

TEST(address, exceptions)
{
    ASSERT_THROW(address("127.0.0.1"), invalid_argument); // No port
    ASSERT_THROW(address("127.0.0.0.1:8080"), invalid_argument); // Too many dots
    ASSERT_THROW(address("-10.0.0.0:8080"), invalid_argument); // Negative address
    ASSERT_THROW(address("256.0.0.0:8080"), invalid_argument); // Too big number

    // Next statements result are determined by BSD sockets functions,
    // but they shouldn't fail
    ASSERT_NO_THROW(address("10:8080"));
    ASSERT_NO_THROW(address("10.10:8080"));
    ASSERT_NO_THROW(address("10.10.10:8080"));
    ASSERT_NO_THROW(address("10.10.10.10:8080"));
    ASSERT_NO_THROW(address("10.10.10.10:8080"));
    ASSERT_NO_THROW(address("127.0.0.0:65536"));
}
