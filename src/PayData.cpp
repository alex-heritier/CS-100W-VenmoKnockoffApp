
#include <iostream>
#include <cstring>
#include "PayData.hpp"
#include "CommandData.hpp"

using namespace std;

PayData::PayData(string &username, int amount): username(username), amount(amount) {}

PayData::PayData(const unsigned char *raw_data)
{
	inflate(raw_data);
}

void PayData::inflate(const unsigned char *raw_data)
{
	username = string(reinterpret_cast<const char *>(raw_data), FIELD_SIZE);
        raw_data += FIELD_SIZE;
	amount = deserialize_int(raw_data);
}

void PayData::serialize(unsigned char *buf)
{
	char padded_field[FIELD_SIZE];

        // username
        memset(padded_field, 0, sizeof(padded_field));
        strncpy(padded_field, username.c_str(), username.length() + 1);
        buf = serialize_char_array(buf, padded_field, sizeof(padded_field));
        // amount
        buf = serialize_int(buf, amount);
}

int PayData::size()
{
	return FIELD_SIZE + sizeof(int);
}

string PayData::getUsername()
{
	return username;
}

int PayData::getAmount()
{
	return amount;
}
