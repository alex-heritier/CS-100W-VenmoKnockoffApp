/*
 * FundSource.hpp
 *
 *  Created on: Oct 15, 2016
 *      Author: Example
 */

#ifndef FUNDSOURCE_HPP_
#define FUNDSOURCE_HPP_

#include <string>

using std::string;

//Information about a user's source of funds
class FundSource {
public:
	FundSource(string company);
	virtual ~FundSource();
	virtual string toString() const;
	string getCompany() const;
private:
	string company;
};

#endif /* FUNDSOURCE_HPP_ */
