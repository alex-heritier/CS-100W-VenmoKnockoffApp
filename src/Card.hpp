/*
 * Card.hpp
 *
 *  Created on: Oct 15, 2016
 *      Author: Example
 */

#ifndef CARD_HPP_
#define CARD_HPP_

#include "FundSource.hpp"
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

/**
 * A fundsource from a credit card, debit card, etc
 */
namespace vkastd {

class Card: public FundSource {
public:
	Card();
	Card(string company, string cardType, int cardNum);
	virtual ~Card();
	virtual string toString() const;
	string getCardType() const;
	int getCardNum() const;
private:
	string cardType;
	int cardNum;

	friend class boost::serialization::access;
	template<typename Archive>
	void serialize(Archive &ar, const unsigned int version)
	{
		ar & boost::serialization::base_object<FundSource>(*this);
		ar & cardType;
		ar & cardNum;
	}
};
}
#endif /* CARD_HPP_ */
