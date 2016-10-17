
#ifndef NONEXISTENT_COMMAND_EXCEPTION_HPP
#define NONEXISTENT_COMMAND_EXCEPTION_HPP

#include <string>

class NonexistentCommandException {
public:
	NonexistentCommandException(std::string &);
	std::string getCommand();
private:
	std::string badCommand;
};

#endif
