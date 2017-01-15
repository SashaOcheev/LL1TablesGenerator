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
	CTableGenerator(const std::vector<std::pair<CToken, std::vector<std::vector<CToken>>>> &grammar);

	std::vector<TableRow> GetTable() const;

	~CTableGenerator() = default;

protected:

	bool IsRightGrammar(const std::vector<std::pair<CToken, std::vector<std::vector<CToken>>>> &grammar) const; //TODO is LL1
	std::vector<size_t> GetPremisses(const std::vector<std::pair<CToken, std::vector<std::vector<CToken>>>> &grammar) const;
	void SetSize(const std::vector<std::pair<CToken, std::vector<std::vector<CToken>>>> &grammar, const std::vector<size_t> &premisses);

	void SetEnd(const std::vector<std::pair<CToken, std::vector<std::vector<CToken>>>> &grammar, const std::vector<size_t> &premisses);
	void SetError(const std::vector<std::pair<CToken, std::vector<std::vector<CToken>>>> &grammar, const std::vector<size_t> &premisses);
	void SetStack(const std::vector<std::pair<CToken, std::vector<std::vector<CToken>>>> &grammar, const std::vector<size_t> &premisses);
	void SetTransition(const std::vector<std::pair<CToken, std::vector<std::vector<CToken>>>> &grammar, const std::vector<size_t> &premisses);
	void SetShift(const std::vector<std::pair<CToken, std::vector<std::vector<CToken>>>> &grammar, const std::vector<size_t> &premisses);


private:
	std::vector<TableRow> m_table;
};
