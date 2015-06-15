//
//  CommonError.cpp
//  TextParser
//
//  Created by Vadim Kirilin on 27.01.15.
//  Copyright (c) 2015 Vadim Kirilin. All rights reserved.
//

#include "CommonError.h"
#include "common.h"

CCommonError::CCommonError( const string& msg ) : message( msg )
{}

void CCommonError::Delete() {
    delete this;
}

void CCommonError::Show() {
#ifndef DEBUG
    cerr << message << endl;
#else
    cout << message << endl;
#endif
}
