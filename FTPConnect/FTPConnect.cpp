// FTPConnect.cpp: ���������� ����� ����� ��� ����������� ����������.
//

#include "stdafx.h"
#include "FTPConnect.h"
#include "url.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ������������ ������ ����������

CWinApp theApp;

using namespace std;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	if(argc != 3)
	{
		_tprintf(_T("ERROR: You missing argument"));
		_tprintf(_T("EXAMPLE: FTPConnect.exe <file path> <ftp://user:pass@host>"));
		exit(1);
	}

	int nRetCode = 0;

	// ���������������� MFC, � ����� ������ � ��������� �� ������� ��� ����
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO. �������� ��� ������ �������������� ����� ������������
		_tprintf(_T("����������� ������: ���� ��� ������������� MFC\n"));
		nRetCode = 1;
	}
	else
	{
		CUrl cUrl;
		CString sError;
		cUrl.LoadFileToFtp(argv[1], argv[2], sError);

		if(sError.GetLength())
		{
			_tprintf(_T("ERROR:%s"), sError.GetString());
		}
		else
		{
			_tprintf(_T("File download"));
		}

		getchar();
	}

	return nRetCode;
}
