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

void ALogger::log(string str)
{
	m_stream << "Log: " << str << endl;
}

// log a variable number of arguments
void ALogger::log( const char* str, va_list list)
{
	char buffer[260];

	vsnprintf(buffer, 260, str, list);

	m_stream << "Log: " << buffer << endl;

}
 
ALogger::ALogger()
{
	m_stream.open("ADebug.LOG");
	m_stream << "[ " << __DATE__ << "\t" << __TIME__ << " ] " << "log file opened..." << endl;
}

ALogger::~ALogger()
{
	m_stream << "[ " << __DATE__ << "\t" << __TIME__ << " ] " << "log file closed..." << endl;
	m_stream.close();
}

void ALOG(string str)
{
//#ifdef _DEBUG
	ALogger::instance()->log(str.c_str());
//#endif
}

void ALOG( const char* str, ...)
{
//#ifdef _DEBUG
	va_list args;
	va_start(args, str);
	ALogger::instance()->log(str, args);
	va_end(args);
//#endif
}



