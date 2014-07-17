#pragma once

#include "urlBase.h"

class CUrl
{
private:
	typedef CUrl thisClass;

public:

	CUrl();
	~CUrl();

	void LoadFileToFtp(CString sFilePath, CString sAddressFTP, CString &sError);
};
