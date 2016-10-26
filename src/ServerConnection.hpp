
#ifndef SERVER_CONNECTION_HPP
#define SERVER_CONNECTION_HPP

#include <string>

class ServerConnection {
public:
	ServerConnection();
	ServerConnection(std::string, std::string);
	virtual ~ServerConnection();
	void sendData(std::string &);
	void sendData(void *, unsigned int);
private:
	// variables
	int sock;
	std::string socket_path;
	// methods
	void init(std::string, std::string);
};

#endif
