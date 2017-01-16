// LL1TablesGenerator.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <fstream>

#include "TableGenerator.h"
#include "TableUtility.h"

using namespace std;

std::pair<CToken, std::vector<std::vector<CToken>>> MakeGrammarLine(const std::string &premiss, const std::vector<std::vector<CToken>> &chain)
{
	return std::make_pair(CToken(premiss, Token::NONTERMINAL), chain);
}


typedef std::vector<std::pair<CToken, std::vector<std::vector<CToken>>>> GrammarType;

GrammarType GetExampleIniziateDirectly()
{
	GrammarType grammar =
	{
		MakeGrammarLine("PROG",{ { CToken("PROG", Token::TERMINAL), CToken("id", Token::TERMINAL), CToken("VAR", Token::NONTERMINAL), CToken("begin", Token::TERMINAL), CToken("lists", Token::NONTERMINAL), CToken("END", Token::TERMINAL), }, }),
		MakeGrammarLine("VAR",{ { CToken("VAR", Token::TERMINAL), CToken("idlist", Token::NONTERMINAL) } }),
		MakeGrammarLine("idlist",{ { CToken("id", Token::TERMINAL), CToken("A", Token::NONTERMINAL) } }),
		MakeGrammarLine("A",{ { CToken(",", Token::TERMINAL), CToken("idlist", Token::NONTERMINAL) },{ CToken("") }, }),
		MakeGrammarLine("lists",{ { CToken("st", Token::TERMINAL), CToken("B", Token::NONTERMINAL) } }),
		MakeGrammarLine("B",{ { CToken("lists", Token::NONTERMINAL) },{ CToken("") } }),
	};

	return grammar;
}

GrammarType GetExampleByStringVectorVector()
{
	GrammarType grammar = InitGrammar({
		{ "<PROG>", ":", "PROG", "id", "<VAR>", "begin", "<lists>", "end" },
		{ "<VAR>", ":", "VAR", "<idlist>" },
		{ "<idlist>", ":","id", "<A>" },
		{ "<A>", ":", ",", "<idlist>", "|", "" },
		{ "<lists>", ":", "st", "<B>" },
		{ "<B>", ":", "<lists>", "|", "" }
	});

	return grammar;
}

GrammarType GetExampleByStringVector()
{
	GrammarType grammar = InitGrammar({
		"<PROG> : PROG id <VAR> begin <lists> end",
		"<VAR> : VAR <idlist>" ,
		"<idlist> : id <A>" ,
		"<A> : , <idlist> | <>" ,
		"<lists> : st <B>" ,
		"<B> : <lists> | <>"
	});

	return grammar;
}


GrammarType GetArithmetics()
{
	GrammarType grammar = InitGrammar({
		"<sign-expr> : <sign> <expr>",
		"<expr> : <paren> <after> | <type> <after>",
		"<after> : <act> <expr> | <>",
		"<paren> : ( <sign-expr> )",
		"<act> : / | * | % | * | - | +",
		"<type> : int | float | <id>",
		"<id> : id <idx>",
		"<idx> : [ <after-idx>",
		"<after-idx> : int ] | id ]",
		"<sign> : + | - | <>"
	});

	return grammar;
}


int main()
{
	auto grammar = GetArithmetics();

	CTableGenerator table(grammar);
	auto t = table.GetTable();
	std::ofstream file;
	file.open("table.txt");
	PrintTableForC(file, t);
	//PrintTable(file, t);
	file.close();


    return 0;
}

