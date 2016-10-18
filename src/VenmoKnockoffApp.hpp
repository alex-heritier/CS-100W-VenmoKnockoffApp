
#ifndef VENMO_KNOCKOFF_APP_HPP
#define VENMO_KNOCKOFF_APP_HPP

#include <string>

#include "User.hpp"

class VenmoKnockoffApp {
public:
	VenmoKnockoffApp();
	void createUser(string &, string &);
	void login(string &, string &);
	void pay(string &, int);
	void addFunds(string &, int);
private:
	User user;
};

#endif
