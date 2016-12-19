
#include <iostream>
#include <string>

#include "NonexistentCommandException.hpp"
#include "VKAClient.hpp"

using namespace std;

void processCommand(string &, VKAClient &);
void processCreateUser(vector<string> &);
void processLogin(vector<string> &);
void processPay(vector<string> &);
void processAddFunds(vector<string> &);
void processAddFundSource(vector<string> &);


string createUserCommand   	= "create_user";
string loginCommand        	= "login";
string payCommand          	= "pay";
string addFundsCommand     	= "add_funds";
string addFundSourceCommand 	= "add_fund_source";
	
void processCommand(string &command, VKAClient &app)
{


	vector<string> params;
	if (command == createUserCommand) {
		processCreateUser(params);
		app.createUser(params[0], params[1]);
	} else if (command == loginCommand) {
		processLogin(params);
		app.login(params[0], params[1]);
	} else if (command == payCommand) {
		processPay(params);
		app.pay(params[0], params[1], std::stoi(params[2]));
	} else if (command == addFundsCommand) {
		processAddFunds(params);
		app.addFunds(params[0], params[1],  std::stoi(params[2]));
	} else if (command == addFundSourceCommand) {
		processAddFundSource(params);
		app.addFundSource(params[0], params[1], params[2], params[3]);
	} else {
		throw NonexistentCommandException(command);
	}
}

void processCreateUser(vector<string> &params)
{
	string username;
	string password;

	cout << "What is your username?" << endl;
	cout << "> ";
	getline(std::cin, username);

	cout << "What is your password?" << endl;
	cout << "> ";
	getline(std::cin, password);

	params.push_back(username);
	params.push_back(password);
}

void processLogin(vector<string> &params)
{
	string username;
	string password;

	cout << "What is your username?" << endl;
	cout << "> ";
	getline(std::cin, username);

	cout << "What is your password?" << endl;
        cout << "> ";
        getline(std::cin, password);

	params.push_back(username);
	params.push_back(password);
}

void processPay(vector<string> &params)
{
	string myUsername;
	string otherUsername;
	string amount;

	cout << "Who are you?" << endl;
	cout << "> ";
	getline(std::cin, myUsername);

	cout << "Who are you paying?" << endl;
	cout << "> ";
	getline(std::cin, otherUsername);

	cout << "How much are you sending in cents?" << endl;
	cout << "> ";
	getline(std::cin, amount);

	params.push_back(myUsername);
	params.push_back(otherUsername);
	params.push_back(amount);
}

void processAddFunds(vector<string> &params)
{
	string username;
	string fundTag;
	string amount;
	
	cout << "What is your username?" << endl;
	cout << "> ";
	getline(std::cin, username);

	cout << "What fund source are you pulling from?" << endl;
        cout << "> ";
        getline(std::cin, fundTag);

        cout << "How much are you withdrawing in cents?" << endl;
        cout << "> ";
        getline(std::cin, amount);

	params.push_back(username);
        params.push_back(fundTag);
        params.push_back(amount);
}

void processAddFundSource(vector<string> &params)
{
	string type;
	string username;
	string company;
	string fundID;
	string cardType;

	cout << "What is your username?" << endl;
	cout << "> ";
	getline(std::cin, username);

	cout << "Is this a bank account or a card?" << endl;
	cout << "> ";
	getline(std::cin, type);

	string fundName = type == "bank" ? "bank account" : "card";
	
	cout << "What is the " << fundName << "'s company name?" << endl;
	cout << "> ";
	getline(std::cin, company);

	cout << "What is the " << fundName << "'s number?" << endl;
	cout << "> ";
	getline(std::cin, fundID);

	if (type == "card") {
		cout << "What type of card is this?" << endl;
		cout << "> ";
		getline(std::cin, cardType);
	}

	params.push_back(username);
	params.push_back(company);
	params.push_back(fundID);
	params.push_back(cardType);
}

int main(int argc, char **argv)
{
	VKAClient app;

	string cmd;
	while(true) {
		cout << endl;
        	cout << "Enter a command." << endl;
			cout << "Choose from " << createUserCommand << ", " << loginCommand<< ", " << payCommand << ", " << addFundsCommand<< ", " << addFundSourceCommand << endl;
        	cout << "> ";
        	getline(std::cin, cmd);

        	try {
            		processCommand(cmd, app);
        	} catch (const NonexistentCommandException &e) {
           		cout << "Nonexistent command entered." << endl;
        	}
    	}
	return 0;
}
