
#include <iostream>

#include "VenmoKnockoffApp.hpp"

using namespace std;

VenmoKnockoffApp::VenmoKnockoffApp():
	user(string("_DEFAULT"))
{

}

void VenmoKnockoffApp::createUser(string &username, string &password)
{
	// create account somehow
	login(username, password);
}

void VenmoKnockoffApp::login(string &username, string &password)
{
	user = User(username);
}

void VenmoKnockoffApp::pay(string &username, int amount)
{
	cout << "Paying " << username << " $" << amount / 100 << endl;
}

void VenmoKnockoffApp::addFunds(string &fundTag, int amount)
{
	cout << "Withdrawing $" << amount / 100 << " from " << fundTag << endl;
}
