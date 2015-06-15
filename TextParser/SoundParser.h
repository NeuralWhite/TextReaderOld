//
//  TextEntity.h
//  TextParser
//
//  Created by Vadim Kirilin on 30.01.15.
//  Copyright (c) 2015 Vadim Kirilin. All rights reserved.
//
#pragma once

#include "common.h"
#include "File.h"
#include <libxml/xmlreader.h>
#include "ILetter.h"
#include "Array.h"
#include "ISoundParser.h"

enum TSonorityState {
    SS_Neutral,
    SS_Sonor,
    SS_Deaf
};

enum THardnessState {
    HS_Neutral,
    HS_Hard,
    HS_Soft
};

enum TStateSaving {
    SS_Save,
    SS_Keep,
    SS_Drop
};

enum TPrediction {
    P_Vowel,
    P_Consonant,
    P_Special,
    P_Empty,
    P_Unknown
};

class CSoundParser : public ISoundParser {
public:
    CSoundParser( IFile* _file, const string& letterDictionary );
    ~CSoundParser();
    
    IArray<IRecognizedLetter *>* TranslateNextWord();
    
    void SetTranslationMode( TTranslationMode _mode ) { mode = _mode; }
    TTranslationMode GetTranslationMode() const { return mode; }

private:
    IFile* file;
    TTranslationMode mode;
    CArray<IUnrecognizedLetter*> letters;
    
    TSonorityState currSonorityState;
    THardnessState currHardnessState;
    TPrediction prediction;
    TStateSaving saving;
    
    wstring prevWord;
    wstring getNextWord();
    
    CArray<IUnrecognizedLetter*>* currentWord;
    CArray<IRecognizedLetter*>* recognizedWord;
    
    bool recognition_began;
    
    void fillCurrentWord( const wstring& word );
    
    void translationStep( int pos );
    
    IRecognizedLetter* getDefaultConsonantLetter( IUnrecognizedLetter* letter );
    IRecognizedLetter* getDefaultVowelLetter( IUnrecognizedLetter* letter );
    IRecognizedLetter* getSpecialConsonantLetter( IUnrecognizedLetter* letter );
    IRecognizedLetter* getSpecialVowelLetter( IUnrecognizedLetter* letter );
    IRecognizedLetter* getSpecialLetter( IUnrecognizedLetter* letter );
    void setStates( int pos );
    void setPrediction( int pos );
    bool letterCanBeDeaf( IUnrecognizedLetter* letter );
    bool letterCanBeSonor( IUnrecognizedLetter* letter );
    bool letterCanBeSoft( IUnrecognizedLetter* letter );
    bool letterCanBeHard( IUnrecognizedLetter* letter );
    bool isSoftingVowel( IUnrecognizedLetter* letter );
    bool checkInstallSonarity( IConsonantLetter* letter );
};

