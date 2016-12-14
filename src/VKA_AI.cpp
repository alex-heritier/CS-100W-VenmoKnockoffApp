
#include <iostream>
#include <string>
#include <ctime>
#include <cstdlib>
#include <chrono>
#include <thread>

#include "VKAClient.hpp"
#include "CommandData.hpp"

using namespace std;

void randomizeString(char *buf, int length);
int randomInt();
void randomCreateUser(VKAClient &app);
void randomLogin(VKAClient &app);
void randomPay(VKAClient &app);
void randomAddFunds(VKAClient &app);

void randomizeString(char *buf, int length)
{
    	int i = 0;
	for (i = 0; i < length; i++) {
		buf[i] = (rand() % 74) + 48;
	}
    	for (; i < FIELD_SIZE; i++) {
        	buf[i] = '\0';
    	}
}

int randomInt()
{
	return (int)rand() % 500;
}

void randomCreateUser(VKAClient &app)
{
	string username;
	string password;

	char buf[(randomInt() % (FIELD_SIZE - 6)) + 6];
	randomizeString(buf, (randomInt() % (FIELD_SIZE - 6)) + 6);
	username = string(buf, FIELD_SIZE); // (randomInt() % ((randomInt() % (FIELD_SIZE - 6)) + 6 - 6)) + 6
	randomizeString(buf, (randomInt() % (FIELD_SIZE - 6)) + 6);
	password = string(buf, FIELD_SIZE);

	app.createUser(username, password);
}

void randomLogin(VKAClient &app)
{
	string username;
	string password;

        char buf[(randomInt() % (FIELD_SIZE - 6)) + 6];
        randomizeString(buf, (randomInt() % (FIELD_SIZE - 6)) + 6);
        username = string(buf, FIELD_SIZE);
        randomizeString(buf, (randomInt() % (FIELD_SIZE - 6)) + 6);
        password = string(buf, FIELD_SIZE);

        app.login(username, password);
}

void randomPay(VKAClient &app)
{
	string myUsername;
	string otherUsername;
	int amount;
	
        char buf[(randomInt() % (FIELD_SIZE - 6)) + 6];
        randomizeString(buf, (randomInt() % (FIELD_SIZE - 6)) + 6);
        myUsername = string(buf, FIELD_SIZE);
	randomizeString(buf, (randomInt() % (FIELD_SIZE - 6)) + 6);
        otherUsername = string(buf, FIELD_SIZE);
        amount = randomInt();

        app.pay(myUsername, otherUsername, amount);
}

void randomAddFunds(VKAClient &app)
{
	string username;
	string fundTag;
	int amount;

	char buf[(randomInt() % (FIELD_SIZE - 6)) + 6];
	randomizeString(buf, (randomInt() % (FIELD_SIZE - 6)) + 6);
        username = string(buf, FIELD_SIZE);
	randomizeString(buf, (randomInt() % (FIELD_SIZE - 6)) + 6);
        fundTag = string(buf, FIELD_SIZE);
        amount = randomInt();

        app.addFunds(username, fundTag, amount);
}

int main(int argc, char **argv)
{
	VKAClient app;
	void (*function_array[4])(VKAClient &);

	function_array[0] = randomCreateUser;
	function_array[1] = randomLogin;
	function_array[2] = randomPay;
	function_array[3] = randomAddFunds;

	srand((int)time(0));
	/*
	while(true) {
		function_array[randomInt() % 4](app);
		std::this_thread::sleep_for(std::chrono::milliseconds(2000));
	}
	*/
	
	//register and login
	string username = "AIDude";
	string password = "aiqwerty";
	app.createUser(username, password);
	//std::this_thread::sleep_for(std::chrono::milliseconds(2000));
	app.login(username, password);
	//std::this_thread::sleep_for(std::chrono::milliseconds(2000));
	
	//Adding a fund source
	string comp = "Bad Company";
	string accNum = "1234";
	string cardType = "credit";
	string firstSource = "0";
	app.addFundSource(username, comp, accNum, cardType);
	//std::this_thread::sleep_for(std::chrono::milliseconds(2000));
	while(true)
	{
		app.addFunds(username, firstSource, 10000); //Perform basic task for the rest of time
		//std::this_thread::sleep_for(std::chrono::milliseconds(2000));
	}
	return 0;
}
