
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
#include "AddFundSourceData.hpp"
#include "User.hpp"
#include "Bank.hpp"
#include "Card.hpp"
#include <map>
#include <set>
#include <sstream>
#include <cstdlib>
#include <fstream>
#include <algorithm>


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
template<class Function>
string simpleEncrypt(const string& original, Function unaryCharFun);
string sizeEncrypt(const string& original);
void setup(string &);
void captureRequest(struct connection &);
void processRequest(struct connection &);
bool registerUser(const string& newUsername, const string& newPassword);
bool loginUser(const string& inUsername, const string& inPassword);
string payTo(const string& sender, const string& receiver, int amount);
string addFunds(const string& username, int fundIndex, int amount);
string addFundSource(const string &, const string &, const string &, const string &);
void loadUserMaps();
void saveUserMaps();

static ServerConnection *server;

std::map<string, string> username_password;
std::map<string, std::shared_ptr<User> > userMap;
//std::shared_ptr<User> currentUser;
std::set<string> loggedInUsers;

string uMapFile("userMap.txt");

/**
Returns a string encrypted with a simple unary character function
@param original the original string
@param unaryCharFun the unary character function
@return the encrypted string
*/
template<class Function>
string simpleEncrypt(const string& original, Function unaryCharFun)
{
	std::stringstream enString;
	for(string::size_type i = 0; i < original.size(); i++)
	{
		enString << unaryCharFun(original[i]);
	}
	return enString.str();
}

/**
Returns a string where the size of the string is added to all characters
@param original the original string
@return the new string
*/
string sizeEncrypt(const string& original)
{
	return simpleEncrypt(original, [&original] (char c)
			{
				char offSet = static_cast<char>(original.size());
				char result = c + offSet;
				return result;
			});
}

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
	string curUser = "";
	switch (con.type) {
		case CommandType::CREATE_USER:
			cout << con.address << ": " << dynamic_cast<UserData *>(data)->getUsername() << ", ";
			cout << dynamic_cast<UserData *>(data)->getPassword() << endl;
			
			if(registerUser(dynamic_cast<UserData *>(data)->getUsername(),dynamic_cast<UserData *>(data)->getPassword() ) )
			{
				response = "Account created successfully. Welcome to VenmoKnockoffApp!";
				curUser = dynamic_cast<UserData *>(data)->getUsername();
			}
			else
				response = "The username already exists in the system. Account creation failed";
			break;
		case CommandType::LOGIN:
			cout << con.address << ": " << dynamic_cast<UserData *>(data)->getUsername() << ", ";
			cout << dynamic_cast<UserData *>(data)->getPassword() << endl;

			if(loginUser(dynamic_cast<UserData *>(data)->getUsername() , dynamic_cast<UserData *>(data)->getPassword() ) )
			{
				response = "Login successful.";
				curUser = dynamic_cast<UserData *>(data)->getUsername();
			}
			else
				response = "Login failure";
			break;
		case CommandType::PAY:
			//cout << con.address << ": " << dynamic_cast<PayData *>(data)->getUsername() << ", ";
			//cout << dynamic_cast<PayData *>(data)->getAmount() << endl;
			
			response = payTo(dynamic_cast<PayData *>(data)->getSender(), dynamic_cast<PayData *>(data)->getSendee(),dynamic_cast<PayData *>(data)->getAmount() );
			curUser = dynamic_cast<PayData *>(data)->getSender();
			break;
		case CommandType::ADD_FUNDS:
			//cout << con.address << ": " << dynamic_cast<AddFundsData *>(data)->getFundTag() << ", ";
			//cout << dynamic_cast<AddFundsData *>(data)->getAmount() << endl;

			response = addFunds(dynamic_cast<AddFundsData *>(data)->getUsername(), atoi(dynamic_cast<AddFundsData *>(data)->getFundTag().c_str()) , dynamic_cast<AddFundsData *>(data)->getAmount() );
			//response += "\nFunds added successfully.";
			curUser = dynamic_cast<AddFundsData *>(data)->getUsername();
			break;
		case CommandType::ADD_FUND_SOURCE:
			response = addFundSource(dynamic_cast<AddFundSourceData *>(data)->getUsername(), dynamic_cast<AddFundSourceData *>(data)->getCompany(), dynamic_cast<AddFundSourceData *>(data)->getFundID(), dynamic_cast<AddFundSourceData *>(data)->getCardType());
			curUser = dynamic_cast<AddFundSourceData *>(data)->getUsername();
			break;
		default:
			cerr << "ERROR: corrupted request from client." << endl;
			exit(-1);
	}
	if(curUser != "" && loggedInUsers.find(curUser) != loggedInUsers.end() )//If a user is logged in, response includes their user info
	{
		response += "\n";
		response += userMap[curUser]->toString();
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
	
	//Comment out 4 lines when adding FundSource command is fully implemented
	//std::shared_ptr<FundSource> defaultCard(new Card("Default Company", "credit", 100 * newUsername.length() + 1000 * newPassword.length() ) );
	//std::shared_ptr<FundSource> defaultBank(new Bank("Default Company 2", 101 * newUsername.length() + 1010 * newPassword.length()) );
	//newUser->addFundSource(defaultCard);
	//newUser->addFundSource(defaultBank);
	
	loggedInUsers.insert(newUsername); //Automatically log in new user
	userMap.insert(std::pair<string, std::shared_ptr<User> >(newUsername, newUser)); //Add to userMap
	username_password.insert(std::pair<string, string>( newUsername, sizeEncrypt(newPassword) ) ); //Add to username_password map
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
	if(username_password[inUsername] != sizeEncrypt(inPassword) ) //wrong password
	{
		return false;
	}
	loggedInUsers.insert(inUsername);
	return true;
}

