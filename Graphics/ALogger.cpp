#include "ALogger.h"

// static variables
ALogger* ALogger::m_instance = 0;

// return the Logger instance
ALogger* ALogger::instance()
{
	if (!m_instance)
	{
		m_instance = new ALogger;
	}

	return m_instance;
}

void ALogger::log(wstring str)
{
	m_stream << TEXT("Log: ") << str << endl;
	//cout << "Log: " << str << endl;
}

// log a variable number of arguments
void ALogger::log( const wchar_t* str, va_list list)
{
	wchar_t buffer[260];

	_vsnwprintf_s( buffer, 260, str, list);

	m_stream << TEXT("Log: ") << buffer << endl;
	//cout << "Log: " << buffer << endl;

}
 
ALogger::ALogger()
{
	m_stream.open("Debug.LOG");
	m_stream << TEXT("[ ") << __DATE__ << TEXT("\t") << __TIME__ << TEXT(" ] ") << TEXT("log file opened...") << endl;
	//cout << "[ " << __DATE__ << "\t" << __TIME__ << " ] " << "log file opened..." << endl;
}

ALogger::~ALogger()
{
	m_stream << TEXT("[ ") << __DATE__ << TEXT("\t") << __TIME__ << TEXT(" ] ") << TEXT("log file closed...") << endl;
	m_stream.close();
}

void ALOG(wstring str)
{
//#ifdef _DEBUG
	ALogger::instance()->log(str.c_str());
//#endif
}

void ALOG( const wchar_t* str, ...)
{
//#ifdef _DEBUG
	va_list args;
	va_start(args, str);
	ALogger::instance()->log(str, args);
	va_end(args);
//#endif
}



