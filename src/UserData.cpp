
#include <iostream>
#include <cstring>

#include "UserData.hpp"

using namespace std;

UserData::UserData(string &username, string &password): username(username), password(password) {}

UserData::UserData(const unsigned char *raw_data)
{
	inflate(raw_data);
}

void UserData::inflate(const unsigned char *raw_data)
{
	username = string(reinterpret_cast<const char *>(raw_data), (size_t)MAX_FIELD_SIZE);
        raw_data += MAX_FIELD_SIZE;
	password = string(reinterpret_cast<const char *>(raw_data), MAX_FIELD_SIZE);
}

void UserData::serialize(unsigned char *buf)
{
	char padded_field[MAX_FIELD_SIZE];

	// username
        memset(padded_field, 0, sizeof(padded_field));
	strncpy(padded_field, username.c_str(), username.length() + 1);
        buf = serialize_char_array(buf, padded_field, sizeof(padded_field));
	// password
	memset(padded_field, 0, sizeof(padded_field));
	strncpy(padded_field, password.c_str(), password.length() + 1);
        buf = serialize_char_array(buf, padded_field, sizeof(padded_field));
}

int UserData::size()
{
	return MAX_FIELD_SIZE + MAX_FIELD_SIZE;
}

string UserData::getUsername()
{
	return username;
}

string UserData::getPassword()
{
	return password;
}

