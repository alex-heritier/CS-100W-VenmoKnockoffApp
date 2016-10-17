
#include <iostream>
#include <string>

#include "NonexistentCommandException.hpp"

using namespace std;

void processCreateUser();
void processLogin();
void processPay();
void processAddFunds();

void processCommand(string &command)
{
	string createUserCommand("create_user");
	string loginCommand("login");
	string payCommand("pay");
	string addFundsCommand("add_funds");

	if (command == createUserCommand) {
		processCreateUser();
	} else if (command == loginCommand) {
		processLogin();
	} else if (command == payCommand) {
		processPay();
	} else if (command == addFundsCommand) {
		processAddFunds();
	} else {
		throw NonexistentCommandException(command);
	}
}

void processCreateUser()
{
	// TODO
}

void processLogin()
{
	// TODO
}

void processPay()
{
	// TODO
}

void processAddFunds()
{
	// TODO
}

int main(int argc, char **argv)
{
	string tmp;
    	while(getline(cin, tmp)) {
		try {
			processCommand(tmp);
		} catch (NonexistentCommandException &e) {
			cout << "Nonexistent command entered." << endl;
		}
   	}
    	return 0;
}
