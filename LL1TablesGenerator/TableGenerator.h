#pragma once
#include <utility>
#include "TableUtility.h"

#include <exception>
#include <unordered_map>
#include <list>
#include <stack>
#include <algorithm>

class CTableGenerator
{
public:
	CTableGenerator(const std::vector<std::pair<CToken, std::vector<std::vector<CToken>>>> &grammar)
	{
		IsRightGrammar(grammar);

		auto premisses = GetPremisses(grammar);

		SetSize(grammar, premisses);
		SetEnds(grammar, premisses);

		int i = 5;
	}

	std::vector<TableRow> GetTable() const
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

	std::vector<size_t> GetPremisses(const std::vector<std::pair<CToken, std::vector<std::vector<CToken>>>> &grammar) const
	{
		std::vector<size_t> premisses;

		size_t i = 0;
		for (const auto &line : grammar)
		{
			premisses.push_back(i);
			for (const auto &j : line.second)
			{
				i += j.size() + 1;
			}
		}
		
		return premisses;
	}

	void SetSize(const std::vector<std::pair<CToken, std::vector<std::vector<CToken>>>> &grammar, std::vector<size_t> premisses)
	{
		if (premisses.empty())
		{
			return ;
		}

		size_t size = premisses.back();
		for (const auto &vec : grammar.back().second)
		{
			size += vec.size() + 1;
		}

		m_table.resize(size);
	}

	void SetEnds(const std::vector<std::pair<CToken, std::vector<std::vector<CToken>>>> &grammar, std::vector<size_t> premisses)
	{
		for (auto &i : m_table)
		{
			i.isEnd = false;
		}

		std::vector<size_t> may_end = { 0 };
		size_t i = 0;
		while (i < may_end.size())
		{
			for (size_t j = 0; j < grammar[may_end[i]].second.size(); ++j)
			{
				if (grammar[may_end[i]].second[j].back().GetType() == Token::EPSILON || grammar[may_end[i]].second[j].back().GetType() == Token::TERMINAL)
				{
					size_t k = premisses[i] + j;
					for (size_t n = 0; n <= j; ++n)
					{
						k += grammar[may_end[i]].second[n].size();
					}
					m_table[k].isEnd = true;
				}
				else if (grammar[may_end[i]].second[j].back().GetType() == Token::NONTERMINAL)
				{
					//add to may_end if it has not
					
					//find_number
					size_t k = 0;
					for (; k < grammar.size(); ++k)
					{
						if (grammar[may_end[i]].second[j].back() == grammar[k].first)
						{
							break;
						}
					}

					//find in may_end
					if (std::find(may_end.begin(), may_end.end(), k) != may_end.end())
					{
						may_end.push_back(k);
					}
				}
			}
			++i;
		}
	}

private:
	std::vector<TableRow> m_table;
};
