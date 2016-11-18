
#include "NonexistentCommandException.hpp"

NonexistentCommandException::NonexistentCommandException(std::string &cmd):
	badCommand(cmd)
{

}

std::string NonexistentCommandException::getCommand() const
{
	return badCommand;
}

std::ostream& operator <<(std::ostream& out, const NonexistentCommandException& ex)
{
	out << "The command " << ex.getCommand() << " does not exist. Please look up the list of valid commands for this program.";
	return out;
}