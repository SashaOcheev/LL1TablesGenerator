#include "stdafx.h"
#include "TableUtility.h"

bool CToken::operator==(const CToken & other) const
{
	return (other.m_token == m_token && other.m_type == m_type);
}

bool CToken::operator!=(const CToken & other) const
{
	return !(*this == other);
}

Token CToken::GetType() const
{
	return m_type;
}

std::string CToken::GetToken() const
{
	return m_token;
}

void PrintTableRow(std::ostream &strm, const TableRow & tableRow)
{
	strm << tableRow.isShift << "\t";
	strm << tableRow.transition << "\t";
	strm << tableRow.isStack << "\t";
	strm << tableRow.isError << "\t";
	strm << tableRow.isEnd << "\t";
	for (const auto &str : tableRow.startSet)
	{
		strm << str << " ";
	}
	strm << std::endl;
}

void PrintTableRowForC(std::ostream & strm, const TableRow & tableRow)
{
	strm << "LL1TableString({";
	for (const auto &str : tableRow.startSet)
	{
		strm << "\"" << str << "\"" << ",";
	}
	strm << "}, "<< tableRow.isShift << ", ";
	strm << tableRow.transition << ", ";
	strm << tableRow.isStack << ", ";
	strm << tableRow.isError << ", ";
	strm << tableRow.isEnd << "),";
	strm << std::endl;
}
