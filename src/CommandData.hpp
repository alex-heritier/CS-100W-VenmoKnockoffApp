
/**
 * CommandData.hpp
 * A collection of easily serializable structs for communicating between the client and server
 */

#ifndef COMMAND_DATA_HPP
#define COMMAND_DATA_HPP

#include <iostream>

// Using this instead of an enum because this can be compared against char's without casting
namespace CommandType {
	const char CREATE_USER = '0';
	const char LOGIN = '1';
	const char PAY = '2';
	const char ADD_FUNDS = '3';
}

const unsigned int FIELD_SIZE = 32;	// max size for a single data field (ex. username, password, etc)
const unsigned int MAX_PAYLOAD_SIZE = 1024; // max total size for a command data struct

// data for create_user and login commands
struct user_data {
        char username[FIELD_SIZE];
        char password[FIELD_SIZE];
};

// data for pay command
struct pay_data {
	char username[FIELD_SIZE];
	char message[FIELD_SIZE];
	unsigned int amount;
};

// data for add_funds command
struct add_funds_data {
        char username[FIELD_SIZE];
        char fund_tag[FIELD_SIZE];
        unsigned int amount;
};

// serializable user data
struct user_data_serial {
        char type;
        struct user_data user;
};

// serializable pay data
struct pay_data_serial {
	char type;
	struct pay_data pay;
};

// serializable add_funds data
struct add_funds_data_serial {
	char type;
	struct add_funds_data add_funds;
};

unsigned char *serialize_int(unsigned char *buf, int value)
{
	buf[0] = value >> 24;
	buf[1] = value >> 16;
	buf[2] = value >> 8;
	buf[3] = value;
	return buf + 4;
}

unsigned char *serialize_char(unsigned char *buf, char value)
{
	buf[0] = value;
	return buf + 1;
}

unsigned char *serialize_char_array(unsigned char *buf, char *value, int length)
{
	memcpy(buf, value, length);
	return buf + length;
}

int deserialize_int(unsigned char *buf)
{
	int result = 0;
	result |= *(buf) << 24;
        result |= *(buf + 1) << 16;
        result |= *(buf + 2) << 8;
        result |= *(buf + 3);
	return result;
}

void serialize_user_data(unsigned char *buf, struct user_data_serial *value)
{
	buf = serialize_char(buf, value->type);
	buf = serialize_char_array(buf, value->user.username, sizeof(value->user.username));
	buf = serialize_char_array(buf, value->user.password, sizeof(value->user.password));	
}

void deserialize_user_data(struct user_data *dest, unsigned char *buf)
{
	memcpy(&dest->username, buf, sizeof(dest->username));
	buf += sizeof(dest->username);
	memcpy(&dest->password, buf, sizeof(dest->password));
}

void serialize_pay_data(unsigned char *buf, struct pay_data_serial *value)
{
        buf = serialize_char(buf, value->type);
        buf = serialize_char_array(buf, value->pay.username, sizeof(value->pay.username));
        buf = serialize_int(buf, value->pay.amount);
}

void deserialize_pay_data(struct pay_data *dest, unsigned char *buf)
{
        memcpy(&dest->username, buf, sizeof(dest->username));
        buf += sizeof(dest->username);
	int amount_buf = deserialize_int(buf);
	std::cout << "amount_buf is " << amount_buf << std::endl;
	dest->amount = amount_buf;
}

void serialize_add_funds_data(unsigned char *buf, struct add_funds_data_serial *value)
{
        buf = serialize_char(buf, value->type);
        buf = serialize_char_array(buf, value->add_funds.fund_tag, sizeof(value->add_funds.fund_tag));
        buf = serialize_int(buf, value->add_funds.amount);
}

void deserialize_add_funds_data(struct add_funds_data *dest, unsigned char *buf)
{
        memcpy(&dest->fund_tag, buf, sizeof(dest->fund_tag));
        buf += sizeof(dest->fund_tag);
        int amount_buf = deserialize_int(buf);
        std::cout << "amount_buf is " << amount_buf << std::endl;
        dest->amount = amount_buf;
}

#endif
