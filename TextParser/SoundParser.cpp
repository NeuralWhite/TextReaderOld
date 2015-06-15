//
//  TextEntity.cpp
//  TextParser
//
//  Created by Vadim Kirilin on 30.01.15.
//  Copyright (c) 2015 Vadim Kirilin. All rights reserved.
//

#include "SoundParser.h"

#include <libxml/parser.h>
#include <libxml/tree.h>

#include "Letter.h"
#include "ILetter.h"
#include "ConsonantLetter.h"
#include "VowelLetter.h"

const char* letterName = "letter";

CSoundParser::CSoundParser( IFile* _file, const string& letterDictionary ) :
    currHardnessState( HS_Neutral ),
    currSonorityState( SS_Neutral ),
    saving( SS_Drop ),
    currentWord( NULL ),
    recognizedWord( NULL ),
    prediction( P_Unknown ),
    recognition_began( false ),
    prevWord(L"")
{
    ASSERT( _file != 0 )
    file = _file;
    xmlInitParser();
    LIBXML_TEST_VERSION
    xmlDoc* doc = xmlReadFile( letterDictionary.c_str(), NULL, 0 );
    assert( doc != 0, "Problem with file " + letterDictionary );
    xmlNode* root = xmlDocGetRootElement( doc );
    ASSERT( root != 0 );
    for( xmlNode* currnode = root->children; currnode; currnode = currnode->next ) {
        if( ( currnode->type == XML_ELEMENT_NODE ) && ( !xmlStrcmp(currnode->name, (const xmlChar*)letterName) ) ) {
            try {
                CUnrecognizedLetter* letter = new CUnrecognizedLetter( currnode );
                letters.PushBack( letter );
            } catch( IError* error ) {
                error->Show();
                error->Delete();
            }
        }
    }
    xmlFree( doc );
}

CSoundParser::~CSoundParser() {
    if( currentWord )
        delete currentWord;
}

IArray<IRecognizedLetter*>* CSoundParser::TranslateNextWord() {
    if( recognizedWord != 0 )
        recognizedWord = 0;
    recognizedWord = new CArray<IRecognizedLetter*>;
    fillCurrentWord( getNextWord() );
    for( int i = currentWord->Size() - 1; i >= 0; i-- )
        translationStep( i );
    recognition_began = false;
    return recognizedWord;
}

void CSoundParser::fillCurrentWord( const wstring& word ) {
    if( currentWord )
        delete currentWord;
    currentWord = new CArray<IUnrecognizedLetter*>;
    for( int i = 0; i < word.length(); i++ ) {
        wstring currWord = L"";
        currWord.push_back( word[i] );
        for( int j = 0; j < letters.Size(); j++ ) {
            IUnrecognizedLetter* currLetter = letters.Item( j );
            if( currLetter->GetLetterValue() == currWord ) {
                currentWord->PushBack( new CUnrecognizedLetter( currLetter ) );
                break;
            }
        }
    }
}

void CSoundParser::translationStep( int pos ) {
    if( saving == SS_Drop )
        currHardnessState = HS_Neutral;
    if( saving == SS_Keep )
        saving = SS_Drop;
    if( saving == SS_Save )
        saving = SS_Keep;
    IUnrecognizedLetter* currLetter = currentWord->Item( pos );
    setPrediction( pos );
    if( currLetter->GetLetterValue() == L"ъ" ) {
        currHardnessState = HS_Hard;
        return;
    }
    if( currLetter->GetLetterValue() == L"ь" ) {
        currHardnessState = HS_Soft;
        saving = SS_Save;
        return;
    }
    currSonorityState = SS_Neutral;
    setStates( pos );
    recognizedWord->PushFront( getSpecialLetter( currentWord->Item( pos ) ) );
    recognition_began = true;
}

