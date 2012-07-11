//
//  IntsBlock.h
//  storageman
//
//  Created by Ce Zhang on 7/10/12.
//  Copyright (c) 2012 University of Wisconsin-Madison. All rights reserved.
//

#ifndef storageman_IntsBlock_h
#define storageman_IntsBlock_h

#include "Common.h"
#include <assert.h>

namespace mia{
    namespace sm{


class IntPair{
public:
    int first;
    int second;
};

class IntsBlock{
    
public:
    int size;  
    int content[MemBlockNINT];
    
    IntsBlock() : size(0){}
    
    template<class TYPE>
    void append(TYPE obj){
        
        int nint = sizeof(TYPE)/SIZEOFINT;
        assert(size + nint < MemBlockNINT);
        *((TYPE*) &content[size]) = obj;
        size += nint;
        
    }
    
    template<class TYPE>
    TYPE get(int nint){
        return *((TYPE*) &content[nint]);
    }
    
    
};


    }}


#endif
