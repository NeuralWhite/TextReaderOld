//
//  TextFile.cpp
//  TextParser
//
//  Created by Vadim Kirilin on 30.01.15.
//  Copyright (c) 2015 Vadim Kirilin. All rights reserved.
//

#include "TextFile.h"
#include "Array.h"
#include <algorithm>
#include <codecvt>

const wstring punctsigns = L",.!?;:-'\"";
const wstring alphabet = L"йцукенгшщзхъфывапролджэёячсмитьбю";

CTextFile::CTextFile( const string& _dictionary ) :
    mode( PM_EraseAll )
{
    wifstream dict( _dictionary, std::ifstream::in );
    assert( dict.is_open(), "Unable to open file " + _dictionary );
    wstring word = L"";
    wstring value = L"";
    bool state = true;
    while ( !dict.eof() ) {
        if( state ) {
            dict >> word;
            std::transform(word.begin(), word.end(), word.begin(), ::tolower);
            word = trimSpaces(word);
            word = checkFirst(word);
            state = false;
        } else {
            dict >> value;
            state = true;
            words.push_back(word);
            values.push_back(value);
        }
    }
    dict.close();
}

void CTextFile::Open( const string& _filename ) {
    file.open( _filename );
    assert( file.is_open(), "Unable to open file " + _filename );
}

void CTextFile::Close() {
    file.close();
}

wstring CTextFile::eraseChar( const wstring& word, wchar_t letter ) const {
    wstring result = word;
    wstring sign = wconvert(letter);
    wstring::size_type i = 0;
    while( ( i = result.find( sign, i ) ) != result.npos ) {
        result.replace( i, sign.length(), L"" );
    }
    return result;
}

wstring CTextFile::clearAll( const wstring& word ) const {
    wstring result = word;
    for( int i = 0; i < punctsigns.length(); i++ ) {
        result = eraseChar( result, punctsigns[i] );
    }
    return result;
}

wstring CTextFile::clearSaveComma( const wstring& word ) const {
    wstring result = word;
    for( int i = 1; i < punctsigns.length(); i++ ) {
        result = eraseChar( result, punctsigns[i] );
    }
    return result;
}

wstring CTextFile::clearSaveDotsAndComma( const wstring& word ) const {
    wstring result = word;
    for( int i = 2; i < punctsigns.length(); i++ ) {
        result = eraseChar( result, punctsigns[i] );
    }
    return result;
}

wstring CTextFile::clear( const wstring& word ) const {
    wstring result = word;
    switch (mode) {
        case PM_EraseAll:
            result = clearAll( word );
            break;
        case PM_CommaOnly:
            result = clearSaveComma( word );
        case PM_DotsAndComma:
            result = clearSaveDotsAndComma( word );
        case PM_SaveAll:
            break;
        default:
            ASSERT(false);
            break;
    }
    return result;
}

wstring CTextFile::GetNextWord() {
    if( EndOfFile() ) {
        return L"";
    }
    wstring result = L"";
    file >> result;
    std::transform(result.begin(), result.end(), result.begin(), ::tolower);
    result = clear( result );
    result = trimSpaces(result);
    if( result == L"" ) {
        return GetNextWord();
    }
    return result;
}

wstring CTextFile::GetWordData( const wstring& word ) const {
    wstring key = word;
    std::transform(key.begin(), key.end(), key.begin(), ::tolower);
    key = clearAll( key );
    std::wstring_convert<std::codecvt_utf8<wchar_t>,wchar_t> convert;
    return getWord(checkFirst(key));
}

IArray<wstring>* CTextFile::GetNextSentence() {
    IArray<wstring>* array = new CArray<wstring>;
    bool isEnd = false;
    while (!isEnd) {
        wstring word = L"";
        file >> word;
        if( (word[word.length() - 1] == '.') || (word[word.length() - 1] == '!') || (word[word.length() - 1] == '?') ) {
            isEnd = true;
        }
        array->PushBack( clear( word ) );
    }
    return array;
}

wstring CTextFile::trimSpaces( const wstring& word ) const {
    return eraseChar(word, wchar_t(' '));
}

wstring CTextFile::getWord( const wstring& value ) const {
    for(int i = 0; i < words.size(); i++ ) {
        if( words[i] == value ) {
            return values[i];
        }
    }
    return L"";
}

wstring CTextFile::checkFirst( const wstring& value ) const {
    for( wstring::size_type i = 0; i < alphabet.length(); i++ ) {
        if( alphabet.find(wconvert(value[0])) != -1 ) {
            return value;
        }
    }
    wstring res = value;
    res.erase(res.begin(), res.begin() + 1);
    return res;
}



