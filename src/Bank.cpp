/*
 * Bank.cpp
 *
 *  Created on: Oct 15, 2016
 *      Author: Example
 */

#include "Bank.hpp"

#include <sstream>

using std::stringstream;

/**
 * Constructs a bank source with the given company and account number.
 * @param company the company to which the bank belongs
 * @param accountNum the bank account number
 */
Bank::Bank(string company, int accountNum) : FundSource(company), accountNum(accountNum)
{
	// TODO Auto-generated constructor stub

}

Bank::~Bank() {
	// TODO Auto-generated destructor stub
}

/**
 * @return company information and the bank account number
 */
string Bank::toString() const
{
	stringstream info;
	info << FundSource::toString() << "\n";
	info << "Account number: " << accountNum;
	return info.str();
}

/**
 * @return the bank account number
 */
int Bank::getAccountNum() const
{
	return accountNum;
}
