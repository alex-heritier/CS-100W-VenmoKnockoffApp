
#ifndef PAY_DATA_HPP
#define PAY_DATA_HPP

#include <iostream>

#include "Serializable.hpp"

class PayData : public Serializable {
public:
        PayData(std::string &, std::string &, int);
        PayData(const unsigned char *); // inflation constructor
        void inflate(const unsigned char *);
        void serialize(unsigned char *);
        int size();
        std::string getSender();
	std::string getSendee();
        int getAmount();
private:
        std::string sender;
	std::string sendee;
	int amount;
};

#endif
