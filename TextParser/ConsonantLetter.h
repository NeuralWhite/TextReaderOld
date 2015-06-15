//
//  ConsonantLetter.h
//  TextParser
//
//  Created by Vadim Kirilin on 02.02.15.
//  Copyright (c) 2015 Vadim Kirilin. All rights reserved.
//
#pragma once
#include "common.h"
#include "ILetter.h"

class CConsonantLetter : public IConsonantLetter {
public:
    CConsonantLetter() : value(L"   "), sound(CS_Unknown), type(CT_Unknown) {}
    
    wstring GetLetterValue() const { return value; }
    void SetLetterValue( const wstring& _value ) { value = _value; }
    
    virtual TConsonantType GetDefaultType() const { return type; }
    void SetDefaultType( TConsonantType _type ) { type = _type; }
    
    virtual TConsonantSound GetDefaultSound() const { return sound; }
    void SetDefaultSound( TConsonantSound _sound ) { sound = _sound; }
    
    CConsonantLetter* Copy() const
    {
        CConsonantLetter* nletter = new CConsonantLetter();
        nletter->value = this->value;
        nletter->sound = this->sound;
        nletter->type = this->type;
        return nletter;
    }
private:
    wstring value;
    TConsonantSound sound;
    TConsonantType type;
};