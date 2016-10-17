/*
 * Bank.hpp
 *
 *  Created on: Oct 15, 2016
 *      Author: Example
 */

#ifndef BANK_HPP_
#define BANK_HPP_

#include "FundSource.hpp"

//A fund source specifies a bank
class Bank: public FundSource {
public:
	Bank(string company, int accountNum);
	int getAccountNum() const;
	virtual ~Bank();
	virtual string toString() const;
private:
	int accountNum;
};

#endif /* BANK_HPP_ */
