//
//  main.cpp
//  TextParser
//
//  Created by Vadim Kirilin on 27.01.15.
//  Copyright (c) 2015 Vadim Kirilin. All rights reserved.
//

#include <iostream>
#include <codecvt>
#include "common.h"
#include "CMDParser.h"
#include "TextFile.h"
#include "Letter.h"
#include "SoundParser.h"
#include "sndfile.h"

int main(int argc, const char * argv[]) {
    setlocale(LC_ALL, "ru_RU");
    std::locale::global(std::locale("ru_RU"));
    Pair<unsigned long, string> a( 1, "f" );
    Pair<unsigned long, string> b( 1, "flag" );
    Pair<unsigned long, string> c( 4, "g" );
    vector<Pair<unsigned long, string>> v;
    v.push_back(a);
    v.push_back(b);
    v.push_back(c);
    CCMDParser parser( argc, argv, v );
    std::wstring_convert<std::codecvt_utf8<wchar_t>,wchar_t> convert;
    
    try {
        CTextFile file( "/Users/vadimkirilin/Documents/Dict.txt" );
        file.Open( "/Users/vadimkirilin/Documents/File.txt" );
        CSoundParser parser(&file, "/Users/vadimkirilin/Documents/LettersState.xml" );
        while( !file.EndOfFile() ) {
            IArray<IRecognizedLetter*>* result = parser.TranslateNextWord();
            for( int i = 0; i < result->Size(); i++ ) {
                IRecognizedLetter* curr = result->Item( i );
                if( curr->GetObjectType() == LOT_Consonant ) {
                    IConsonantLetter* cons = letter_cast<IConsonantLetter>(curr);
                    cout << convert.to_bytes(cons->GetLetterValue());
                    if( cons->GetDefaultSound() == CS_Soft )
                        cout << "'";
                    else
                        cout << "";
                } else {
                    IVowelLetter* vowl = letter_cast<IVowelLetter>(curr);
                    cout << convert.to_bytes( vowl->GetLetterValue() );
                }
            }
            cout << "  ";
            delete result;
        }
    }
    catch( IError* err ) {
        err->Show();
        err->Delete();
    }
    SNDFILE* file;
    SF_INFO info;
    return 0;
}
