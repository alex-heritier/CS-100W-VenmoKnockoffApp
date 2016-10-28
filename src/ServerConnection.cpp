
#include <iostream>
#include <string>
#include <cmath>
#include <random>

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#include "ServerConnection.hpp"
#include "CommandData.hpp"

using std::string;
using std::cout;
using std::cerr;
using std::endl;

void ServerConnection::init_client(string client_path)
{
	struct sockaddr_un client_addr;

        cout << "Socket path is " << client_path << endl;

        // build the client address
        memset(&client_addr, 0, sizeof(client_addr));
        client_addr.sun_family = AF_UNIX;
        strncpy(client_addr.sun_path, client_path.c_str(), client_path.length() + 1);

        // create the client socket
        if ((sock = socket(AF_UNIX, SOCK_DGRAM, 0)) < 0) {
                perror("Socket creation");
                exit(-1);
        }
        cout << "Socket identifier is " << sock << endl;

        // bind the client socket
        unlink(socket_path.c_str());
        if (bind(sock, (struct sockaddr *) &client_addr, sizeof(client_addr)) < 0) {
                perror("Binding name to datagram socket");
                exit(-1);
        }
}

void ServerConnection::init_server(std::string server_path)
{
	struct sockaddr_un server_addr;
	
	// build the server address
        if (!server_path.empty()) {
                memset(&server_addr, 0, sizeof(server_addr));
                server_addr.sun_family = AF_UNIX;
                strncpy(server_addr.sun_path, server_path.c_str(), server_path.length() + 1);
        }

	// connect to server socket if server is specified
        if (!server_path.empty()) {
                if (connect(sock, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
                        perror("Connecting to server");
                        exit(-1);
                }
        }
}

ServerConnection::ServerConnection(string client_path, string server_path): sock(-1), socket_path(client_path)
{
	init_client(client_path);
	init_server(server_path);
}

ServerConnection::ServerConnection(string client_path): sock(-1), socket_path(client_path)
{
	init_client(client_path);
}

ServerConnection::ServerConnection(): sock(-69), socket_path("") {
	string client_socket_path("/tmp/client_socket");
	string server_socket_path("/tmp/server_socket");
	
	std::random_device rd;
	unsigned int suffix = rd();
	client_socket_path.append(std::to_string(suffix));
	
	init_client(client_socket_path);
	init_server(server_socket_path);
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

void ServerConnection::sendData(string &socket_path, void *buf, unsigned int buf_length)
{
	struct sockaddr_un socket_addr;

	// build the socket address
	memset(&socket_addr, 0, sizeof(socket_addr));
	socket_addr.sun_family = AF_UNIX;
	strncpy(socket_addr.sun_path, socket_path.c_str(), socket_path.length() + 1);

	if (connect(sock, (struct sockaddr *) &socket_addr, sizeof(socket_addr)) < 0) {
		perror("Connecting to server");
		exit(-1);
        }

	sendData(buf, buf_length);
}

string ServerConnection::readData(unsigned char *buf)
{
        struct sockaddr_un client_addr;

        // get client payload and address
        socklen_t size = sizeof(client_addr);
        int bytes_read = 0;
        if ((bytes_read = recvfrom(sock, buf, MAX_PAYLOAD_SIZE, 0,
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
	
	return string(client_addr.sun_path);
}

