//
//  KeyValue.h
//  storageman
//
//  Created by Ce Zhang on 7/10/12.
//  Copyright (c) 2012 University of Wisconsin-Madison. All rights reserved.
//

#ifndef storageman_KeyValue_h
#define storageman_KeyValue_h


#include <assert.h>
#include "IntsBlock.h"
#include "SlottedPage.h"
#include "UniformPage.h"

namespace mia{
    namespace sm{


template<template<template<class C> class A, class B> class BUFFER, class TYPE = IntsBlock>
class KeyValue_vl{
public:
    
    int ckey;
    int PageNKEY;
    
    BUFFER<UniformPage, IntPair> buf_keyoffs;
    BUFFER<SlottedPage, IntsBlock> buf_values;
    
    KeyValue_vl() : ckey(-1), PageNKEY(
        (int)((PageSIZE - 8)/sizeof(IntPair))
    ) {}
    
    IntsBlock get(int key){
        
        int c_pageid = key / PageNKEY;
        int c_slotid = key % PageNKEY;
        
        IntPair k_block = buf_keyoffs.get(c_pageid, c_slotid);
        IntsBlock v_block = buf_values.get(k_block.first, k_block.second);
        
        return v_block;
    }
    
    void set(int key, IntsBlock block){
        
        assert(key <= ckey + 1 && key >= 0);    // only inc. set or inplace set
        
        //std::cout << PageNKEY << std::endl;
        
        int c_pageid = key / PageNKEY;
        int c_slotid = key % PageNKEY;
        
        //std::cout << c_pageid << " " << c_slotid << std::endl;
        
        if(key == ckey + 1){    // push
            int v_pageid, v_slotid;
        
            buf_values.push(block, v_pageid, v_slotid);
                
            IntPair k_block;
            k_block.first = v_pageid;
            k_block.second = v_slotid;
        
            int k_pageid, k_slotid;
        
            buf_keyoffs.push(k_block, k_pageid, k_slotid);
        
            //std::cout << key << std::endl;
            //std::cout << "calc'ed: " << c_pageid << "," << c_slotid << ";"
            //<< "actual: " << k_pageid << "," << k_slotid << std::endl;
        
            assert(c_pageid == k_pageid);    //calcualted is same as actual.
            assert(c_slotid == k_slotid);    //calcualted is same as actual.
        }else{  // update
            
            IntPair k_block = buf_keyoffs.get(c_pageid, c_slotid);
            buf_values.set(k_block.first, k_block.second, block);
            
        }
        
        if(ckey < key){
            ckey = key;
        }
        
    }
    
    
};


    }}
        
#endif







