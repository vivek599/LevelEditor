#pragma once

#include <stdlib.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <Windows.h>
#include <stdarg.h>

using namespace std;

void ALOG( string str );
void ALOG(const wchar_t* str, ...);


class ALogger
{
public:

    static ALogger* instance();
    
	void log(string str);
	void log(const wchar_t* str, va_list list);
    
private:

    ALogger();
	~ALogger(); 
    
    // static variables
    static ALogger *m_instance;
    ofstream m_stream;
 
};

 

