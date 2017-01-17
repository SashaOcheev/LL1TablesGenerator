#include "stdafx.h"
#include "TableGenerator.h"
#include <fstream>

CTableGenerator::CTableGenerator(const std::vector<std::pair<CToken, std::vector<std::vector<CToken>>>>& grammar)
{
	IsRightGrammar(grammar);
	m_grammar = grammar;

	Setpremises();

	SetSize();

	SetEnd();
	SetError();
	SetStack();
	SetTransition();
	SetShift();
	//SetStartSetForTerminals();
	//SetStartSetForNonTerminals();

}

using namespace std;

std::vector<TableRow> CTableGenerator::GetTable() const
{
	return m_table;
}

//TODO is LL1
bool CTableGenerator::IsRightGrammar(const std::vector<std::pair<CToken, std::vector<std::vector<CToken>>>>& grammar) const
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

void CTableGenerator::Setpremises()
{
	std::vector<size_t> premises;

	size_t i = 0;
	for (const auto &line : m_grammar)
	{
		premises.push_back(i);
		for (const auto &j : line.second)
		{
			i += j.size() + 1;
		}
	}
	
	m_premises = premises;
}

void CTableGenerator::SetSize()
{
	if (m_premises.empty())
	{
		return;
	}

	size_t size = m_premises.back();
	for (const auto &vec : m_grammar.back().second)
	{
		size += vec.size() + 1;
	}

	m_table.resize(size);
}

void CTableGenerator::SetEnd()
{
	for (auto &i : m_table)
	{
		i.isEnd = false;
	}

	std::vector<size_t> may_end = { 0 };
	size_t i = 0;
	while (i < may_end.size())
	{
		for (size_t j = 0; j < m_grammar[may_end[i]].second.size(); ++j)
		{
			if (m_grammar[may_end[i]].second[j].back().GetType() == Token::EPSILON || m_grammar[may_end[i]].second[j].back().GetType() == Token::TERMINAL)
			{
				size_t k = m_premises[i] + j;
				for (size_t n = 0; n <= j; ++n)
				{
					k += m_grammar[may_end[i]].second[n].size();
				}
				m_table[k].isEnd = true;
			}
			else if (m_grammar[may_end[i]].second[j].back().GetType() == Token::NONTERMINAL)
			{
				//add to may_end if it has not

				//find_number
				size_t k = 0;
				for (; k < m_grammar.size(); ++k)
				{
					if (m_grammar[may_end[i]].second[j].back() == m_grammar[k].first)
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

void CTableGenerator::SetError()
{
	for (auto &i : m_table)
	{
		i.isError = true;
	}

	for (size_t i = 0; i < m_grammar.size(); ++i)
	{
		for (size_t j = 0; j < m_grammar[i].second.size(); ++j)
		{
			if (j + 1 < m_grammar[i].second.size())
			{
				size_t k = m_premises[i] + j;
				for (size_t n = 0; n < j; ++n)
				{
					k += m_grammar[i].second[n].size();
				}
				m_table[k].isError = false;
			}
		}
	}
}

void CTableGenerator::SetStack()
{
	for (auto &i : m_table)
	{
		i.isStack = false;
	}

	for (size_t i = 0; i < m_grammar.size(); ++i)
	{
		for (const auto &j : m_grammar[i].second)
		{
			for (size_t k = 0; k < j.size() - 1; ++k)
			{
				if (j[k].GetType() == Token::NONTERMINAL)
				{
					auto idx = m_premises[i] + m_grammar[i].second.size() + k;
					m_table[idx].isStack = true;
				}
			}
		}
	}
}

void CTableGenerator::SetTransition()
{
	for (size_t i = 0; i < m_grammar.size(); ++i)
	{
		for (size_t j = 0; j < m_grammar[i].second.size(); ++j)
		{

			//set premisse transitions
			size_t k = m_grammar[i].second.size();
			size_t tablePos = m_premises[i] + k;//
			for (size_t n = 0; n < j; ++n)
			{
				tablePos += m_grammar[i].second[n].size();
			}

			for (size_t n = 0; n < j; ++n)
			{
				k += m_grammar[i].second[n].size();
			}
			m_table[m_premises[i] + j].transition = k + m_premises[i];

			//set chain transition
			for (size_t tok = 0; tok < m_grammar[i].second[j].size(); ++tok)
			{
				if (m_grammar[i].second[j][tok].GetType() != Token::NONTERMINAL)
				{
					if (tok < m_grammar[i].second[j].size() - 1)
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
					for (size_t n = 0; n < m_grammar.size(); ++n)
					{
						if (m_grammar[i].second[j][tok] == m_grammar[n].first)
						{
							m_table[tablePos + tok].transition = m_premises[n];
						}
					}
				}
			}
		}

	}
}

void CTableGenerator::SetShift()
{
	for (auto &i : m_table)
	{
		i.isShift = false;
	}

	for (size_t i = 0; i < m_grammar.size(); ++i)
	{
		for (size_t j = 0; j < m_grammar[i].second.size(); ++j)
		{
			for (size_t k = 0; k < m_grammar[i].second[j].size(); ++k)
			{
				if (m_grammar[i].second[j][k].GetType() == Token::TERMINAL)
				{
					size_t pos = m_premises[i] + m_grammar[i].second.size() + k;
					for (size_t n = 0; n < j; ++n)
					{
						pos += m_grammar[i].second[n].size();
					}
					m_table[pos].isShift = true;
				}
			}
		}
	}
}

void CTableGenerator::SetStartSetForTerminals()
{
	//if non-terminal
	for (size_t i = 0; i < m_grammar.size(); ++i)
	{
		for (size_t j = 0; j < m_grammar[i].second.size(); ++j)
		{
			for (size_t k = 0; k < m_grammar[i].second[j].size(); ++k)
			{
				if (m_grammar[i].second[j][k].GetType() == Token::TERMINAL)
				{
					size_t pos = m_premises[i] + m_grammar[i].second.size() + k;
					for (size_t n = 0; n < j; ++n)
					{
						pos += m_grammar[i].second[n].size();
					}
					m_table[pos].startSet.insert(m_grammar[i].second[j][k].GetToken());
				}
			}
		}
	}
}

void CTableGenerator::SetStartSetForNonTerminals()
{
	for (size_t i = 0; i < m_grammar.size(); ++i)
	{
		if (!m_table[m_premises[i]].startSet.empty())
		{
			continue;
		}
		SetStartSetForConcreteNonTerminal(i);
	}
}

void CTableGenerator::SetStartSetForConcreteNonTerminal(size_t premisePos)
{
	for (size_t i = 0; i < m_grammar[premisePos].second.size(); ++i)
	{
		auto token = m_grammar[premisePos].second[i].front();
		if (token.GetType() == Token::TERMINAL)
		{
			m_table[m_premises[premisePos]].startSet.insert(token.GetToken());
		}
		else if (token.GetType() == Token::TERMINAL)
		{

		}
	}
}


void PrintTable(std::ostream &strm, const std::vector<TableRow>& table)
{
	for (const auto& row : table)
	{
		PrintTableRow(strm, row);
	}
}

void PrintTableForC(std::ostream & strm, const std::vector<TableRow>& table)
{
	strm << "LL1Table table1 = \n{\n";
	for (const auto& row : table)
	{
		strm << "\t";
		PrintTableRowForC(strm, row);
	}
	strm << "};\n";
}
