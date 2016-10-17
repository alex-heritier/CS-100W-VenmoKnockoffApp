/*
 * Card.cpp
 *
 *  Created on: Oct 15, 2016
 *      Author: Example
 */

#include "Card.hpp"
#include <sstream>

using std::stringstream;

/**
 * Constructs a card source from the given company name, card type, and card number
 * @param company the company that issued the card
 * @param cardType the type of card
 * @param cardNum the number on the card
 */
Card::Card(string company, string cardType, int cardNum) : FundSource(company), cardType(cardType), cardNum(cardNum)
{
	// TODO Auto-generated constructor stub

}

Card::~Card() {
	// TODO Auto-generated destructor stub
}

/**
 * @return the card's type
 */
string Card::getCardType() const
{
	return cardType;
}

/**
 * @return the card's number
 */
int Card::getCardNum() const
{
	return cardNum;
}

/**
 * @return the information about the card
 */
string Card::toString() const
{
	stringstream info;
	info << FundSource::toString() << "\n";
	info << "Card type: " << cardType << "\n";
	info << "Card number: " << cardNum;
	return info.str();
}
