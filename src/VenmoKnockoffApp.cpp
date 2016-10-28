
#include <iostream>
#include <string>


#include "UserData.hpp"
#include "PayData.hpp"
#include "AddFundsData.hpp"
#include "CommandData.hpp"
#include "User.hpp"
#include "VenmoKnockoffApp.hpp"

using namespace std;

VenmoKnockoffApp::VenmoKnockoffApp():
	user(User("")),
	serverConnection() {}

void VenmoKnockoffApp::createUser(string &username, string &password)
{
	if (!user.getUsername().empty()) { // check if already logged in
		cerr << "Can't create a new user if already logged in." << endl;
		return;
	}
	
	// create account
	UserData userData(username, password);
	unsigned char buf[userData.size() + 1]; // extra byte for command type
	buf[0] = CommandType::CREATE_USER;
	userData.serialize(buf + 1);

	serverConnection.sendData(buf, sizeof(buf));	

	// login after creating account
	login(username, password);
}

void VenmoKnockoffApp::login(string &username, string &password)
{
	if (!user.getUsername().empty()) { // check if already logged in
		cerr << "Can't login when already logged in." << endl;
		return;
	}

	// authenticate login
        UserData userData(username, password);
        unsigned char buf[userData.size() + 1]; // extra byte for command type
        buf[0] = CommandType::LOGIN;
        userData.serialize(buf + 1);
        
        serverConnection.sendData(buf, sizeof(buf));

	// set current user
	user = User(username);
}

void VenmoKnockoffApp::pay(string &username, int amount)
{
	// send money to user
	PayData payData(username, amount);
	unsigned char buf[payData.size() + 1]; // extra byte for command type
	buf[0] = CommandType::PAY;
	payData.serialize(buf + 1);

	serverConnection.sendData(buf, sizeof(buf));
}

void VenmoKnockoffApp::addFunds(string &fundTag, int amount)
{
	// pull money from fund surce
        AddFundsData addFundsData(fundTag, amount); 
        unsigned char buf[addFundsData.size() + 1]; // extra byte for command type
        buf[0] = CommandType::ADD_FUNDS;
        addFundsData.serialize(buf + 1);

        serverConnection.sendData(buf, sizeof(buf));
}
