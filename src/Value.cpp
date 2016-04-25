//
//  Value.cpp
//  Lisp
//
//  Created by Joachim Wester on 22/04/16.
//  Copyright © 2016 Starcounter AB.
//

#include "Value.hpp"
#include "Cons.hpp"
#include "Isolate.hpp"


std::string STRING::ToString() {
    if (Pointer == 0) {
        return "\"\"";
    }
    return (char*) Pointer;
}



void STRING::Print() {
    std::cout << "\"";
    std::cout << ToString();
    std::cout << "\"";
}

void STRING::AllocateString( const char* str, size_t size ) {
    char* obj = (char*)CurrentIsolate->Heap.SafeMalloc(size+1); // We add a zero termination so we have a
    // cheap conversion
    // to zero terminated strings if needed. TODO! GC
    memcpy(obj, str, size);
    obj[size] = 0;
    this->Pointer = (uint64_t)obj;
}


SYMBOL::SYMBOL( const char* str, size_t len ) {
    IsHeapObject = false;
    PType = PSymbol;
    Integer = CurrentIsolate->RegisterSymbol( str, len );
}

std::string SYMBOL::ToString() {
    return CurrentIsolate->GetStringFromSymbolId(Integer-1); // vectors are zero based. symbols start with 1
}

void SYMBOL::Print() {
    std::cout << ToString();
    // std::cout << "#";       // Uncomment these to see symbol integer
    // std::cout << Integer;   // Uncomment these to see symbol integer
}


void NIL::Print() {
    std::cout << "nil";
}



void VALUE::Print() {
    if (IsHeapObject == true) {
        switch (HType) {
            case (TString) :
                ((STRING*)this)->Print();
                return;
            case (TCons) :
                ((CONS*)this)->Print();
                return;
            default:
                break;
        }
    }
    else {
        switch (PType) {
            case (PSymbol) :
                ((SYMBOL*)this)->Print();
                return;
            case (PNil) :
                ((NIL*)this)->Print();
                return;
            case (PInteger) :
                ((INTEGER*)this)->Print();
                return;
            default:
                break;
        }

    }
    std::cout << "I dont know how to print a heap=";
    std::cout << IsHeapObject;
    std::cout << " htype=";
    std::cout << HType;
    std::cout << " ptype=";
    std::cout << PType;
}

void INTEGER::Print() {
    std::cout << Integer;
}


