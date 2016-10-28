
#include <iostream>
#include <string>
#include <ctime>
#include <cstdlib>

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
	char buf[(randomInt() % (FIELD_SIZE - 6)) + 6];
	randomizeString(buf, (randomInt() % (FIELD_SIZE - 6)) + 6);
	string username = string(buf, FIELD_SIZE); // (randomInt() % ((randomInt() % (FIELD_SIZE - 6)) + 6 - 6)) + 6
	randomizeString(buf, (randomInt() % (FIELD_SIZE - 6)) + 6);
	string password = string(buf, FIELD_SIZE);

	app.createUser(username, password);
}

void randomLogin(VKAClient &app)
{
        char buf[(randomInt() % (FIELD_SIZE - 6)) + 6];
        randomizeString(buf, (randomInt() % (FIELD_SIZE - 6)) + 6);
        string username = string(buf, FIELD_SIZE);
        randomizeString(buf, (randomInt() % (FIELD_SIZE - 6)) + 6);
        string password = string(buf, FIELD_SIZE);

        app.login(username, password);
}

void randomPay(VKAClient &app)
{
        char buf[(randomInt() % (FIELD_SIZE - 6)) + 6];
        randomizeString(buf, (randomInt() % (FIELD_SIZE - 6)) + 6);
        string username = string(buf, FIELD_SIZE);
        int amount = randomInt();

        app.pay(username, amount);
}

void randomAddFunds(VKAClient &app)
{
	char buf[(randomInt() % (FIELD_SIZE - 6)) + 6];
	randomizeString(buf, (randomInt() % (FIELD_SIZE - 6)) + 6);
        string fundTag = string(buf, FIELD_SIZE);
        int amount = randomInt();

        app.addFunds(fundTag, amount);
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
	while(true) {
		function_array[randomInt() % 4](app);
	}
	return 0;
}
