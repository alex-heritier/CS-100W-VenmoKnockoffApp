/*
 * User.cpp
 *
 *  Created on: Oct 15, 2016
 *      Author: Example
 */

#include "User.hpp"
#include <sstream> //Makes it easy to build a string from several smaller strings and numbers.
#include "helper_funs.hpp"
#include <algorithm>

User::User() : username(""), balance(0)
{

}

/*
 * Constructs a new user with the given username and an empty balance.
 * @param username the username
 */
User::User(const string& username) : username(username), balance(0)
{
	// TODO Auto-generated constructor stub

}

/**
 * Constructs a new user with the given username and the given balance
 * @param username the username
 * @param balance the account balance in cents
 */
User::User(const string& username, int balance): username(username), balance(balance)
{

}

User::~User() {
	// TODO Auto-generated destructor stub
}

/**
 * @return a string containing the username.
 */
string User::getUsername() const
{
	return username;
}

/**
 * @return a string containing the username, balance, and an indexed list of outside sources of funds.
 */
string User::toString() const
{
	stringstream info;
	info << "Username: " << username << "\n";
	info << "Balance: " << dollarString(balance) << "\n";
	/*
	for(unsigned i = 0; i < funders.size(); i++)
	{
		info << "[" << i << "]\n" << funders[i]->toString() << "\n";
	}
	*/
	unsigned index = 0;
	for_each(funders.begin(), funders.end(), [&index, &info] (shared_ptr<FundSource> fund) {
		info << "[" << index << "]\n" << fund->toString() << "\n";
		index++;
	});
	return info.str();
}

/**
 * @param fundIndex the index of the fund source
 * @param amount the amount to be taken from the fund source in cents
 * @return a string that states how much money was taken from which source.
 */
string User::addFunds(int fundIndex, int amount)
{
	//Todo
	balance += amount;
	stringstream receipt;
	receipt << "Pulled " << dollarString(amount) << " from\n" << funders[fundIndex]->toString();
	return receipt.str();
}

/**
 * @return the current account balance in cents
 */
int User::getBalance() const
{
	return balance;
}

/**
 * Return the number of fund sources.
 */
int User::getFundSize() const
{
	return funders.size();
}

/**
 * Deducts the given amount from the account balance
 * @param amount the cent amount to be removed from the account balance
 */
void User::deduct(int amount) throw (NotEnoughFundsException)
{
	if(balance < amount)
	{
		throw NotEnoughFundsException(amount, balance);
	}
	balance -= amount;
}

/**
 * @param the cent amount by which to increase the balance
 */
void User::receive(int amount)
{
	balance += amount;
}

/**
 * Adds a new source of funds to the user account
 * @param source the shared pointer to the fund source
 */
void User::addFundSource(shared_ptr<FundSource>& source)
{
	funders.push_back(source);
}



