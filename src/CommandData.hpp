
/**
 * CommandData.hpp
 * A collection of easily serializable structs for communicating between the client and server
 */

#ifndef COMMAND_DATA_HPP
#define COMMAND_DATA_HPP

// Using this instead of an enum because this can be compared against char's without casting
namespace CommandType {
	constexpr char CREATE_USER 	= '0';
	constexpr char LOGIN 	= '1';
	constexpr char PAY 		= '2';
	constexpr char ADD_FUNDS 	= '3';
}

constexpr unsigned int FIELD_SIZE = 32;	// max size for a single data field (ex. username, password, etc)
constexpr unsigned int MAX_PAYLOAD_SIZE = 1024; // max total size for a command data struct

#endif
