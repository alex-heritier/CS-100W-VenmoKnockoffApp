/*
 * Card.hpp
 *
 *  Created on: Oct 15, 2016
 *      Author: Example
 */

#ifndef CARD_HPP_
#define CARD_HPP_

#include "FundSource.hpp"

/**
 * A fundsource from a credit card, debit card, etc
 */
class Card: public FundSource {
public:
	Card(string company, string cardType, int cardNum);
	virtual ~Card();
	virtual string toString() const;
	string getCardType() const;
	int getCardNum() const;
private:
	string cardType;
	int cardNum;
};

#endif /* CARD_HPP_ */
