/*
 * NotEnoughFundsException.cpp
 *
 *  Created on: Oct 15, 2016
 *      Author: Example
 */

#include "NotEnoughFundsException.hpp"
#include "helper_funs.hpp"

/**
 * Constructs an exception from the given parameters
 * @param amountPulled the amount that the user attempted to deduct in cents
 * @param amountPresent the amount that the user has in their balance in cents
 */
NotEnoughFundsException::NotEnoughFundsException(int amountPulled, int amountPresent) : amountPulled(amountPulled), amountPresent(amountPresent)
{
	// TODO Auto-generated constructor stub

}

NotEnoughFundsException::~NotEnoughFundsException() {
	// TODO Auto-generated destructor stub
}

/**
 * @return the amount the user attempted to deduct in cents
 */
int NotEnoughFundsException::getAmountPulled() const
{
	return amountPulled;
}

/**
 * @return the amount the user actually has in their account in cents
 */
int NotEnoughFundsException::getAmountPresent() const
{
	return amountPresent;
}

std::ostream& operator <<(std::ostream& out, const NotEnoughFundsException& ex)
{

	out << "Present amount " << dollarString(ex.getAmountPresent()) << " is less than pulled amount " << dollarString(ex.getAmountPulled());
	return out;
}
