/*
 * User.hpp
 *
 *  Created on: Oct 15, 2016
 *      Author: Example
 */

#ifndef USER_HPP_
#define USER_HPP_

#include <string>
#include <vector>
#include <memory>
#include "FundSource.hpp"
#include "NotEnoughFundsException.hpp"

using namespace std;

//Class that represents a user of the command line payment system
class User {
public:
	User(const string& username);
	User(const string& username, int balance);
	virtual ~User();
	string toString() const;
	string addFunds(int fundIndex, int amount);
	int getBalance() const;
	void deduct(int amount) throw (NotEnoughFundsException);
	void receive(int amount);
	void addFundSource(shared_ptr<FundSource>& source);
private:
	string username;
	int balance;
	vector<shared_ptr<FundSource> > funders;

};

#endif /* USER_HPP_ */
