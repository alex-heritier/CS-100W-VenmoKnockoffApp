/*
 * NotEnoughFundsException.hpp
 *
 *  Created on: Oct 15, 2016
 *      Author: Example
 */

#ifndef NOTENOUGHFUNDSEXCEPTION_HPP_
#define NOTENOUGHFUNDSEXCEPTION_HPP_

#include <iostream>
//Exception to be thrown when the user attempts to deduct more money than they have in their account.
class NotEnoughFundsException {
public:
	NotEnoughFundsException(int amountPulled, int amountPresent);
	virtual ~NotEnoughFundsException();
	int getAmountPulled() const;
	int getAmountPresent() const;
	friend std::ostream& operator <<(std::ostream& out, const NotEnoughFundsException& ex);
private:
	int amountPulled;
	int amountPresent;
};

#endif /* NOTENOUGHFUNDSEXCEPTION_HPP_ */
