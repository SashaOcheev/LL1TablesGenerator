#include "stdafx.h"
#include "TableUtility.h"

bool CToken::operator==(const CToken & other) const
{
	return (other.m_token == m_token && other.m_type == m_type);
}

bool CToken::operator!=(const CToken & other) const
{
	return !(*this == other);
}

Token CToken::GetType() const
{
	return m_type;
}

std::string CToken::GetToken() const
{
	return m_token;
}
