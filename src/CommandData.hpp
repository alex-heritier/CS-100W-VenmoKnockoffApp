
/**
 * CommandData.hpp
 * A collection of easily serializable structs for communicating between the client and server
 */

#ifndef COMMAND_DATA_HPP
#define COMMAND_DATA_HPP

#include <iostream>

// Using this instead of an enum because this can be compared against char's without casting
namespace CommandType {
	const char CREATE_USER 	= '0';
	const char LOGIN 	= '1';
	const char PAY 		= '2';
	const char ADD_FUNDS 	= '3';
}

const unsigned int FIELD_SIZE = 32;	// max size for a single data field (ex. username, password, etc)
const unsigned int MAX_PAYLOAD_SIZE = 1024; // max total size for a command data struct

#endif
