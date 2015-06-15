//
//  Error.h
//  TextParser
//
//  Created by Vadim Kirilin on 27.01.15.
//  Copyright (c) 2015 Vadim Kirilin. All rights reserved.
//
#pragma once

#include "common.h"

interface IError {
    virtual void Show() = 0;
    virtual void Delete() = 0;
};



