
#include <iostream>
#include <string>

#include "NonexistentCommandException.hpp"
#include "VenmoKnockoffApp.hpp"

using namespace std;

void processCreateUser(vector<string> &);
void processLogin(vector<string> &);
void processPay(vector<string> &);
void processAddFunds(vector<string> &);

void processCommand(string &command, VenmoKnockoffApp &app)
{
	string createUserCommand("create_user");
	string loginCommand("login");
	string payCommand("pay");
	string addFundsCommand("add_funds");

	vector<string> params;
	if (command == createUserCommand) {
		processCreateUser(params);
		// app.createUser(params[0]);		
	} else if (command == loginCommand) {
		processLogin(params);
		// app.login(params[0], params[1]);
	} else if (command == payCommand) {
		processPay(params);
		// app.pay(params[0], std::stoi(params[1]));
	} else if (command == addFundsCommand) {
		processAddFunds(params);
		// app.addFunds(params[0], params[1]);
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
	// TODO
}

void processPay(vector<string> &params)
{
	// TODO
}

void processAddFunds(vector<string> &params)
{
	// TODO
}

int main(int argc, char **argv)
{
	VenmoKnockoffApp app;

	
        cout << "Enter a command." << endl;
	cout << "> ";

	string cmd;
    	while(getline(std::cin, cmd)) {
		try {
			processCommand(cmd, app);
		} catch (NonexistentCommandException &e) {
			cout << "Nonexistent command entered." << endl;
		}

		cout << "Enter a command." << endl;
                cout << "> ";
   	}
    	return 0;
}
