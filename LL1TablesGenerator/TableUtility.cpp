#include "stdafx.h"
#include "TableUtility.h"

Token CToken::GetType() const
{
	return m_type;
}

std::string CToken::GetToken() const
{
	return m_token;
}
