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
		SetEnd(grammar, premisses);
		SetError(grammar, premisses);
		SetStack(grammar, premisses);
		SetTransition(grammar, premisses);
		SetShift(grammar, premisses);
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

	void SetSize(const std::vector<std::pair<CToken, std::vector<std::vector<CToken>>>> &grammar, const std::vector<size_t> &premisses)
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

	void SetEnd(const std::vector<std::pair<CToken, std::vector<std::vector<CToken>>>> &grammar, const std::vector<size_t> &premisses)
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

	void SetError(const std::vector<std::pair<CToken, std::vector<std::vector<CToken>>>> &grammar, const std::vector<size_t> &premisses)
	{
		for (auto &i : m_table)
		{
			i.isError = true;
		}

		for (size_t i = 0; i < grammar.size(); ++i)
		{
			for (size_t j = 0; j < grammar[i].second.size(); ++j)
			{
				if (j + 1 < grammar[i].second.size())
				{
					size_t k = premisses[i] + j;
					for (size_t n = 0; n < j; ++n)
					{
						k += grammar[i].second[n].size();
					}
					m_table[k].isError = false;
				}
			}
		}
	}

	void SetStack(const std::vector<std::pair<CToken, std::vector<std::vector<CToken>>>> &grammar, const std::vector<size_t> &premisses)
	{
		for (auto &i : m_table)
		{
			i.isStack = false;
		}

		for (size_t i = 0; i < grammar.size(); ++i)
		{
			for (const auto &j : grammar[i].second)
			{
				for (size_t k = 0; k < j.size() - 1; ++k)
				{
					if (j[k].GetType() == Token::NONTERMINAL)
					{
						m_table[premisses[i] + grammar[i].second.size() + k].isStack = true;
					}
				}
			}
		}
	}

	void SetTransition(const std::vector<std::pair<CToken, std::vector<std::vector<CToken>>>> &grammar, const std::vector<size_t> &premisses)
	{
		for (size_t i = 0; i < grammar.size(); ++i)
		{
			for (size_t j = 0; j < grammar[i].second.size(); ++j)
			{

				//set premisse transitions
				size_t k = grammar[i].second.size();
				size_t tablePos = premisses[i] + k;//
				for (size_t n = 0; n < j; ++n)
				{
					tablePos += grammar[i].second[n].size();
				}

				for (size_t n = 0; n < j; ++n)
				{
					k += grammar[i].second[n].size();
				}
				m_table[premisses[i] + j].transition = k + premisses[i];

				//set chain transition
				for (size_t tok = 0; tok < grammar[i].second[j].size(); ++tok)
				{
					if (grammar[i].second[j][tok].GetType() != Token::NONTERMINAL)
					{
						if (tok < grammar[i].second[j].size() - 1)
						{
							m_table[tablePos + tok].transition = tablePos + tok + 1;
						}
						else
						{
							m_table[tablePos + tok].transition = -1;
						}
					}
					else
					{
						//find in premiss
						for (size_t n = 0; n < grammar.size(); ++n)
						{
							if (grammar[i].second[j][tok] == grammar[n].first)
							{
								m_table[tablePos + tok].transition = premisses[n];
							}
						}
					}
				}
			}

		}
	}

	void SetShift(const std::vector<std::pair<CToken, std::vector<std::vector<CToken>>>> &grammar, const std::vector<size_t> &premisses)
	{
		for (auto &i : m_table)
		{
			i.isShift = false;
		}

		for (size_t i = 0; i < grammar.size(); ++i)
		{
			for (size_t j = 0; j < grammar[i].second.size(); ++j)
			{
				for (size_t k = 0; k < grammar[i].second[j].size(); ++k)
				{
					if (grammar[i].second[j][k].GetType() == Token::TERMINAL)
					{
						size_t pos = premisses[i] + grammar[i].second.size() + k;
						for (size_t n = 0; n < j; ++n)
						{
							pos += grammar[i].second[n].size();
						}
						m_table[pos].isShift = true;
					}
				}
			}
		}
	}


private:
	std::vector<TableRow> m_table;
};
