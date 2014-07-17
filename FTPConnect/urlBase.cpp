#include "stdafx.h"
#include "urlBase.h"
#include <sys/stat.h>

//CUrlBase

char * CUrlBase::m_bufferData = {0};

CUrlBase::CUrlBase()
{
	m_curl = NULL;
	OpenConnection();

	memset(m_errorBuffer, 0, CURLOPT_ERRORBUFFER);
	m_err = curl_easy_setopt(m_curl, CURLOPT_ERRORBUFFER, m_errorBuffer);
	CHECK_ERR();
}

CUrlBase::~CUrlBase()
{
	if(m_curl != NULL)
	{
		CloseConnection();
	}
}

void CUrlBase::CloseConnection()
{
	if(m_curl != NULL)
	{
		curl_global_cleanup();
		curl_easy_cleanup(m_curl);
	}
}

void CUrlBase::OpenConnection()
{
	if( m_curl != NULL )
		CloseConnection();

	curl_global_init(CURL_GLOBAL_ALL);
	m_curl = curl_easy_init();
}

//проверка на ошибку
void CUrlBase::CHECK_ERR()
{
	//USES_CONVERSION;

	if(m_err != CURLE_OK)
	{
		char *chErr = strcat((char*)curl_easy_strerror(m_err), "\n");
		throw std::exception(strcat(chErr, GetError()));
	}
}

//определяем, куда выводить ошибки
char * CUrlBase::GetError()
{
	//curl_easy_setopt(m_curl, CURLOPT_ERRORBUFFER, m_errorBuffer);
	return m_errorBuffer;
}

void CUrlBase::FileUpload(FILE *pFile)
{
	fseek (pFile, 0, SEEK_END);
	curl_off_t fsize = ftell(pFile);
	fseek (pFile, 0, SEEK_SET);

	m_err = curl_easy_setopt(m_curl, CURLOPT_UPLOAD, 1L);
	CHECK_ERR();
	m_err = curl_easy_setopt(m_curl, CURLOPT_READDATA, pFile);
	CHECK_ERR();
	m_err = curl_easy_setopt(m_curl, CURLOPT_INFILESIZE_LARGE,
					 (curl_off_t)fsize);
	CHECK_ERR();
}

void CUrlBase::FTPCreatMissingDir()
{
	m_err = curl_easy_setopt(m_curl, CURLOPT_FTP_CREATE_MISSING_DIRS, 1L);
	CHECK_ERR();
}

void CUrlBase::ReadBuffer()
{
	m_err = curl_easy_setopt(m_curl, CURLOPT_READFUNCTION, read_callback);
	CHECK_ERR();
}

size_t CUrlBase::read_callback(void *ptr, size_t size, size_t nmemb, void *stream)
{
  curl_off_t nread;
  /* in real-world cases, this would probably get this data differently
     as this fread() stuff is exactly what the library already would do
     by default internally */ 
  size_t retcode = fread(ptr, size, nmemb, (FILE *)stream);
 
  nread = (curl_off_t)retcode;
 
  //fprintf(stderr, "*** We read %" CURL_FORMAT_CURL_OFF_T
  //        " bytes from file\n", nread);
  return retcode;
}

//////////////////////////////////////////////////////////////////////////////
//установка прокси
void CUrlBase::Proxy(char * proxy)
{
	m_err = curl_easy_setopt(m_curl, CURLOPT_PROXY, proxy);
	//CHECK_ERR();
}

//установка URL
void CUrlBase::URL(char * url)
{
	m_err = curl_easy_setopt(m_curl, CURLOPT_URL, url);
	CHECK_ERR();
}

void CUrlBase::Header(char * header)
{
	curl_slist *pHeader = curl_slist_append(NULL, header);
	m_err = curl_easy_setopt(m_curl, CURLOPT_URL, pHeader);
	CHECK_ERR();
}

//установка POST-запроса
void CUrlBase::POSTRequest(char * request)
{
	m_err = curl_easy_setopt(m_curl, CURLOPT_POST, TRUE);
	CHECK_ERR();

	m_err = curl_easy_setopt(m_curl, CURLOPT_POSTFIELDS, request);
	CHECK_ERR();
}

//установка User-Agent
void CUrlBase::UserAgent(char * userAgent)
{
	m_err = curl_easy_setopt(m_curl, CURLOPT_USERAGENT, userAgent);
	CHECK_ERR();
}

//Установка cookie в память
void CUrlBase::SetCookieToMemory()
{
	m_err = curl_easy_setopt(m_curl, CURLOPT_COOKIEFILE, "");
	CHECK_ERR();
}

//Будет использоваться протокол HTTPS
void CUrlBase::UseHTTPS()
{
	m_err = curl_easy_setopt(m_curl, CURLOPT_SSL_VERIFYPEER, 0L);
	CHECK_ERR();

	m_err = curl_easy_setopt(m_curl, CURLOPT_SSL_VERIFYHOST, 0L);
	CHECK_ERR();
}

//Разрешение редирект-а на другую страницу
void CUrlBase::AllowRedirect()
{
	m_err = curl_easy_setopt(m_curl, CURLOPT_FOLLOWLOCATION, TRUE);
	CHECK_ERR();
}

//Выполнить transfer
bool CUrlBase::Perform()
{
	m_err = curl_easy_perform(m_curl);
	CHECK_ERR();

	return true;
}

//Код ответа сервера
void CUrlBase::GetResponseCode(long &respcode)
{
	m_err = curl_easy_getinfo(m_curl, CURLINFO_RESPONSE_CODE, &respcode);//В случае успешного выполнения операции мы получаем ответ от сервера с кодом 200
	CHECK_ERR();
}

//Текущий URL
void CUrlBase::GetCurrentURL(CString &sURL)
{
	USES_CONVERSION;

	char *url;
	m_err = curl_easy_getinfo(m_curl,CURLINFO_EFFECTIVE_URL, &url); //grabbing it from curl
	CHECK_ERR();
	sURL = A2T(url);
}

//Текущиe Cookie
CUrlBase::dataUrl* CUrlBase::GetCurrentCookie()
{
	dataUrl *cookie;
	curl_easy_getinfo(m_curl, CURLINFO_COOKIELIST, &cookie);
	CHECK_ERR();
	return cookie;
}

//Установка заголовока
void CUrlBase::FillHeader(dataUrl * pHeader, CString sVal)
{
	USES_CONVERSION;

	curl_slist_append((curl_slist*)pHeader, W2A(sVal.GetString()));
	CHECK_ERR();
}

//функция обратного вызова
int CUrlBase::writer(char *data, size_t size, size_t nmemb, char * buffer)
{
	m_bufferData = data;
	//memcpy(&m_bufferData, data, size * nmemb);
	return size * nmemb;
}

//указываем функцию обратного вызова для записи получаемых данных
void CUrlBase::WriteBuffer()
{
	m_err = curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, writer);
	CHECK_ERR();
}

//указываем куда записывать принимаемые данные
void CUrlBase::WriteData(char *bufferData)
{
	m_err = curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, bufferData);
	CHECK_ERR();
}

void CUrlBase::ConnectOnly()
{
	m_err = curl_easy_setopt(m_curl, CURLOPT_CONNECT_ONLY, 1L);
	CHECK_ERR();
}
