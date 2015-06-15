//
//  common.h
//  TextParser
//
//  Created by Vadim Kirilin on 27.01.15.
//  Copyright (c) 2015 Vadim Kirilin. All rights reserved.
//

#pragma once

#define interface struct

#include "Error.h"

#include <string>
using std::wstring;
using std::string;

#include <iostream>
using std::wcout;
using std::wcerr;
using std::endl;
using std::cout;
using std::cerr;

#include <vector>
using std::vector;

template<typename A, typename B>
struct Pair {
    Pair( A _first, B _second ) :
    first(_first),
    second(_second)
    {}
    const A a() const { return first; }
    const B b() const { return second; }
private:
    A first;
    B second;
};

string convert( const char c );
string convert( const int i );
wstring wconvert( const wchar_t c );

void assert( bool expr, const string& msg );

#define ASSERT( expr) assert( expr, convert(__LINE__) + " " + __FILE__ );

template<typename Type>
interface IArray {
    virtual ~IArray() {}
    virtual int Size() const = 0;
    virtual const Type Item( int i ) const {
        return const_cast<IArray<Type>*>(this)->Item( i );
    }
    virtual Type Item( int i ) = 0;
    virtual void PushBack( const Type& item ) = 0;
};

template<typename Type>
interface IArray<Type*> {
    virtual ~IArray() {}
    virtual int Size() const = 0;
    virtual const Type* Item( int i ) const {
        return const_cast<IArray<Type*>*>(this)->Item( i );
    }
    virtual Type* Item( int i ) = 0;
    virtual void PushBack( Type* item ) = 0;
    virtual void Clear() = 0;
};

void testPrint( const wstring& str );