//
//  TextFile.h
//  TextParser
//
//  Created by Vadim Kirilin on 30.01.15.
//  Copyright (c) 2015 Vadim Kirilin. All rights reserved.
//

#pragma once

#include "common.h"
#include "File.h"

#include <fstream>
#include <map>
using std::wifstream;
using std::map;

class CTextFile : public IFile {
public:
    CTextFile( const string& _dictionary );
    
    void Open( const string& _filename );
    void Close();
    
    wstring GetNextWord();
    
    wstring GetWordData( const wstring& word ) const;
    
    IArray<wstring>* GetNextSentence();
    
    void SetPunctuationMode( TPunctuationMode _mode ) { mode = _mode; }
    TPunctuationMode GetPunctuationMode() const { return mode; }
    
    bool EndOfFile() const { return file.eof(); }
private:
    vector<wstring> words;
    vector<wstring> values;
    wifstream file;
    
    TPunctuationMode mode;
    
    void parseDictionary();
    wstring clear( const wstring& word ) const;
    wstring clearAll( const wstring& word ) const;
    wstring clearSaveComma( const wstring& word ) const;
    wstring clearSaveDotsAndComma( const wstring& word ) const;
    wstring eraseChar( const wstring& word, wchar_t letter ) const;
    wstring trimSpaces( const wstring& word ) const;
    wstring getWord( const wstring& value ) const;
    wstring checkFirst( const wstring& value ) const;
};