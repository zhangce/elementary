//
//  Buffer_mm.h
//  storageman
//
//  Created by Ce Zhang on 7/10/12.
//  Copyright (c) 2012 University of Wisconsin-Madison. All rights reserved.
//

#ifndef storageman_Buffer_mm_h
#define storageman_Buffer_mm_h

#include <vector>
#include <assert.h>
#include "SlottedPage.h"
#include "IntsBlock.h"

namespace mia{
    namespace sm{


template<template<class A> class PAGE, class TYPE>
    // if PAGE=SlottedPage, TYPE must = IntsBlock
class Buffer_mm{
public:
            
    int cpage;
    
    int miss_read;
    int miss_write;
        
    std::vector<PAGE<TYPE> > pages;
        
    Buffer_mm(){
        
        std::cout << "";
        
        cpage = 0;
        
        miss_read = 0;
        miss_write = 0;
        
        PAGE<TYPE> page;
        pages.push_back(page);
    }
        
    //TODO: assume pageid and slotid are good
    TYPE get(int pageid, int slotid){
        return pages[pageid].get(slotid);
    }
        
    //TODO: assume pageid and slotid are good
    //TODO: must be in place update
    void set(int pageid, int slotid, TYPE obj){
        pages[pageid].inplace_update(slotid, obj);
    }
        
    void push(TYPE obj, int & pageid, int & slotid){
        
        int rs = pages[cpage].push(obj);
        
        if(rs == -1){
            cpage ++;
            PAGE<TYPE> page;
            pages.push_back(page);
            rs = pages[cpage].push(obj);
        }
                
        assert(rs != -1);
            
        slotid = rs;
        pageid = cpage;
    }
        
};

    }}
#endif
