
#include <iostream>
#include <string>
#include <thread>
#include <mutex>

#include "VKAClient.hpp"
#include "CommandData.hpp"
#include "UserData.hpp"
#include "PayData.hpp"
#include "AddFundsData.hpp"
#include "AddFundSourceData.hpp"
#include "User.hpp"

using namespace std;

VKAClient::VKAClient():
	user(User("")),
	serverConnection()
{
	readThread = thread(&VKAClient::captureResponses, this);
}

void VKAClient::captureResponses()
{
	unsigned char buf[MAX_PAYLOAD_SIZE];

	while (true) {
        	string sender = serverConnection.readData(buf);

		std::lock_guard<std::mutex> guard(responseMutex);	// lock response
		cout << "Getting response from server..." << endl;
		response = string(reinterpret_cast<char *>(buf));
	}
}

bool VKAClient::getResponse(string &res)
{
	std::lock_guard<std::mutex> guard(responseMutex);	// lock response

	if (response.empty()) {
		return false;
	} else {
		res = response;
		response = "";
		return true;
	}
}

void VKAClient::createUser(string &username, string &password)
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

void VKAClient::login(string &username, string &password)
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

void VKAClient::pay(string &sender, string &sendee, int amount)
{
	// prepare pay data
	PayData payData(sender, sendee, amount);
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

void VKAClient::addFunds(string &username, string &fundTag, int amount)
{
	// prepare add fund data
        AddFundsData addFundsData(username, fundTag, amount);
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

void VKAClient::addFundSource(string &username, string &company, string &fundID, string &cardType)
{
	// prepare data
	AddFundSourceData addFundSourceData(username, company, fundID, cardType);
	unsigned char buf[addFundSourceData.size() + 1]; // extra byte for command type
        buf[0] = CommandType::ADD_FUND_SOURCE;
        addFundSourceData.serialize(buf + 1);

        // send data
        serverConnection.sendData(buf, sizeof(buf));

        // wait for response
        string res;
        while (!getResponse(res)) {}    // loop until response
        cout << res << endl;
}
