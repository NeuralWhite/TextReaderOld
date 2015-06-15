//
//  CMDParser.cpp
//  TextParser
//
//  Created by Vadim Kirilin on 27.01.15.
//  Copyright (c) 2015 Vadim Kirilin. All rights reserved.
//

#include "CMDParser.h"
#include "common.h"

unsigned long CCMDParser::parseWord( const char word[] ) const {
    unsigned long flags = 0;
    if( word[0] != '-' )
        return flags;
    if( word[1] != '-') {
        for( int i = 1; word[i] != '\0'; i++ ) {
            for( int j = 0; j < flagNames.size(); j++ ) {
                if( flagNames[j].b() == convert( word[i] ) ) {
                    flags |= flagNames[j].a();
                }
            }
        }
    } else {
        string flag = word;
        flag.erase(flag.begin(), flag.begin() + 2);
        for( int j = 0; j < flagNames.size(); j++ ) {
            if( flagNames[j].b() == flag ) {
                flags |= flagNames[j].a();
            }
        }
    }
    return flags;
}

void CCMDParser::parseFlags( int argc, const char* argv[] ) const {
    ASSERT( argv != 0 );
    for( int i = 1; i < argc; i++ ) {
        unsigned long flag_value = parseWord( argv[i] );
        if( flag_value == 0 ) {
            args.push_back( argv[i] );
        }
        num_flags |= flag_value;
    }
    for( int i = 0; i < flagNames.size(); i++ ) {
        if( (flagNames[i].a() & num_flags) == flagNames[i].a() ) {
            flags.push_back( flagNames[i].b() );
        }
    }
}

CCMDParser::CCMDParser( int argc, const char* argv[], vector<Pair<unsigned long, string>>& names ) :
    flagNames(names),
    num_flags(0) {
        parseFlags(argc, argv);
}

vector<string> CCMDParser::GetArgs() const {
    return args;
}

bool CCMDParser::CheckFlag(unsigned long value) const {
    return (num_flags & value) == value;
}

bool CCMDParser::CheckFlag(const string &name) const {
    for( int i = 0; i < flags.size(); i++ ) {
        if( name == flags[i] )
            return true;
    }
    return false;
}
    
