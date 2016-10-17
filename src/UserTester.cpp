//============================================================================
// Name        : UserTester.cpp
// Author      : Bradley Wong
// Version     :
// Copyright   : 
// Description : Compile with C++11 or higher
//============================================================================

#include <iostream>
#include "User.hpp"
#include "Bank.hpp"
#include "Card.hpp"
using namespace std;

int main() {
	User bradUser("Bradley", 100);
	shared_ptr<FundSource> bank1 (new Bank("Evil", 666)); //Use shared_ptr to prevent object slicing
	shared_ptr<FundSource> card1 (new Card("Blue Eyes", "debit", 3000));
	bradUser.addFundSource(bank1);
	bradUser.addFundSource(card1);
	cout << bradUser.toString() << endl;
	try{
	bradUser.deduct(101);
	}
	catch(NotEnoughFundsException& ex)
	{
		cout << ex.getAmountPresent() << " " << ex.getAmountPulled() << endl;
	}

	cout << bradUser.addFunds(0, 2000000) << endl;
	cout << bradUser.toString() << endl;
	return 0;
}
