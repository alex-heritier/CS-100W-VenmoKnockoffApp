
#include "NonexistentCommandException.hpp"

NonexistentCommandException::NonexistentCommandException(std::string &cmd):
	badCommand(cmd)
{

}

std::string NonexistentCommandException::getCommand()
{
	return badCommand;
}
