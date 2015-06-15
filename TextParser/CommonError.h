//
//  CommonError.h
//  TextParser
//
//  Created by Vadim Kirilin on 27.01.15.
//  Copyright (c) 2015 Vadim Kirilin. All rights reserved.
//

#pragma once

#include "common.h"

class CCommonError : public IError {
public:
    CCommonError( const string& msg );
    virtual void Show();
    virtual void Delete();
private:
    const string message;
};
