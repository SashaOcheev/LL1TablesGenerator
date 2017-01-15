#pragma once
#include <utility>
#include "TableUtility.h"

#include <exception>
#include <unordered_map>
#include <list>
#include <stack>
#include <algorithm>

void PrintTable(const std::vector<TableRow> &table);

class CTableGenerator
{
public:
	CTableGenerator(const std::vector<std::pair<CToken, std::vector<std::vector<CToken>>>> &grammar);

	std::vector<TableRow> GetTable() const;

	~CTableGenerator() = default;

protected:

	bool IsRightGrammar(const std::vector<std::pair<CToken, std::vector<std::vector<CToken>>>> &grammar) const; //TODO is LL1
	void Setpremises();
	void SetSize();

	void SetEnd();
	void SetError();
	void SetStack();
	void SetTransition();
	void SetShift();
	void SetStartSet();


private:
	std::vector<TableRow> m_table;
	std::vector<std::pair<CToken, std::vector<std::vector<CToken>>>> m_grammar;
	std::vector<size_t> m_premises;

};
