
#include <iostream>
#include <string>

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
	struct user_data_serial create_req;
	memset(&create_req, 0, sizeof(create_req));
	create_req.type = CommandType::CREATE_USER;
	strncpy(create_req.user.username, username.c_str(), username.length() + 1);
	strncpy(create_req.user.password, password.c_str(), password.length() + 1);

	cout << "SIZE OF CREATE " << sizeof(create_req) << endl;
	serverConnection.sendData(&create_req, sizeof(create_req));	

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
	struct user_data_serial login_req;
	memset(&login_req, 0, sizeof(login_req));
	login_req.type = CommandType::LOGIN;
	strncpy(login_req.user.username, username.c_str(), username.length()+1);
	strncpy(login_req.user.password, password.c_str(), password.length()+1);

	cout << "SIZE OF LOGIN " << sizeof(login_req) << endl;
	serverConnection.sendData(&login_req, sizeof(login_req));

	// set current user
	user = User(username);
}

void VenmoKnockoffApp::pay(string &username, int amount)
{
	// send money to user
	struct pay_data_serial pay_req;
	memset(&pay_req, 0, sizeof(pay_req));
	pay_req.type = CommandType::PAY;
	strncpy(pay_req.pay.username, username.c_str(), username.length()+1);
        pay_req.pay.amount = amount;

	unsigned char buf[sizeof(pay_req.type)
		+ sizeof(pay_req.pay.username)
		+ sizeof(pay_req.pay.amount)];
	serialize_pay_data(buf, &pay_req);

	for (int i = 0; i < sizeof(buf); i++) {
		cout << buf[i];
	}
	cout << endl;

	cout << "SIZE OF PAY REQUEST " << sizeof(pay_req) << endl;
	serverConnection.sendData(buf, sizeof(buf));
}

void VenmoKnockoffApp::addFunds(string &fundTag, int amount)
{
	// withdraw money from fund source
        struct add_funds_data_serial add_funds_req;
	memset(&add_funds_req, 0, sizeof(add_funds_req));
        add_funds_req.type = CommandType::PAY;
        strncpy(add_funds_req.add_funds.fund_tag, fundTag.c_str(), fundTag.length()+1);
        add_funds_req.add_funds.amount = amount;

        cout << "SIZE OF ADD FUNDS REQUEST " << sizeof(add_funds_req) << endl;
        serverConnection.sendData(&add_funds_req, sizeof(add_funds_req));
}