void CSoundParser::setStates( int pos ) {
    if( ( pos != currentWord->Size() - 1 ) && recognition_began ) {
        IRecognizedLetter* lastLetter = recognizedWord->Item( 0 );
        if( (lastLetter->GetObjectType() == LOT_Vowel) && isSoftingVowel( currentWord->Item( pos + 1 ) ) ) {
            if( saving == SS_Drop )
                currHardnessState = HS_Soft;
            
        }
        if(lastLetter->GetObjectType() == LOT_Consonant) {
            IConsonantLetter* letter = letter_cast<IConsonantLetter>( lastLetter );
            if( letter->GetDefaultType() == CT_Deaf )
                currSonorityState = SS_Deaf;
            else {
                currSonorityState = SS_Neutral;
                if( checkInstallSonarity( letter ) )
                currSonorityState = SS_Sonor;
            }
        }
    } else {
        if( currentWord->Size() != 1 )
            currSonorityState = SS_Deaf;
        if( saving == SS_Drop )
            currHardnessState = HS_Neutral;
    }
}

void CSoundParser::setPrediction( int pos ) {
    if( ( currentWord->Size() == 1 ) || ( pos == 0 ) ) {
        prediction = P_Empty;
    } else {
        if( currentWord->Item( pos - 1 )->GetLetterType() == LT_Vowel )
            prediction = P_Vowel;
        if( currentWord->Item( pos - 1 )->GetLetterType() == LT_Consonant )
        {
            if( ( currentWord->Item( pos - 1 )->GetLetterValue() == L"ъ" ) ||
               ( currentWord->Item( pos - 1 )->GetLetterValue() == L"ь" ) )
                prediction = P_Special;
            else
                prediction = P_Consonant;
        }
    }
    
}

IRecognizedLetter* CSoundParser::getDefaultConsonantLetter( IUnrecognizedLetter* letter ) {
    CConsonantLetter* newLetter = new CConsonantLetter();
    newLetter->SetLetterValue( letter->GetLetterValue() );
    newLetter->SetDefaultSound( CS_Hard );
    newLetter->SetDefaultType( letter->GetDefaultType() );
    if( !letterCanBeHard( letter ) )
        newLetter->SetDefaultSound( CS_Soft );
    return newLetter;
}

IRecognizedLetter* CSoundParser::getDefaultVowelLetter( IUnrecognizedLetter* letter) {
    const IArray<wstring>* sounds = letter->GetSounds();
    ASSERT( sounds != NULL )
    ASSERT( sounds->Size() != 0 )
    CVowelLetter* newLetter = new CVowelLetter();
    newLetter->SetLetterValue( sounds->Item( 0 ) );
    return newLetter;
}

IRecognizedLetter* CSoundParser::getSpecialConsonantLetter( IUnrecognizedLetter* letter ) {
    CConsonantLetter* newLetter = letter_cast<CConsonantLetter>( getDefaultConsonantLetter( letter ) );
    newLetter->SetLetterValue( L"" );
    if( currSonorityState == SS_Deaf ) {
        if( letterCanBeDeaf( letter ) )
            newLetter->SetDefaultType( CT_Deaf );
        else
            newLetter->SetDefaultType( letter->GetDefaultType() );
    } else if( currSonorityState == SS_Sonor ) {
        if( letterCanBeSonor( letter ) )
            newLetter->SetDefaultType( CT_Sonant );
        else
            newLetter->SetDefaultType( letter->GetDefaultType() );
    }
    
    if( currHardnessState == HS_Hard ) {
        if( letterCanBeHard( letter ) )
            newLetter->SetDefaultSound( CS_Hard );
        else
            newLetter->SetDefaultSound( CS_Soft );
    } else if( currHardnessState == HS_Soft ) {
        if( letterCanBeSoft( letter ) )
            newLetter->SetDefaultSound( CS_Soft );
        else
            newLetter->SetDefaultSound( CS_Hard );
    }
    const IArray<wstring>* sounds = letter->GetSounds();
    ASSERT( sounds != 0 )
    wstring spec_char = L"";
    if( newLetter->GetDefaultType() == CT_Deaf ) {
        spec_char = L"d";
    } else {
        spec_char = L"s";
    }
    wstring value = L"";
    for( int i = 0; i < sounds->Size(); i++ ) {
        wstring sound = sounds->Item( i );
        if( sound.find( spec_char ) != wstring::npos ) {
            value.push_back( sound[1] );
            break;
        }
    }
    newLetter->SetLetterValue( value );
    return newLetter;
}

