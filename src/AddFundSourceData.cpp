
#include "AddFundSourceData.hpp"
#include "CommandData.hpp"

using namespace std;

AddFundSourceData::AddFundSourceData(string &username, string &company, string &fundID, string &cardType):
	username(username), company(company), fundID(fundID), cardType(cardType) {}

AddFundSourceData::AddFundSourceData(const unsigned char *raw_data)
{
        inflate(raw_data);
}

void AddFundSourceData::inflate(const unsigned char *raw_data)
{
        company = string(reinterpret_cast<const char *>(raw_data), FIELD_SIZE);
        raw_data += FIELD_SIZE;
        fundID = string(reinterpret_cast<const char *>(raw_data), FIELD_SIZE);
        raw_data += FIELD_SIZE;
        cardType = string(reinterpret_cast<const char *>(raw_data), FIELD_SIZE);
}

void AddFundSourceData::serialize(unsigned char *buf)
{
        char padded_field[FIELD_SIZE];

        // company
        memset(padded_field, 0, sizeof(padded_field));
        strncpy(padded_field, company.c_str(), company.length() + 1);
        buf = serialize_char_array(buf, padded_field, sizeof(padded_field));

        // fund ID
        memset(padded_field, 0, sizeof(padded_field));
        strncpy(padded_field, fundID.c_str(), fundID.length() + 1);
        buf = serialize_char_array(buf, padded_field, sizeof(padded_field));

        // card type
	memset(padded_field, 0, sizeof(padded_field));
        strncpy(padded_field, cardType.c_str(), cardType.length() + 1);
        buf = serialize_char_array(buf, padded_field, sizeof(padded_field));
}

int AddFundSourceData::size()
{
        return FIELD_SIZE + FIELD_SIZE + FIELD_SIZE;
}

string AddFundSourceData::getUsername()
{
	return username;
}

string AddFundSourceData::getCompany()
{
        return company;
}

string AddFundSourceData::getFundID()
{
        return fundID;
}

string AddFundSourceData::getCardType()
{
        return cardType;
}

