/*
 * FundSource.cpp
 *
 *  Created on: Oct 15, 2016
 *      Author: Example
 */

#include "FundSource.hpp"

FundSource::FundSource(): company("")
{

}

/**
 * Constructs a new FundSource associated with the given company
 * @param company the name of the company
 */
FundSource::FundSource(string company) : company(company)
{
	// TODO Auto-generated constructor stub

}

FundSource::~FundSource() {
	// TODO Auto-generated destructor stub
}

/**
 * @return a string containing information about the fund source
 */
string FundSource::toString() const
{
	return "Company: " + company;
}

/**
 * @return the name of the company in charge of this source
 */
string FundSource::getCompany() const
{
	return company;
}