IRecognizedLetter* CSoundParser::getSpecialVowelLetter( IUnrecognizedLetter* letter ) {
    const IArray<wstring>* sounds = letter->GetSounds();
    CVowelLetter* vowelLetter = letter_cast<CVowelLetter>( getDefaultVowelLetter( letter ) );
    ASSERT( sounds != 0 )
    if( sounds->Size() == 1 )
        return vowelLetter;
    bool allOne = true;
    for( int i = 0; i < sounds->Size(); i++ ) {
        allOne &= sounds->Item( i ).length() == 1;
    }
    if( allOne )
        return vowelLetter;
    if( ( prediction == P_Vowel ) || ( prediction == P_Special ) || ( prediction == P_Empty ) )
        vowelLetter->SetLetterValue( sounds->Item( 1 ) );
    return vowelLetter;
}

IRecognizedLetter* CSoundParser::getSpecialLetter( IUnrecognizedLetter* letter ) {
    if( letter->GetLetterType() == LT_Vowel )
        return getSpecialVowelLetter( letter );
    else
        return getSpecialConsonantLetter( letter );
}

bool CSoundParser::letterCanBeDeaf( IUnrecognizedLetter* letter ) {
    const IArray<wstring>* sounds = letter->GetSounds();
    ASSERT( sounds != 0 )
    for( int i = 0; i < sounds->Size(); i++ ) {
        wstring currWord = sounds->Item( i );
        if( currWord.find( L"d" ) != wstring::npos )
            return true;
    }
    return false;
}

bool CSoundParser::letterCanBeSonor( IUnrecognizedLetter* letter ) {
    const IArray<wstring>* sounds = letter->GetSounds();
    ASSERT( sounds != 0 )
    for( int i = 0; i < sounds->Size(); i++ ) {
        wstring currWord = sounds->Item( i );
        if( currWord.find( L"s" ) != wstring::npos )
            return true;
    }
    return false;
}

bool CSoundParser::letterCanBeHard( IUnrecognizedLetter* letter ) {
    return ( letter->GetLetterValue() != L"й" ) && ( letter->GetLetterValue() != L"ч" ) && ( letter->GetLetterValue() != L"щ" );
}

bool CSoundParser::letterCanBeSoft( IUnrecognizedLetter* letter ) {
    return ( letter->GetLetterValue() != L"ж" ) && ( letter->GetLetterValue() != L"ш" ) && ( letter->GetLetterValue() != L"ц" );
}

bool CSoundParser::isSoftingVowel( IUnrecognizedLetter* letter ) {
    return ( letter->GetLetterValue() == L"е" )
    || ( letter->GetLetterValue() == L"ё" )
    || ( letter->GetLetterValue() == L"и" )
    || ( letter->GetLetterValue() == L"ю" )
    || ( letter->GetLetterValue() == L"я" );
}

bool CSoundParser::checkInstallSonarity( IConsonantLetter* letter ) {
    return ( letter->GetDefaultType() == CT_Sonant ) &&
    ( (letter->GetLetterValue() != L"в" ) &&
     (letter->GetLetterValue() != L"й" ) &&
     (letter->GetLetterValue() != L"л" ) &&
     (letter->GetLetterValue() != L"м" ) &&
     (letter->GetLetterValue() != L"н" ) &&
     (letter->GetLetterValue() != L"р" ) );
}

wstring CSoundParser::getNextWord() {
    wstring ret = file->GetNextWord();
    if( file->EndOfFile() ) {
        return prevWord;
    }
    if( prevWord != L"" ) {
        ret = prevWord + ret;
        prevWord = L"";
        return ret;
    } else {
        if( ret.length() == 1 ) {
            prevWord = ret;
            return getNextWord();
        } else
            return ret;
    }
    return L"";
}