#pragma once

#include <stdlib.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <Windows.h>
#include <stdarg.h>

using namespace std;

void ALOG( wstring str );
void ALOG(const wchar_t* str, ...);


class ALogger
{
public:

    static ALogger* instance();
    
	void log(wstring str);
	void log(const wchar_t* str, va_list list);
    
private:

    ALogger();
	~ALogger(); 
    
    // static variables
    static ALogger *m_instance;
    wofstream m_stream;
 
};

 

