//
//  Letter.h
//  TextParser
//
//  Created by Vadim Kirilin on 31.01.15.
//  Copyright (c) 2015 Vadim Kirilin. All rights reserved.
//

#pragma once
#include "common.h"
#include "ILetter.h"
#include "Array.h"
#include <libxml/tree.h>

class CUnrecognizedLetter : public IUnrecognizedLetter {
public:
    explicit CUnrecognizedLetter( xmlNode* ptr );
    CUnrecognizedLetter( const CUnrecognizedLetter& other );
    CUnrecognizedLetter( const IUnrecognizedLetter* other );
    ~CUnrecognizedLetter();
    
    TLetterType GetLetterType() const { return letterType; }
    
    wstring GetLetterValue() const { return value; }
    
    const IArray<wstring>* GetSounds() const { return &sounds; }
    
    TConsonantType GetDefaultType() const { return consonantType; }
    
    TConsonantSound GetDefaultSound() const { return CS_Unknown; }
    
    CUnrecognizedLetter* Copy() const
    {
        CUnrecognizedLetter* nletter = new CUnrecognizedLetter( *this );
        return nletter;
    }
    
    CUnrecognizedLetter& operator=( const CUnrecognizedLetter& other ) {
        this->consonantSound = other.consonantSound;
        this->consonantType = other.consonantType;
        this->letterType = other.letterType;
        this->value = other.value;
        this->sounds.Clear();
        for( int i = 0; i < other.sounds.Size(); i++ )
            this->sounds.PushBack( other.GetSounds()->Item( i ) );
        return *this;
    }
    
private:
    
    TLetterType letterType;
    wstring value;
    CArray<wstring> sounds;
    TConsonantType consonantType;
    TConsonantSound consonantSound;
    
    TLetterType getType( xmlNode* ptr );
    void getSounds( xmlNode* ptr );
    void getParams( xmlNode* ptr );

    CArray<wstring> parse( const wstring& snds );
    inline wstring getProp( xmlNode* ptr, const string& name );
    xmlNode* getChildNode( xmlNode* ptr, const string& name );
};