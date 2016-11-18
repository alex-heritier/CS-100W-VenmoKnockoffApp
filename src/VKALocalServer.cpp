
/**
 * VKALocalServer is the main file for the VenmoKnockoffApp local server.
 * MainClassUser and MainClassAI clients communicate with each other through this local server
 */

#include <iostream>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#include "ServerConnection.hpp"
#include "CommandData.hpp"
#include "Serializable.hpp"
#include "UserData.hpp"
#include "PayData.hpp"
#include "AddFundsData.hpp"
#include "User.hpp"
#include <map>
#include <sstream>
#include <cstdlib>

using std::string;
using std::cout;
using std::cerr;
using std::endl;

struct connection {
	string address;
	char type;
	Serializable *data;
};

void setup(string &);
void captureRequest(struct connection &);
void processRequest(struct connection &);
bool registerUser(const string& newUsername, const string& newPassword);
bool loginUser(const string& inUsername, const string& inPassword);
string payTo(const string& otherUsername, int amount);
string addFunds(int fundIndex, int amount);

static ServerConnection *server;

std::map<string, string> username_password;
std::map<string, std::shared_ptr<User> > userMap;
std::shared_ptr<User> currentUser;

void setup(string &socket_path)
{
	server = new ServerConnection(socket_path);
	server->setDebug(true);
}

void captureRequest(struct connection &con)
{
	unsigned char buf[MAX_PAYLOAD_SIZE];
	string client_path = server->readData(buf);

	// fill connection
	con.address = client_path;
	con.type = buf[0];	// set type byte
	switch (con.type) {
                case CommandType::CREATE_USER:
			con.data = new UserData(buf + 1);	// skip type byte
                        break;
                case CommandType::LOGIN:
			con.data = new UserData(buf + 1);   // skip type byte
                        break;
                case CommandType::PAY:
			con.data = new PayData(buf + 1);	// skip type byte
                        break;
                case CommandType::ADD_FUNDS:
			con.data = new AddFundsData(buf + 1);    // skip type byte
                        break;
                default:
                        cerr << "Bad command type." << endl;
                        exit(-1);
        }	
}

void processRequest(struct connection &con)
{
	Serializable *data = con.data;
	string response;
	switch (con.type) {
		case CommandType::CREATE_USER:
			cout << con.address << ": " << dynamic_cast<UserData *>(data)->getUsername() << ", ";
			cout << dynamic_cast<UserData *>(data)->getPassword() << endl;
			
			string newUsername(dynamic_cast<UserData *>(data)->getUsername() );
			string newPassword(dynamic_cast<UserData *>(data)->getPassword() );
			
			
			bool registerSuccess = registerUser(newUsername, newPassword);
			if(registerSuccess)
				response = "Account created successfully. Welcome to VenmoKnockoffApp!";
			else
				response = "The username already exists in the system. Account creation failed";
			break;
		case CommandType::LOGIN:
			cout << con.address << ": " << dynamic_cast<UserData *>(data)->getUsername() << ", ";
			cout << dynamic_cast<UserData *>(data)->getPassword() << endl;
			
			string inUsername(dynamic_cast<UserData *>(data)->getUsername() );
			string inPassword(dynamic_cast<UserData *>(data)->getPassword() );
			
			bool loginSuccess = loginUser(inUsername, inPassword);
		
			if(loginSuccess)
				response = "Login successful.";
			else
				response = "Login failure";
			break;
		case CommandType::PAY:
			cout << con.address << ": " << dynamic_cast<PayData *>(data)->getUsername() << ", ";
			cout << dynamic_cast<PayData *>(data)->getAmount() << endl;
			
			string otherUsername(dynamic_cast<PayData *>(data)->getUsername() );
			int amount = dynamic_cast<PayData *>(data)->getAmount();
			
			response = payTo(otherUsername, amount);
			break;
		case CommandType::ADD_FUNDS:
			cout << con.address << ": " << dynamic_cast<AddFundsData *>(data)->getFundTag() << ", ";
			cout << dynamic_cast<AddFundsData *>(data)->getAmount() << endl;
			
			string fundtag = dynamic_cast<AddFundsData *>(data)->getFundTag();
			int fundIndex = atoi(fundtag.c_str() );
			int amount = dynamic_cast<AddFundsData *>(data)->getAmount();
			
			response = addFunds(fundInxex, amount);
			//response += "\nFunds added successfully.";
			break;
		default:
			cerr << "ERROR: corrupted request from client." << endl;
			exit(-1);
	}
	if(currentUser)//If a user is logged in, response includes their user info
	{
		response += "\n";
		response += currentUser->toString();
	}
        server->sendData(con.address, response);
}

/**
* @param newUsername the username of a new user
* @param newPassword the user's password
*/
bool registerUser(const string& newUsername, const string& newPassword)
{
	if(userMap.find(newUsername) != userMap.end() || username_password.find(newUsername) != userMap.end()) //User already exists
	{
		return false;
	}
	std::shared_ptr<User> newUser(new User(newUsername) );
	currentUser = newUser; //Automatically log in new user
	userMap.insert(std::pair<string, std::shared_ptr<User> >(newUsername, newUser)); //Add to userMap
	username_password.insert(std::pair<string, string>( newUsername, newPassword ) ); //Add to username_password map
	return true;
}

/**
* @param inUsername the username of an existing user
* @param inPassword the user's password
*/
bool loginUser(const string& inUsername, const string& inPassword)
{
	if(userMap.find(inUsername) == userMap.end() || username_password.find(inUsername) == userMap.end() ) //User doesn't exist
	{
		return false;
	}
	if(username_password.find(inUsername)->second != inPassword) //wrong password
	{
		return false;
	}
	currentUser = userMap.find(inUsername)->second;
	return true;
}

/**
* @param otherUsername the name of the other user
* @param amount the amount to be transferred in cents
* @return a string containing a success message, or a message on failure
*/
string payTo(const string& otherUsername, int amount)
{
	if(userMap.find(otherUsername) == userMap.end() ) //other user doesn't exist
	{
		return "Other user doesn't exist";
	}
	if(!currentUser) //no one is logged in
	{
		return "No one is logged in";
	}
	
	try
	{
		currentUser->deduct(amount);
		std::shared_ptr<User> oUser = userMap.find(otherUsername)->second;
		oUser->receive(amount);
	}
	catch(NotEnoughFundsException& ex)
	{
		std::stringstream msg;
		msg << ex;
		return msg.str();
	}
	
	return "Payment processed successfully.";
}

/**
* @param fundIndex the index of the user's fund source
* @param amount the amount to be pulled from the fund source in cents
* @return a string containing the receipt, or a message on failure
*/
string addFunds(int fundIndex, int amount)
{
	if(!currentUser) //no one is logged in
	{
		return "No one is logged in.";
	}
	if(fundIndex >= currentUser->getFundSize() || fundInxex < 0)
	{
		return "Not a valid fund index.";
	}
	string receipt = currentUser->addFunds(fundIndex, amount);
	return receipt;
}

int main(int argc, char **argv)
{
	string socket_path = "/tmp/server_socket";
	setup(socket_path);

	struct connection con;
	while (true) {
		captureRequest(con);
		processRequest(con);
	}
	delete server;

	return 0;
}
