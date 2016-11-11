/*
 * Bank.hpp
 *
 *  Created on: Oct 15, 2016
 *      Author: Example
 */

#ifndef BANK_HPP_
#define BANK_HPP_

#include "FundSource.hpp"
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

//A fund source specifies a bank
class Bank: public FundSource {
public:
	Bank();
	Bank(string company, int accountNum);
	int getAccountNum() const;
	virtual ~Bank();
	virtual string toString() const;
private:
	friend class boost::serialization::access;
	template<typename Archive>
	void serialize(Archive &ar, const unsigned int version)
	{
		ar & boost::serialization::base_object<FundSource>(*this);
		ar & accountNum;
	}


	int accountNum;
};

#endif /* BANK_HPP_ */
