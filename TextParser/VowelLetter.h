//
//  VowelLetter.h
//  TextParser
//
//  Created by Vadim Kirilin on 02.02.15.
//  Copyright (c) 2015 Vadim Kirilin. All rights reserved.
//
#pragma once
#include "common.h"
#include "ILetter.h"

class CVowelLetter : public IVowelLetter {
public:
    CVowelLetter() : value(L"   ") {}
    
    virtual wstring GetLetterValue() const { return value; }
    void SetLetterValue( const wstring& _value ) { value = _value; }
    
    CVowelLetter* Copy() const
    {
        CVowelLetter* nletter = new CVowelLetter();
        nletter->value = this->value;
        return nletter;
    }
    
private:
    wstring value;
};
