
#ifndef SERVER_CONNECTION_HPP
#define SERVER_CONNECTION_HPP

#include <string>

class ServerConnection {
public:
	ServerConnection();
	ServerConnection(std::string);
	ServerConnection(std::string, std::string);
	virtual ~ServerConnection();
	void sendData(std::string &);
	void sendData(void *, unsigned int);
	void sendData(std::string &, void *, unsigned int);
	std::string readData(unsigned char *);
private:
	// variables
	int sock;
	std::string socket_path;
	// methods
	void init_client(std::string);
	void init_server(std::string);
};

#endif
