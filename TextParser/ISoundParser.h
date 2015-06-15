//
//  ISoundParser.h
//  TextParser
//
//  Created by Vadim Kirilin on 03.02.15.
//  Copyright (c) 2015 Vadim Kirilin. All rights reserved.
//
#include "common.h"
#include "ILetter.h"


enum TTranslationMode {
    TM_Default
};

interface ISoundParser {
    virtual IArray<IRecognizedLetter *>* TranslateNextWord() = 0;
    
    virtual void SetTranslationMode( TTranslationMode _mode ) = 0;
    virtual TTranslationMode GetTranslationMode() const = 0;
};
