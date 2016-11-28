#pragma once
#include <utility>
#include "TableUtility.h"

#include <exception>
#include <unordered_map>
#include <list>
#include <algorithm>
#include <numeric>

class CTableGenerator
{
public:
	CTableGenerator(const std::vector<std::pair<CToken, std::vector<std::vector<CToken>>>> &grammar)
	{
		IsRightGrammar(grammar);

		SetTransitions(grammar);

		int i = 5;
	}

	std::vector<TableRow> GetGrammar() const
	{
		return m_table;
	}

	~CTableGenerator() = default;

protected:

	//TODO is LL1
	bool IsRightGrammar(const std::vector<std::pair<CToken, std::vector<std::vector<CToken>>>> &grammar) const
	{
		for (auto &grammar_string : grammar)
		{
			if (grammar_string.first.GetType() != Token::NONTERMINAL)
			{
				throw std::domain_error("premiss must be non-terminal");
			}
		}

		return true;
	}

	void SetTransitions(const std::vector<std::pair<CToken, std::vector<std::vector<CToken>>>> &grammar)
	{
		std::vector<size_t> premisses;
		
		//set premisses
		size_t i = 0;
		for (const auto &line : grammar)
		{
			premisses.push_back(i);
			for (const auto &j : line.second)
			{
				i += j.size() + 1;
			}
		}

		m_table.resize(i);
	}

private:
	std::vector<TableRow> m_table;
};
