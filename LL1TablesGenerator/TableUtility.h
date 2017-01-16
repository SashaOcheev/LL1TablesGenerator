#pragma once
#include <string>
#include <set>
#include <utility>
#include <list>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <string>

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
void PrintTableRowForC(std::ostream &strm, const TableRow &tableRow);

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

	CToken(const std::string &token, Token type = Token::EPSILON);

	bool operator==(const CToken &other) const;
	bool operator!=(const CToken &other) const;

	Token GetType() const;
	std::string GetToken() const;

private:
	std::string m_token;
	Token m_type;
};

//"<NONTERMINAL>", "TERMINAL", "" - epsilon 
CToken InitTokenByString(const std::string &str);

std::pair<CToken, std::vector<std::vector<CToken>>> InitGrammarLine(const std::vector<std::string> &tokens);

std::vector<std::pair<CToken, std::vector<std::vector<CToken>>>> InitGrammar(const std::vector < std::vector<std::string>> &lines);