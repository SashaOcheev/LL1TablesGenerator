#include "stdafx.h"
#include "TableUtility.h"
#include <algorithm>
#include <sstream>
#include <iterator>


CToken::CToken(const std::string & token, Token type)
	: m_token(token)
	, m_type(type)
{
	if (m_type == Token::EPSILON && token != "")
	{
		throw std::domain_error("Epsilon must have empty token");
	}
}


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

CToken InitTokenByString(const std::string & str)
{
	if (str.empty() || str == "<>")
	{
		return(CToken(""));
	}
	else if (str.front() == '<')
	{
		if (str.back() != '>')
		{
			throw std::domain_error("Nonterminal must ends by \">\"");
		}
		auto nontermStr = str.substr(1, str.size() - 2);
		return(CToken(nontermStr, Token::NONTERMINAL));
	}
	return(CToken(str, Token::TERMINAL));
}

std::pair<CToken, std::vector<std::vector<CToken>>> InitGrammarLine(const std::vector<std::string>& tokens)
{
	if (tokens.size() < 3 || tokens[1] != ":")
	{
		throw std::exception("wrong format of grammar line");
	}

	auto str = tokens[0];
	auto line = std::make_pair(InitTokenByString(str), std::vector<std::vector<CToken>>());
	line.second.push_back(std::vector<CToken>());

	for (size_t i = 2; i < tokens.size(); ++i)
	{
		if (tokens[i] != "|")
		{
			tokens[i];
			line.second.back().push_back(InitTokenByString(tokens[i]));
		}
		else
		{
			line.second.push_back(std::vector<CToken>());
		}
	}
	return line;
}

std::vector<std::pair<CToken, std::vector<std::vector<CToken>>>> InitGrammar(const std::vector<std::vector<std::string>>& lines)
{
	std::vector<std::pair<CToken, std::vector<std::vector<CToken>>>> grammar;
	for (const auto &ln : lines)
	{
		grammar.push_back(InitGrammarLine(ln));
	}
	return grammar;
}

std::pair<CToken, std::vector<std::vector<CToken>>> InitGrammarLine(const std::string & tokens)
{
	std::stringstream ss(tokens);
	std::istream_iterator<std::string> begin(ss);
	std::istream_iterator<std::string> end;
	std::vector<std::string> vstrings(begin, end);

	return InitGrammarLine(vstrings);
}

std::vector<std::pair<CToken, std::vector<std::vector<CToken>>>> InitGrammar(const std::vector<std::string>& lines)
{
	std::vector<std::pair<CToken, std::vector<std::vector<CToken>>>> grammar;
	for (const auto &ln : lines)
	{
		grammar.push_back(InitGrammarLine(ln));
	}
	return grammar;
}
