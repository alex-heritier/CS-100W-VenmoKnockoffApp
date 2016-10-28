
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

static ServerConnection *server;

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
			response = "Account created successfully. Welcome to VenmoKnockoffApp!";
			break;
		case CommandType::LOGIN:
			cout << con.address << ": " << dynamic_cast<UserData *>(data)->getUsername() << ", ";
			cout << dynamic_cast<UserData *>(data)->getPassword() << endl;
			response = "Login successful.";
			break;
		case CommandType::PAY:
			cout << con.address << ": " << dynamic_cast<PayData *>(data)->getUsername() << ", ";
			cout << dynamic_cast<PayData *>(data)->getAmount() << endl;
			response = "Payment processed successfully.";
			break;
		case CommandType::ADD_FUNDS:
			cout << con.address << ": " << dynamic_cast<AddFundsData *>(data)->getFundTag() << ", ";
			cout << dynamic_cast<AddFundsData *>(data)->getAmount() << endl;
			response = "Funds added successfully.";
			break;
		default:
			cerr << "ERROR: corrupted request from client." << endl;
			exit(-1);
	}
        server->sendData(con.address, response);
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
