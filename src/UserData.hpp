
#ifndef USER_DATA_HPP
#define USER_DATA_HPP

#include <iostream>

#include "Serializable.hpp"

class UserData : public Serializable {
public:
	UserData(std::string &, std::string &);
	UserData(const unsigned char *);	// inflation constructor
	void inflate(const unsigned char *);
	void serialize(unsigned char *);
	int size();
	std::string getUsername();
	std::string getPassword();
private:
	std::string username;
	std::string password;
};

#endif
