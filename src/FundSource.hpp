/*
 * FundSource.hpp
 *
 *  Created on: Oct 15, 2016
 *      Author: Example
 */

#ifndef FUNDSOURCE_HPP_
#define FUNDSOURCE_HPP_

#include <string>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

using std::string;

//Information about a user's source of funds
class FundSource {
public:
	FundSource();
	FundSource(string company);
	virtual ~FundSource();
	virtual string toString() const;
	string getCompany() const;
private:
	friend class boost::serialization::access;
	template<typename Archive>
	void serialize(Archive &ar, const unsigned int version)
	{
		ar & company;
	}
	string company;
};

#endif /* FUNDSOURCE_HPP_ */
