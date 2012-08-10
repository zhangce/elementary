//
//  UniformPage.h
//  storageman
//
//  Created by Ce Zhang on 7/10/12.
//  Copyright (c) 2012 University of Wisconsin-Madison. All rights reserved.
//

#ifndef storageman_UniformPage_h
#define storageman_UniformPage_h

namespace mia{
    namespace sm{


#include "Common.h"

template<class TYPE>
class UniformPage{
    
public:
    
    int PageNOBJ;
    int cobj;
    
    TYPE content[(PageSIZE-8)/SIZEOF(TYPE)];
    
    bool alignToPageSize[(PageSIZE - ((int)(PageSIZE-8)/SIZEOF(TYPE))*SIZEOF(TYPE) - 8)];
    
    UniformPage(){
        
        PageNOBJ = (PageSIZE - 8)/SIZEOF(TYPE);
        cobj = 0;
        
    }
        
    // return
    //    true: suc. put in
    //    false: failed. current page full.
    // not thread safe
    int push(TYPE obj){

        if(cobj >= PageNOBJ){
            return -1;
        }
        
        //std::cout << cobj << std::endl;
        
        content[cobj] = obj;
 
        cobj ++;
        
        return cobj - 1;
        
    }
    
    int getNSlot(){
        return cobj;
    }
        
    TYPE get(int nslot){
        return content[nslot];
    }
    
    void inplace_update(int nslot, TYPE obj){
        content[nslot] = obj;
    }
    
};

}}

#endif
