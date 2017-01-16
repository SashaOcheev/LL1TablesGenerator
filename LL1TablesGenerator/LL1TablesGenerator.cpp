// LL1TablesGenerator.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <fstream>

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
		/*MakeGrammarLine("sign-expr",{
			{ CToken("sign", Token::NONTERMINAL), CToken("expr", Token::NONTERMINAL) }
		}),
		MakeGrammarLine("expr",{
			{ CToken("paren", Token::NONTERMINAL), CToken("after", Token::NONTERMINAL) },
			{ CToken("type", Token::NONTERMINAL), CToken("after", Token::NONTERMINAL) }
		}),
		MakeGrammarLine("after",{
			{ CToken("act", Token::NONTERMINAL), CToken("after-act", Token::NONTERMINAL) },
			{ CToken("") } 
		}),
		MakeGrammarLine("after-act",{
			{ CToken("paren", Token::NONTERMINAL) },
			{ CToken("expr", Token::NONTERMINAL) }
		}),
		MakeGrammarLine("paren",{
			{ CToken("(", Token::TERMINAL), CToken("sign-expr", Token::NONTERMINAL), CToken(")", Token::TERMINAL) }
		}),
		MakeGrammarLine("act",{
			{ CToken("+", Token::TERMINAL) },
			{ CToken("-", Token::TERMINAL) },
			{ CToken("*", Token::TERMINAL) },
			{ CToken("/", Token::TERMINAL) },
			{ CToken("%", Token::TERMINAL) },
		}),
		MakeGrammarLine("type",{
			{ CToken("int", Token::TERMINAL) },
			{ CToken("id", Token::NONTERMINAL) },
			{ CToken("float", Token::TERMINAL) }
		}),
		MakeGrammarLine("id",{
			{ CToken("id", Token::TERMINAL), CToken("idx", Token::NONTERMINAL) }
		}),
		MakeGrammarLine("idx",{
			{ CToken("[", Token::TERMINAL), CToken("after-idx", Token::NONTERMINAL) },
			{ CToken("") }
		}),
		MakeGrammarLine("after-idx",{
			{ CToken("int", Token::TERMINAL), CToken("]", Token::TERMINAL) },
			{ CToken("id", Token::TERMINAL), CToken("]", Token::TERMINAL) },
		}),
		MakeGrammarLine("sign",{
			{ CToken("+", Token::TERMINAL) },
			{ CToken("-", Token::TERMINAL) },
			{ CToken("") }
		}),*/
	};

	grammar.clear();
	MakeGrammarLine("PROG", { { CToken("PROG", Token::TERMINAL), CToken("id", Token::TERMINAL), CToken("VAR", Token::NONTERMINAL), CToken("begin", Token::TERMINAL), CToken("lists", Token::NONTERMINAL), CToken("END", Token::TERMINAL), }, }),
		MakeGrammarLine("VAR", { { CToken("VAR", Token::TERMINAL), CToken("idlist", Token::NONTERMINAL) } }),
		MakeGrammarLine("idlist", { { CToken("id", Token::TERMINAL), CToken("A", Token::NONTERMINAL) } }),
		MakeGrammarLine("A", { { CToken(",", Token::TERMINAL), CToken("idlist", Token::NONTERMINAL) },{ CToken("") }, }),
		MakeGrammarLine("lists", { { CToken("st", Token::TERMINAL), CToken("B", Token::NONTERMINAL) } }),
		MakeGrammarLine("B", { { CToken("lists", Token::NONTERMINAL) },{ CToken("") } }),
	grammar = InitGrammar({
		{"<PROG>", ":", "PROG", "id", "<VAR>", "begin", "<lists>", "end"},
		{"<VAR>", ":", "VAR", "<idlist>"},
		{"<idlist>", ":","id", "<A>"},
		{"<A>", ":", ",", "<idlist>", "|", ""},
		{"<lists>", ":", "st", "<B>"},
		{"<B>", ":", "<lists>", "|", ""}
	});

	CTableGenerator table(grammar);
	auto t = table.GetTable();
	std::ofstream file;
	file.open("table.txt");
	//PrintTableForC(file, t);
	PrintTable(file, t);
	file.close();


    return 0;
}

