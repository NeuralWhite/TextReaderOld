//
//  File.h
//  TextParser
//
//  Created by Vadim Kirilin on 30.01.15.
//  Copyright (c) 2015 Vadim Kirilin. All rights reserved.
//

#pragma once
#include "common.h"


enum TPunctuationMode {
    PM_EraseAll,
    PM_SaveAll,
    PM_CommaOnly,
    PM_DotsAndComma
};

interface IFile {
    virtual void Open( const string& _filename ) = 0;
    virtual void Close() = 0;
    
    virtual wstring GetNextWord() = 0;
    
    virtual wstring GetWordData( const wstring& word ) const = 0;
    
    virtual IArray<wstring>* GetNextSentence() = 0;
    
    virtual void SetPunctuationMode( TPunctuationMode _mode ) = 0;
    virtual TPunctuationMode GetPunctuationMode() const = 0;
    
    virtual bool EndOfFile() const = 0;
};