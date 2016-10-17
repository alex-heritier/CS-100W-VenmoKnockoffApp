/*
 * helper_funs.cpp
 *
 *  Created on: Oct 15, 2016
 *      Author: Example
 */
#include "helper_funs.hpp"

/**
 * Converts an integral amount of cents into dollar notation
 * @param cents the amount of cents
 * @return the dollar representation of the given cents
 */
string dollarString(int cents)
{
	int dollarPart = cents / 100;
	int centPart = cents % 100;
	stringstream dollarRep;
	dollarRep << "$" << dollarPart << ".";
	if(centPart < 10)
		dollarRep << 0;
	dollarRep << centPart;
	return dollarRep.str();
}



