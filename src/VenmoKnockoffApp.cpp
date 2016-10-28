
#include <iostream>
#include <string>
#include <thread>

#include "UserData.hpp"
#include "PayData.hpp"
#include "AddFundsData.hpp"
#include "CommandData.hpp"
#include "User.hpp"
#include "VenmoKnockoffApp.hpp"

using namespace std;

VenmoKnockoffApp::VenmoKnockoffApp():
	user(User("")),
	serverConnection()
{
	readThread = thread(&VenmoKnockoffApp::captureResponses, this);
}

void VenmoKnockoffApp::captureResponses()
{
	unsigned char buf[MAX_PAYLOAD_SIZE];

	while (true) {
        	string sender = serverConnection.readData(buf);

		cout << "Getting reponse from server..." << endl;
		response = string(reinterpret_cast<char *>(buf));
	}
}

bool VenmoKnockoffApp::getResponse(string &res)
{
	if (response.empty()) {
		return false;
	} else {
		res = response;
		response = "";
		return true;
	}
}

void VenmoKnockoffApp::createUser(string &username, string &password)
{
	if (!user.getUsername().empty()) { // check if already logged in
		cerr << "Can't create a new user if already logged in." << endl;
		return;
	}
	
	// prepare user create data
	UserData userData(username, password);
	unsigned char buf[userData.size() + 1]; // extra byte for command type
	buf[0] = CommandType::CREATE_USER;
	userData.serialize(buf + 1);

	// send user create data
	serverConnection.sendData(buf, sizeof(buf));	

	// wait for response
        string res;
        while (!getResponse(res)) {}    // wait for response
        cout << res << endl;

	// login after creating account
	login(username, password);
}

void VenmoKnockoffApp::login(string &username, string &password)
{
	if (!user.getUsername().empty()) { // check if already logged in
		cerr << "Can't login when already logged in." << endl;
		return;
	}

	// prepare login data
        UserData userData(username, password);
        unsigned char buf[userData.size() + 1]; // extra byte for command type
        buf[0] = CommandType::LOGIN;
        userData.serialize(buf + 1);
        
	// send login data
        serverConnection.sendData(buf, sizeof(buf));

	// wait for response
        string res;
        while (!getResponse(res)) {}    // wait for response
        cout << res << endl;


	// set current user
	user = User(username);
}

void VenmoKnockoffApp::pay(string &username, int amount)
{
	// prepare pay data
	PayData payData(username, amount);
	unsigned char buf[payData.size() + 1]; // extra byte for command type
	buf[0] = CommandType::PAY;
	payData.serialize(buf + 1);

	// send pay data
	serverConnection.sendData(buf, sizeof(buf));

	// wait for response
	string res;
	while (!getResponse(res)) {}	// wait for response
	cout << res << endl;
}

void VenmoKnockoffApp::addFunds(string &fundTag, int amount)
{
	// prepare add fund data
        AddFundsData addFundsData(fundTag, amount); 
        unsigned char buf[addFundsData.size() + 1]; // extra byte for command type
        buf[0] = CommandType::ADD_FUNDS;
        addFundsData.serialize(buf + 1);

	// send add fund data
        serverConnection.sendData(buf, sizeof(buf));

	// wait for response
        string res;
        while (!getResponse(res)) {}    // wait for response
        cout << res << endl;
}
