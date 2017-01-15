#pragma once
#include <string>
#include <set>
#include <utility>
#include <list>
#include <vector>
#include <unordered_map>
#include <iostream>

struct TableRow
{
	std::set<std::string> startSet;
	bool isShift;
	int transition;
	bool isStack;
	bool isError;
	bool isEnd;
};

void PrintTableRow(std::ostream &strm, const TableRow &tableRow);

enum class Token
{
	TERMINAL,
	NONTERMINAL,
	EPSILON,
};

class CToken
{
public:
	CToken() = delete;

	CToken(const std::string &token, Token type = Token::EPSILON)
		: m_token(token)
		, m_type(type)
	{
		if (m_type == Token::EPSILON && token != "")
		{
			throw std::domain_error("Epsilon always is empty");
		}
	};

	bool operator==(const CToken &other) const;
	bool operator!=(const CToken &other) const;

	Token GetType() const;
	std::string GetToken() const;

private:
	std::string m_token;
	Token m_type;
};
