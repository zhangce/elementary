//
//  KeyValue_fl.h
//  storageman
//
//  Created by Ce Zhang on 7/10/12.
//  Copyright (c) 2012 University of Wisconsin-Madison. All rights reserved.
//

#ifndef storageman_KeyValue_fl_h
#define storageman_KeyValue_fl_h

#include <assert.h>
#include "UniformPage.h"

namespace mia{
    namespace sm{


template<template<template<class C> class A, class B> class BUFFER, class TYPE>
class KeyValue_fl{
public:
    
    const int PageNVALUE;
    
    int ckey;
    BUFFER<UniformPage, TYPE> buf_values;
    
    KeyValue_fl() : ckey(-1), PageNVALUE((int)(PageSIZE/sizeof(TYPE)))
    {}
    
    TYPE get(int key){
        
        int c_pageid = key/PageNVALUE;
        int c_slotid = key%PageNVALUE;
        
        return buf_values.get(c_pageid, c_slotid);
    }
    
    void set(int key, TYPE block){
        
        assert(key <= ckey + 1 && key >= 0);    // only inc. set or inplace set
        
        int c_pageid = key/PageNVALUE;
        int c_slotid = key%PageNVALUE;
        
        int v_pageid, v_slotid;
        
        buf_values.push(block, v_pageid, v_slotid);
        
        //std::cout << "calc'ed: " << c_pageid << "," << c_slotid << ";"
        //<< "actual: " << v_pageid << "," << v_slotid << std::endl;
        
        assert(c_pageid == v_pageid);
        assert(c_slotid == v_slotid);
        
        if(ckey < key){
            ckey = key;
        }
        
        
    }
    
    
};
        
        
    }}


#endif
