//
//  common.cpp
//  TextParser
//
//  Created by Vadim Kirilin on 27.01.15.
//  Copyright (c) 2015 Vadim Kirilin. All rights reserved.
//

#include "common.h"
#include <sstream>
#include "CommonError.h"

string convert( const char c ) {
    string ret = " ";
    ret[0] = c;
    return ret;
}

string convert( const int i ) {
    std::stringstream ss;
    ss << i;
    return ss.str();
}

void assert( bool expr, const string& msg ) {
    if( !expr ) {
        IError* err = new CCommonError( msg );
        throw err;
    }
}

wstring wconvert( const wchar_t c ) {
    wstring ret = L"";
    ret.push_back(c);
    return ret;
}

void testPrint( const wstring& str ) {
#ifdef DEBUG
    wcout << str << endl;
#endif
}