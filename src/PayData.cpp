
#include <iostream>
#include <cstring>
#include "PayData.hpp"
#include "CommandData.hpp"

using namespace std;

PayData::PayData(string &sender, string &sendee, int amount): sender(sender), sendee(sendee), amount(amount) {}

PayData::PayData(const unsigned char *raw_data)
{
	inflate(raw_data);
}

void PayData::inflate(const unsigned char *raw_data)
{
	sender = string(reinterpret_cast<const char *>(raw_data), FIELD_SIZE);
	raw_data += FIELD_SIZE;
	sendee = string(reinterpret_cast<const char *>(raw_data), FIELD_SIZE);
        raw_data += FIELD_SIZE;
	amount = deserialize_int(raw_data);
}

void PayData::serialize(unsigned char *buf)
{
	char padded_field[FIELD_SIZE];

        // sender
        memset(padded_field, 0, sizeof(padded_field));
        strncpy(padded_field, sender.c_str(), sender.length() + 1);
        buf = serialize_char_array(buf, padded_field, sizeof(padded_field));

	// sendee
	memset(padded_field, 0, sizeof(padded_field));
        strncpy(padded_field, sendee.c_str(),  sendee.length() + 1);
        buf = serialize_char_array(buf, padded_field, sizeof(padded_field));

        // amount
        buf = serialize_int(buf, amount);
}

int PayData::size()
{
	return FIELD_SIZE + FIELD_SIZE + sizeof(int);
}

string PayData::getSender()
{
	return sender;
}

string PayData::getSendee()
{
	return sendee;
}

int PayData::getAmount()
{
	return amount;
}
