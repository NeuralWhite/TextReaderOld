//
//  Array.h
//  TextParser
//
//  Created by Vadim Kirilin on 30.01.15.
//  Copyright (c) 2015 Vadim Kirilin. All rights reserved.
//

#pragma once

#include "common.h"
#include <vector>
using std::vector;
template<typename Type>
class CArray : public IArray<Type> {
public:
    CArray() {}
    virtual int Size() const { return int(array.size()); }
    virtual Type Item( int i ) {
        assert( (i >= 0) && (i < array.size()), "Array index out of subscription" );
        return array[i];
    }
    virtual void PushBack( const Type& item ) {
        array.push_back( item );
    }
    virtual void PushFront( const Type& item ) {
        array.insert( array.begin(), item );
    }
    virtual void Clear() {
        array.clear();
    }
private:
    vector<Type> array;
};

template<typename Type>
class CArray<Type*> : public IArray<Type*> {
public:
    CArray() {}
    CArray( const CArray<Type*>& other ) {
        for( int i = 0; i < other.array.size(); i++ ) {
            this->PushBack(other.array[i]);
        }
    }
    ~CArray() {
        for( int i = 0; i < array.size(); i++ )
            delete array[i];
    }
    virtual int Size() const { return int(array.size()); }
    virtual Type* Item( int i ) {
        assert( (i >= 0) && (i < array.size()), "Array index out of subscription" );
        return (array[i]);
    }
    virtual void PushBack( Type* item ) {
        ASSERT( item != 0 )
        Type* nitem = item->Copy();
        array.push_back( nitem );
    }
    virtual void PushFront( Type* item ) {
        ASSERT( item != 0 )
        Type* nitem = item->Copy();
        array.insert( array.begin(), nitem );
    }
    virtual void Clear() {
        for( int i = 0; i < array.size(); i++ )
            delete array[i];
        array.clear();
    }
private:
    vector<Type*> array;
};