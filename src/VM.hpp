//
//  Continuation.hpp
//  Addie
//
//  Created by Joachim Wester on 28/04/16.
//
//

#ifndef VM_hpp
#define VM_hpp

#include "Isolate.hpp"
#include "Vector.hpp"
#include "Value.hpp"


// Contains fixed registers (used for variables instead of stack)
// Registers are preloaded with constants
//class Frame : Object {
//public:
//    VALUE Variants[10]; // Variant variables/registers
//    VALUE Invariants[10]; // Invariant variables/registers and constants
//};

typedef uint8_t Op;


class Instruction {
public:
    byte A;
    byte B;
    byte C;
    Op OP;
    
    Instruction( Op op ) {
        OP = op;
    }
    
    Instruction( Op op, uint16_t a2, byte c  ) {
        OP = op;
        A = a2 & 0x00FF;
        B = a2 & 0xFF00;
        C = c;
    }
    
    Instruction( Op op, uint32_t a3  ) {
        OP = op;
        A = a3 & 0x0000FF;
        B = a3 & 0x00FF00;
        C = a3 & 0xFF0000;
    }
    
    Instruction( Op op,byte a, byte b, byte c ) {
        OP = op;
        A = a;
        B = b;
        C = c;
    }
    Instruction( Op op, byte a, byte b ) {
        OP = op;
        A = a;
        B = b;
    }
    Instruction( Op op, byte a ) {
        OP = op;
        A = a;
    }
    
    uint32_t A3() {
        return A + (B<<8) + (C<<16);
    }
    
    uint32_t A2() {
        return A + (B<<8);
    }

};

class OpLoadConst : public Instruction {
public:
    OpLoadConst( byte regno, uint16_t value ) : Instruction( LOAD_CONST,value,regno) {
    }
};


class OpCall : public Instruction {
public:
    OpCall( byte symreg, byte p1reg, byte p2reg ) : Instruction( CALL_2, symreg, p1reg, p2reg ) {     }
    OpCall( byte symreg, byte p1reg ) : Instruction( CALL_1, symreg, p1reg ) {
    }
    OpCall( byte symreg ) : Instruction( CALL_0, symreg )   { }
};
class OpMove : public Instruction {
public:
    OpMove( byte reg1, byte reg2 ) : Instruction( MOVE, reg1, reg2 ) {
    }
};





//class Compilation {
//public:
//    Instruction* Code;
//    VALUE* Registers;
//};


struct Compilation {
    u32 SizeOfRegisters;
    u32 SizeOfInitializedRegisters;
    Instruction* Code;
};

// Frames contain registers. They can be garbage collected and they can be referenced
// by continuations.
class Frame : Object {
    
};


class Continuation {
public:
    Instruction* PC;                    // Program Counter (aka Instruction Pointer).
    Frame* frame;
    Compilation* Comp;
    
    void AllocateFrame( Compilation* code) {
        // The compiled code contains the size of the register machine needed for the
        // code. It also contains the initial values for the registers that are either
        // invariant or that have a initial value.
        Comp = code;
        frame = (Frame*)CurrentIsolate->MallocHeap(sizeof(Frame) + code->SizeOfRegisters);
        new (frame) Frame();
        //Registers = (VALUE*)(((byte*)frame) + sizeof(Frame));
        memcpy( ((byte*)frame) + sizeof(Frame), ((byte*)code) + sizeof(Compilation), code->SizeOfInitializedRegisters );
    }
};


#endif /* VM_hpp */