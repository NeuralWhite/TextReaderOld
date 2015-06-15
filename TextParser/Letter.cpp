//
//  Letter.cpp
//  TextParser
//
//  Created by Vadim Kirilin on 31.01.15.
//  Copyright (c) 2015 Vadim Kirilin. All rights reserved.
//

#include "Letter.h"

#include <libxml/parser.h>
#include <libxml/tree.h>
#include <codecvt>

const char* soundName = "sound";
const char* letterTypeName = "type";
const char* soundValueName = "value";
const char* letterNameName = "name";
const char* defaultConsonantType = "default";
const wchar_t soundSeparator = '|';

CUnrecognizedLetter::CUnrecognizedLetter( xmlNode* ptr ) :
    consonantType( CT_Unknown ),
    consonantSound( CS_Unknown )
{
    ASSERT( ptr != 0 );
    value = getProp( ptr, letterNameName );
    letterType = getType( ptr );
    getSounds( ptr );
    getParams( ptr );
}

CUnrecognizedLetter::CUnrecognizedLetter( const CUnrecognizedLetter& other ) {
    this->consonantSound = other.consonantSound;
    this->consonantType = other.consonantType;
    this->letterType = other.letterType;
    this->value = other.value;
    this->sounds.Clear();
    for( int i = 0; i < other.sounds.Size(); i++ )
        this->sounds.PushBack( other.GetSounds()->Item( i ) );
}

CUnrecognizedLetter::CUnrecognizedLetter( const IUnrecognizedLetter* other ) {
    ASSERT( other != NULL )
    this->consonantSound = other->GetDefaultSound();
    this->consonantType = other->GetDefaultType();
    this->letterType = other->GetLetterType();
    this->value = other->GetLetterValue();
    for( int i = 0; i < other->GetSounds()->Size(); i++ )
        this->sounds.PushBack( other->GetSounds()->Item( i ) );
}

CUnrecognizedLetter::~CUnrecognizedLetter() {
}

TLetterType CUnrecognizedLetter::getType( xmlNode* ptr ) {
    wstring value = getProp( ptr, letterTypeName );
    if( value == L"o" )
        return LT_Vowel;
    if( value == L"c" )
        return LT_Consonant;
    assert( false, "Wrong letter type" );
    return LT_Vowel;
};

void CUnrecognizedLetter::getSounds( xmlNode* ptr ) {
    xmlNode* child = getChildNode( ptr, soundName );
    ASSERT( child != 0 )
    wstring value = getProp( child, soundValueName );
    sounds = parse( value );
}

void CUnrecognizedLetter::getParams( xmlNode* ptr ) {
    if( letterType == LT_Consonant ) {
        wstring def = getProp( ptr, defaultConsonantType );
        if( def == L"s" )
            consonantType = CT_Sonant;
        if( def == L"d" )
            consonantType = CT_Deaf;
        consonantSound = CS_Unknown;
    }
}

CArray<wstring> CUnrecognizedLetter::parse( const wstring& snds ) {
    wstring accumulator = L"";
    CArray<wstring> array;
    for( int i = 0; i < snds.length(); i++ ) {
        if( snds[i] != soundSeparator ) {
            accumulator.push_back( snds[i] );
        } else {
            array.PushBack( accumulator );
            accumulator = L"";
        }
    }
    if( accumulator != L"" ) {
        accumulator.shrink_to_fit();
        array.PushBack( accumulator );
    }
    return array;
}

wstring CUnrecognizedLetter::getProp( xmlNode* ptr, const string& name ) {
    ASSERT( ptr != 0 )
    xmlChar* value = xmlGetProp( ptr, (const xmlChar*)(name.c_str()) );
    try {
        std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> conv;
        return conv.from_bytes( (const char*)value );
    }
    catch (std::range_error& error) {
        ASSERT( false );
    }
    return L"";
}

xmlNode* CUnrecognizedLetter::getChildNode( xmlNode* ptr, const string& name ) {
    ASSERT( ptr != 0 )
    xmlNode* ret = ptr->children;
    if( ret == NULL )
        return ret;
    while( ret ) {
        if( ( ret->type == XML_ELEMENT_NODE ) && !xmlStrcmp( ret->name, (const xmlChar*)name.c_str() ) ) {
            break;
        }
        ret = ret->next;
    }
    return ret;
}
