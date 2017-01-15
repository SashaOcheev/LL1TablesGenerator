// LL1TablesGenerator.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "TableGenerator.h"

using namespace std;

std::pair<CToken, std::vector<std::vector<CToken>>> MakeGrammarLine(const std::string &premiss, const std::vector<std::vector<CToken>> &chain)
{
	return std::make_pair(CToken(premiss, Token::NONTERMINAL), chain);
}


int main()
{
	std::vector<std::pair<CToken, std::vector<std::vector<CToken>>>> grammar =
	{
		MakeGrammarLine("PROG", { { CToken("PROG", Token::TERMINAL), CToken("id", Token::TERMINAL), CToken("VAR", Token::NONTERMINAL), CToken("begin", Token::TERMINAL), CToken("lists", Token::NONTERMINAL), CToken("END", Token::TERMINAL), }, }),
		MakeGrammarLine("VAR", { { CToken("VAR", Token::TERMINAL), CToken("idlist", Token::NONTERMINAL) } }),
		MakeGrammarLine("idlist", { { CToken("id", Token::TERMINAL), CToken("A", Token::NONTERMINAL) } }),
		MakeGrammarLine("A", { { CToken(",", Token::TERMINAL), CToken("idlist", Token::NONTERMINAL) }, { CToken("") }, }),
		MakeGrammarLine("lists",  { { CToken("st", Token::TERMINAL), CToken("B", Token::NONTERMINAL) } }),
		MakeGrammarLine("B", { { CToken("lists", Token::NONTERMINAL) }, { CToken("") } }),
	};

	CTableGenerator table(grammar);

	auto t = table.GetTable();
	PrintTable(std::cout, t);

    return 0;
}

