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
		"<act> : / | * | % | - | +",
		"<type> : int | float | <id>",
		"<id> : id <idx>",
		"<idx> : [ <after-idx> | <>",
		"<after-idx> : int ] | id ]",
		"<sign> : + | - | <>"
	});

	return grammar;
}


GrammarType GetFinal()
{
	GrammarType grammar = InitGrammar({
		"<s> : <function_list> MAIN <construction>",
		"<construction> : OPEN <construction_app>",
		"<construction_app> : <statement_list> CLOSE | CLOSE",
		"<function_list> : <function> <function_list> | <>",
		"<function> : FUNCTION FUNCTIONID ( <argument_list> ) <construction> | NEWLINE",
		"<fuction_call> : FUNCTIONID ( <list_row> )",
		"<argument_list> : <argument_list_app> | <>",
		"<argument_list_app> : ID <argument_list_app_app>",
		"<argument_list_app_app> : , <argument_list> | <>",
		"<statement_list> : <statement> <statement_list> | <>",
		"<statement> : <if_statement> | <while_statement> | READ ( <data> ) NEWLINE | PRINT ( <data> ) NEWLINE | RETURN ( <data> ) NEWLINE | <data> <all_expression> NEWLINE | <const_assignment> NEWLINE | <list_creation> NEWLINE | NEWLINE",
		"<insert_bool> : <data> <bool_expression>",
		"<if_statement> : <one_if_statement> <if_statement_app>",
		"<one_if_statement> : IF ( <insert_bool> ) <construction>",
		"<if_statement_app> : <elif_statement> <else_statement>",
		"<elif_statement> : ELIF ( <insert_bool> ) <construction> <elif_statement> | <>",
		"<else_statement> : ELSE <construction> | <>",
		"<while_statement> : WHILE ( <insert_bool> ) <construction> | DO <construction> WHILE ( <insert_bool> ) NEWLINE",
		"<paren_expr> : ( <expression> )",
		"<all_expression> : <expression_app> | <assignment_expression>",
		"<expression> : <data> <expression_app>",
		"<expression_app> : <arithmetic_expression> | <bool_expression>",
		"<arithmetic_expression> : <arithmetic_act> <data> <arithmetic_expression> | <>",
		"<arithmetic_act> : + | - | * | / | %",
		"<bool_expression> : <bool_act> <data> <bool_expression> | ! <bool_expression> | <>",
		"<bool_act> : MORE | LESS | MOREOREQUAL | LESSOREQUAL | == | != | And | Or",
		"<assignment_expression> : <assignment_expression_app> <data>",
		"<const_assignment> : CONST ID = <data>",
		"<assignment_expression_app> : = | += | -= | /= | *= | %=",
		"<insert_arith> : <data> <arithmetic_expression>",
		"<list_creation> : LIST ~ <insert_arith> ~ OF <list_creation_app> ID <list_assign>",
		"<list_creation_app> : <list_creation> | OF TYPENAME",
		"<list_assign> : = <init_list_row> | <>",
		"<init_list_row> : LESS <list_row> MORE",
		"<list_row> : <data> <list_row_app>",
		"<list_row_app> : , <list_row> | <>",
		"<index> : ID <multidimensional_index>",
		"<multidimensional_index> : [ <insert_arith> ] <multidimensional_index> | <>",
		"<data> : <atomic_type> | <index> | <paren_expr> | <init_list_row> | <fuction_call>",
		"<atomic_type> : <numeric_type> | CHAR | BOOL",
		"<numeric_type> : INT | FLOAT",
	});

	return grammar;
}

int main()
{
	//auto grammar = GetArithmetics();
	auto grammar = GetFinal();

	CTableGenerator table(grammar);
	auto t = table.GetTable();
	std::ofstream file;
	file.open("table.txt");
	//PrintTableForC(file, t);
	//PrintTable(file, t);
	PrintTable(file, t);
	file.close();


    return 0;
}

