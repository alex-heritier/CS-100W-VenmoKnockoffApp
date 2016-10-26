
#include <iostream>
#include <string>
#include <cmath>
#include <random>

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#include "ServerConnection.hpp"

using std::string;
using std::cout;
using std::cerr;
using std::endl;

void ServerConnection::init(string client_path, string server_path)
{
	struct sockaddr_un client_addr;
        struct sockaddr_un server_addr;

        cout << "Client socket path is " << client_path << endl;
        cout << "Server socket path is " << server_path << endl;

        // build the client address
        memset(&client_addr, 0, sizeof(client_addr));
        client_addr.sun_family = AF_UNIX;
        strncpy(client_addr.sun_path, client_path.c_str(), sizeof(client_addr.sun_path)-1);
        //client_addr.sun_path[0] = '\0';

        // build the server address
        memset(&server_addr, 0, sizeof(server_addr));
        server_addr.sun_family = AF_UNIX;
        strncpy(server_addr.sun_path, server_path.c_str(), sizeof(server_addr.sun_path)-1);

        // create the client socket
        if ((sock = socket(AF_UNIX, SOCK_DGRAM, 0)) < 0) {
                perror("Socket creation");
                exit(-1);
        }
        cout << "Client socket identifier is " << sock << endl;

        // bind the client socket
        unlink(socket_path.c_str());
        if (bind(sock, (struct sockaddr *) &client_addr, sizeof(client_addr)) < 0) {
                perror("Binding name to datagram socket");
                exit(-1);
        }

        // connect to server socket
        if (connect(sock, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
                perror("Connecting to server");
                exit(-1);
        }

        // test sending some data
        string msg("0Hello World!");
	sendData(msg);
}

ServerConnection::ServerConnection(string client_path, string server_path): sock(-69), socket_path(client_path)
{
	init(client_path, server_path);
}

ServerConnection::ServerConnection(): sock(-69), socket_path("") {
	string client_socket_path("/tmp/client_socket");
	string server_socket_path("/tmp/server_socket");
	
	std::random_device rd;
	unsigned int suffix = rd();
	client_socket_path.append(std::to_string(suffix));
	
	init(client_socket_path, server_socket_path);
}

ServerConnection::~ServerConnection()
{
	close(sock);
	unlink(socket_path.c_str());
}

void ServerConnection::sendData(string &msg)
{
	sendData((void *)msg.c_str(), msg.length() + 1);
}

void ServerConnection::sendData(void *buf, unsigned int buf_length)
{
	if (send(sock, buf, buf_length, 0) < 0) {
                perror("Socket write");
                exit(-1);
        }
}
