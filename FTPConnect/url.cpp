#include "stdafx.h"
#include "url.h"

using namespace std;
//CUrl

CUrl::CUrl()
{
}

CUrl::~CUrl()
{
}

void CUrl::LoadFileToFtp(CString sFilePath, CString sAddressFTP , CString &sError)
{
	USES_CONVERSION;

	CString sFileName = sFilePath.Right(sFilePath.GetLength() - sFilePath.ReverseFind('\\')-1);

	if(sAddressFTP.Right(1) != _T("/"))
	{
		sAddressFTP.Append(_T("/"));
	}

	sAddressFTP.Append(sFileName);

	FILE *pFile;
	try
	{
		pFile = fopen(W2A(sFilePath.GetString()), "rb");
		if(!pFile)
		{
			throw exception("Can't open file.");
		}

		CUrlBase cUrlBase;
		cUrlBase.FileUpload(pFile);
		cUrlBase.URL(W2A(sAddressFTP.GetString()));
		cUrlBase.FTPCreatMissingDir();
		cUrlBase.Perform();

		fclose(pFile);
	}
	catch(exception &e)
	{
		fclose(pFile);
		sError = e.what();
	}
}