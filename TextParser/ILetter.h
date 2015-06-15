//
//  ILetter.h
//  TextParser
//
//  Created by Vadim Kirilin on 31.01.15.
//  Copyright (c) 2015 Vadim Kirilin. All rights reserved.
//

#pragma once
#include "common.h"

enum TLetterType {
    LT_Consonant,
    LT_Vowel,
    LT_Unknown
};

enum TConsonantSound {
    CS_Hard,
    CS_Soft,
    CS_Unknown
};

enum TConsonantType {
    CT_Deaf,
    CT_Sonant,
    CT_Unknown
};

enum TLetterObjectType {
    LOT_Vowel,
    LOT_Consonant,
    LOT_Unrecognized,
    LOT_Pause,
    LOT_Unknown
};

interface ILetter {
    
    ILetter( TLetterObjectType _type) : type(_type) {}
    
    virtual ~ILetter() {}
    
    TLetterObjectType GetObjectType() const { return type; }
    
    virtual wstring GetLetterValue() const = 0;
    
private:
    const TLetterObjectType type;
};

interface IUnrecognizedLetter : public ILetter {
    
    IUnrecognizedLetter() : ILetter(LOT_Unrecognized) {}
    
    virtual TLetterType GetLetterType() const = 0;
    
    virtual const IArray<wstring>* GetSounds() const = 0;
    
    virtual TConsonantType GetDefaultType() const = 0;
    
    virtual TConsonantSound GetDefaultSound() const = 0;
    
    virtual IUnrecognizedLetter* Copy() const = 0;
};

interface IRecognizedLetter : public ILetter {
    IRecognizedLetter( TLetterObjectType type ) : ILetter( type ) {}
    
    virtual IRecognizedLetter* Copy() const = 0;
};

interface IVowelLetter : public IRecognizedLetter {
    IVowelLetter() : IRecognizedLetter(LOT_Vowel) {}
};

interface IConsonantLetter : public IRecognizedLetter {
    IConsonantLetter() : IRecognizedLetter(LOT_Consonant) {}
    
    virtual TConsonantType GetDefaultType() const = 0;
    
    virtual TConsonantSound GetDefaultSound() const = 0;
};

interface IPauseLetter : public ILetter {
    IPauseLetter() : ILetter(LOT_Pause) {}
    
    virtual float GetDuration() = 0;
};

template<typename Type>
Type* letter_cast( ILetter* object ) {
    TLetterObjectType object_type = object->GetObjectType();
    switch (object_type) {
        case LOT_Unrecognized: {
                Type* obj = dynamic_cast<Type*>(object);
                assert( obj != 0, "Wrong object letter cast to unrecognized letter" );
                return obj;
            }
            break;
        case LOT_Consonant: {
                Type* obj = dynamic_cast<Type*>(object);
                assert( obj != 0, "Wrong object letter cast to consonant letter" );
                return obj;
            }
            break;
        case LOT_Vowel: {
                Type* obj = dynamic_cast<Type*>(object);
                assert( obj != 0, "Wrong object letter cast to vowel letter" );
                return obj;
            }
            break;
        case LOT_Pause: {
                Type* obj = dynamic_cast<Type*>(object);
                assert( obj != 0, "Wrong object letter cast to pause letter" );
                return obj;
            }
            break;
        default:
            assert( false, "Letter cast error, cant identify object type" );
            break;
    }
    return NULL;
}
