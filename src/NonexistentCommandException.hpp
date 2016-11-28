
#ifndef NONEXISTENT_COMMAND_EXCEPTION_HPP
#define NONEXISTENT_COMMAND_EXCEPTION_HPP

#include <string>
#include <iostream>
class NonexistentCommandException {
public:
	NonexistentCommandException(std::string &);
	std::string getCommand() const;
	friend std::ostream& operator <<(std::ostream& out, const NonexistentCommandException& ex);
private:
	std::string badCommand;
};

#endif
