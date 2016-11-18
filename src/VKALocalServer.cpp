
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
#include "Bank.hpp"
#include "Card.hpp"
#include <map>
#include <sstream>
#include <cstdlib>
#include <fstream>


#include <boost/serialization/export.hpp>
#include <boost/serialization/map.hpp>

using std::string;
using std::cout;
using std::cerr;
using std::endl;
using namespace vkastd;

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
void loadUserMaps();
void saveUserMaps();

static ServerConnection *server;

std::map<string, string> username_password;
std::map<string, std::shared_ptr<User> > userMap;
std::shared_ptr<User> currentUser;

string uMapFile("userMap.txt");

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
			
			if(registerUser(dynamic_cast<UserData *>(data)->getUsername(),dynamic_cast<UserData *>(data)->getPassword() ) )
				response = "Account created successfully. Welcome to VenmoKnockoffApp!";
			else
				response = "The username already exists in the system. Account creation failed";
			break;
		case CommandType::LOGIN:
			cout << con.address << ": " << dynamic_cast<UserData *>(data)->getUsername() << ", ";
			cout << dynamic_cast<UserData *>(data)->getPassword() << endl;

			if(loginUser(dynamic_cast<UserData *>(data)->getUsername() , dynamic_cast<UserData *>(data)->getPassword() ) )
				response = "Login successful.";
			else
				response = "Login failure";
			break;
		case CommandType::PAY:
			cout << con.address << ": " << dynamic_cast<PayData *>(data)->getUsername() << ", ";
			cout << dynamic_cast<PayData *>(data)->getAmount() << endl;
			
			response = payTo(dynamic_cast<PayData *>(data)->getUsername(), dynamic_cast<PayData *>(data)->getAmount() );
			break;
		case CommandType::ADD_FUNDS:
			cout << con.address << ": " << dynamic_cast<AddFundsData *>(data)->getFundTag() << ", ";
			cout << dynamic_cast<AddFundsData *>(data)->getAmount() << endl;

			response = addFunds(atoi(dynamic_cast<AddFundsData *>(data)->getFundTag().c_str()) , dynamic_cast<AddFundsData *>(data)->getAmount() );
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
        saveUserMaps();
		server->sendData(con.address, response);
}

/**
* @param newUsername the username of a new user
* @param newPassword the user's password
*/
bool registerUser(const string& newUsername, const string& newPassword)
{
	if( !(userMap.find(newUsername) == userMap.end() ) || !(username_password.find(newUsername) == username_password.end() ) ) //User already exists
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
	if(userMap.find(inUsername) == userMap.end() || username_password.find(inUsername) == username_password.end() ) //User doesn't exist
	{
		return false;
	}
	if(username_password[inUsername] != inPassword) //wrong password
	{
		return false;
	}
	currentUser = userMap[inUsername];
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
		//std::shared_ptr<User> oUser = userMap.find(otherUsername)->second;
		//oUser->receive(amount);
		userMap[otherUsername]->receive(amount);
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
	if(fundIndex >= currentUser->getFundSize() || fundIndex < 0)
	{
		return "Not a valid fund index.";
	}
	string receipt = currentUser->addFunds(fundIndex, amount);
	return receipt;
}

//Loads user map and password map from a file
void loadUserMaps()
{
	std::ifstream inFile(uMapFile);
	if(!inFile.is_open())
	{
		cout << "Couldn't open file" << endl;
		std::shared_ptr<User> u1(new User("Default", 100));
		std::shared_ptr<User> u2(new User("Default2", 100));
		std::shared_ptr<FundSource> bank1(new Bank("Bank1", 123) );
		u1->addFundSource(bank1);
		std::shared_ptr<FundSource> card1(new Card("Yugi", "credit", 2500) );
		u2->addFundSource(card1);
		userMap.insert(std::pair<string, std::shared_ptr<User> >(u1->getUsername(),u1) );
		userMap.insert(std::pair<string, std::shared_ptr<User> >(u2->getUsername(),u2) );
		username_password.insert(std::pair<string, string> (u1->getUsername(), "qwerty1"));
		username_password.insert(std::pair<string, string> (u2->getUsername(), "qwerty2"));
		return;
	}
	boost::archive::text_iarchive inArch {inFile};
	inArch.register_type<Bank>();
	inArch.register_type<Card>();
	inArch >> userMap >> username_password;
}

//Saves user map and password map to a file
void saveUserMaps()
{
	std::ofstream outFile(uMapFile);
	boost::archive::text_oarchive outArch {outFile};
	outArch.register_type<Bank>();
	outArch.register_type<Card>();
	outArch << userMap << username_password;
}

int main(int argc, char **argv)
{
	loadUserMaps();
	string socket_path = "/tmp/server_socket";
	setup(socket_path);

	struct connection con;
	while (true) {
		captureRequest(con);
		processRequest(con);
	}
	delete server;
	saveUserMaps();
	return 0;
}
