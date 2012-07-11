//
//  Buffer_mmap.h
//  storageman
//
//  Created by Ce Zhang on 7/10/12.
//  Copyright (c) 2012 University of Wisconsin-Madison. All rights reserved.
//

#ifndef storageman_Buffer_mmap_h
#define storageman_Buffer_mmap_h


#include <vector>
#include <assert.h>
#include "SlottedPage.h"
#include "IntsBlock.h"

#include <sys/ioctl.h> 
#include <sys/fcntl.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "Common.h"

namespace mia{
    namespace sm{


template<template<class A> class PAGE, class TYPE>
// if PAGE=SlottedPage, TYPE must = IntsBlock
class Buffer_mmap{
public:
    
    int cpage;
    
    int miss_read;
    int miss_write;
    
    int fd;
    
    PAGE<TYPE> * pages;
    int cnpages;
    
    Buffer_mmap(){
        
        cpage = 0;
        
        cnpages = MMAP_INIT_NPAGE;
        
        miss_read = 0;
        miss_write = 0;
        
        fd = open(getNextTmpFileName().c_str(), O_RDWR | O_CREAT, (mode_t) 0600);
        assert(fd != -1);
        
        int result_state = lseek(fd, (MMAP_INIT_NPAGE)* sizeof (PAGE<TYPE>) - 1, SEEK_SET);
        assert(result_state != -1);
        
        result_state = write(fd, "", 1);
        assert(result_state == 1);
        
        pages = new(mmap(0, MMAP_INIT_NPAGE * sizeof (PAGE<TYPE>), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)) PAGE<TYPE>[MMAP_INIT_NPAGE];
        
        //pages = new PAGE<TYPE>[MMAP_INIT_NPAGE];
        
        //std::cout << "2" << std::endl;
        
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
            
            if(cpage >= cnpages){
                
                
                int result_state = lseek(fd, (cnpages*2)* sizeof (PAGE<TYPE>) - 1, SEEK_SET);
                assert(result_state != -1);
                
                result_state = write(fd, "", 1);
                assert(result_state == 1);
                
                pages = new (mmap(0, cnpages*2 * sizeof (PAGE<TYPE>), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0))  PAGE<TYPE>[cnpages*2];
                
                //std::cout << "~~~" << std::endl;
                
                cnpages = cnpages * 2;
            }
            
            rs = pages[cpage].push(obj);
        }
        
        assert(rs != -1);
        
        slotid = rs;
        pageid = cpage;
        
    }
    
};

        
    }}


#endif
