#pragma once

#include <stdlib.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <Windows.h>
#include <stdarg.h>

using namespace std;

void ALOG( string str );
void ALOG(const char* str, ...);


class ALogger
{
public:

    static ALogger* instance();
    
	void log(string str);
	void log(const char* str, va_list list);
    
private:

    ALogger();
	~ALogger(); 
    
    // static variables
    static ALogger *m_instance;
    ofstream m_stream;
 
};

 

