
#ifndef ADD_FUND_SOURCE_DATA
#define ADD_FUND_SOURCE_DATA

#include <iostream>

#include "Serializable.hpp"

class AddFundSourceData : public Serializable {
public:
	AddFundSourceData(std::string &, std::string &, std::string &, std::string &);
        AddFundSourceData(const unsigned char *); // inflation constructor
        void inflate(const unsigned char *);
        void serialize(unsigned char *);
        int size();
	std::string getUsername();
        std::string getCompany();
        std::string getFundID();
        std::string getCardType();
private:
	std::string username;
        std::string company;
        std::string fundID;
        std::string cardType;
};

#endif