/**
* @param sender the name of the sending user
* @param receiver the name of the other user
* @param amount the amount to be transferred in cents
* @return a string containing a success message, or a message on failure
*/
string payTo(const string& sender, const string& receiver, int amount)
{
	
	if(std::find(loggedInUsers.begin(), loggedInUsers.end(), sender) == loggedInUsers.end()) //User not logged in
	{
		return "User not logged in";
	}
	if(userMap.find(receiver) == userMap.end() ) //other user doesn't exist
	{
		return "Other user doesn't exist";
	}
	
	try
	{
		userMap[sender]->deduct(amount);
		//std::shared_ptr<User> oUser = userMap.find(otherUsername)->second;
		//oUser->receive(amount);
		userMap[receiver]->receive(amount);
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
* @param username the name of the user making this request
* @param fundIndex the index of the user's fund source
* @param amount the amount to be pulled from the fund source in cents
* @return a string containing the receipt, or a message on failure
*/
string addFunds(const string& username, int fundIndex, int amount)
{
	if(std::find(loggedInUsers.begin(), loggedInUsers.end(), username) == loggedInUsers.end())
	{
		return "User not logged in.";
	}
	if(fundIndex >= userMap[username]->getFundSize() || fundIndex < 0)
	{
		return "Not a valid fund index.";
	}
	string receipt = userMap[username]->addFunds(fundIndex, amount);
	return receipt;
}

string addFundSource(const string &username, const string &company, const string &fundID, const string &cardType)
{
	// TODO implement this
	if(std::find(loggedInUsers.begin(), loggedInUsers.end(), username) == loggedInUsers.end())
	{
		return "User not logged in.";
	}
	if(cardType.empty())
	{
		std::shared_ptr<FundSource> bank(new Bank(company, atoi(fundID.c_str() ) ) );
		userMap[username]->addFundSource(bank);
	}
	else
	{
		std::shared_ptr<FundSource> card(new Card(company, cardType, atoi(fundID.c_str() ) ) );
		userMap[username]->addFundSource(card);
	}
	return "Added fund source";
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
		username_password.insert(std::pair<string, string> (u1->getUsername(), sizeEncrypt("qwerty1") ));
		username_password.insert(std::pair<string, string> (u2->getUsername(), sizeEncrypt("qwerty2") ));
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
