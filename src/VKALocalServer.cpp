
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
void sendData(string &);
void captureRequest(struct connection &);
void processRequest(struct connection &);

static int server_sock;
static ServerConnection *server;

void setup(string &socket_path)
{
	server = new ServerConnection(socket_path, "");
	/*
	struct sockaddr_un server_addr;

	// build the server address
        memset(&server_addr, 0, sizeof(server_addr));
        server_addr.sun_family = AF_UNIX;
        strncpy(server_addr.sun_path, socket_path.c_str(), sizeof(server_addr.sun_path)-1);

	// create server socket
        if ((server_sock = socket(AF_UNIX, SOCK_DGRAM, 0)) < 0) {
		perror("Creating socket");
		exit(-1);
	}

	// bind server socket
	unlink(socket_path.c_str());
	if (bind(server_sock, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
		perror("Binding socket");
		exit(-1);
	}
	*/
}

void sendData(string &msg)
{
	server->sendData(msg);
	/*
	if (send(server_sock, msg.c_str(), msg.length(), 0) < 0) {
		perror("Sending to socket");
		exit(-1);
	}
	*/
}

void captureRequest(struct connection &con)
{
	unsigned char buf[MAX_PAYLOAD_SIZE];
	string client_path(server->readData(buf));
	/*
	struct sockaddr_un client_addr;
	
	// get client payload and address
	socklen_t size = sizeof(client_addr);
	int bytes_read = 0;
	if ((bytes_read = recvfrom(server_sock, buf, MAX_PAYLOAD_SIZE, 0,
			(struct sockaddr *) &client_addr, &size)) < 0) {
		perror("Reading socket");
		exit(-1);
	}
	cout << "Bytes read: " << bytes_read << endl;
	cout << "The bytes read were: " << endl;
	for (int i = 0; i < bytes_read; i++) {
		cout << (char)buf[i];
	}
	cout << endl;
	*/

	// fill connection
	con.address = client_path;
	con.type = buf[0];	// 1st byte is the type indicator
	switch (con.type) {	// fill union based on which type the incoming data is
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
	switch (con.type) {
		case CommandType::CREATE_USER:
			cout << con.address << ": " << dynamic_cast<UserData *>(data)->getUsername() << ", ";
			cout << dynamic_cast<UserData *>(data)->getPassword() << endl;
			break;
		case CommandType::LOGIN:
			cout << con.address << ": " << dynamic_cast<UserData *>(data)->getUsername() << ", ";
			cout << dynamic_cast<UserData *>(data)->getPassword() << endl;
			break;
		case CommandType::PAY:
			cout << con.address << ": " << dynamic_cast<PayData *>(data)->getUsername() << ", ";
			cout << dynamic_cast<PayData *>(data)->getAmount() << endl;
			break;
		case CommandType::ADD_FUNDS:
			cout << con.address << ": " << dynamic_cast<AddFundsData *>(data)->getFundTag() << ", ";
			cout << dynamic_cast<AddFundsData *>(data)->getAmount() << endl;
			break;
		default:
			cerr << "Bad command type." << endl;
			exit(-1);
	}	
}

int main(int argc, char **argv)
{
	string socket_path("/tmp/server_socket");
	setup(socket_path);

	struct connection con;
	while (true) {
		captureRequest(con);
		processRequest(con);
	}
	close(server_sock);

	return 0;
}
