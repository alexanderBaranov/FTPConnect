#include "stdafx.h"

#pragma once

#include <curl/curl.h>

class CUrlBase
{
private:
	typedef CUrlBase thisClass;

public:

	CUrlBase();
	~CUrlBase();

	typedef curl_slist dataUrl;

	void CUrlBase::FileUpload(FILE *pFile);
	void FTPCreatMissingDir();
	void ReadBuffer();
	static size_t read_callback(void *ptr, size_t size, size_t nmemb, void *stream);

	void OpenConnection();
	void CloseConnection();	
	void Proxy(char * proxy);
	void URL(char * url);
	void POSTRequest(char * request);
	void UserAgent(char * userAgent);
	void SetCookieToMemory();
	void UseHTTPS();
	void Header(char * header);
	void AllowRedirect();
	bool Perform();
	void GetResponseCode(long &respcode);
	void GetCurrentURL(CString &sURL);
	dataUrl* GetCurrentCookie();
	void FillHeader(dataUrl * pHeader, CString sVal);
	void CHECK_ERR();
	char * GetError();
	void WriteBuffer();
	void WriteData(char *bufferData);
	void RecvData(CString &sRecvData);
	void ConnectOnly();
	static int writer(char *data, size_t size, size_t nmemb, char * buffer);

	CURL * m_curl;
	CURLcode m_err;
	CString m_tstrProxyFromIE;
	char m_errorBuffer[CURLOPT_ERRORBUFFER];
	static char * m_bufferData;
};