
#ifndef VKA_CLIENT_HPP
#define VKA_CLIENT_HPP

#include <string>
#include <thread>

#include "ServerConnection.hpp"
#include "User.hpp"

class VKAClient {
public:
	VKAClient();
	void createUser(string &, string &);
	void login(string &, string &);
	void pay(string &, int);
	void addFunds(string &, int);
private:
	// variables
	User user;
	ServerConnection serverConnection;
	std::thread readThread;
	std::string response;
	// methods
	void captureResponses();
	bool getResponse(std::string &);
};

#endif
