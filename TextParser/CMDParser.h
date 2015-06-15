//
//  CMDParser.h
//  TextParser
//
//  Created by Vadim Kirilin on 27.01.15.
//  Copyright (c) 2015 Vadim Kirilin. All rights reserved.
//
#pragma once

#include "common.h"

class CCMDParser {
public:
    CCMDParser( int argc, const char* argv[], vector<Pair<unsigned long, string>>& names );
    bool CheckFlag( unsigned long value ) const;
    bool CheckFlag( const string& name ) const;
    vector<string> GetArgs() const;
    
private:
    mutable vector<string> flags;
    mutable vector<string> args;
    vector<Pair<unsigned long, string>> flagNames;
    mutable unsigned long num_flags;

    void parseFlags( int argc, const char* argv[] ) const;
    unsigned long parseWord( const char word[] ) const;
};