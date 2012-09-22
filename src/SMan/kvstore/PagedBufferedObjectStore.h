//
//  PagedBufferedObjectStore.h
//  SMan
//
//  Created by Ce Zhang on 9/14/12.
//  Copyright (c) 2012 HazyResearch. All rights reserved.
//

#ifndef SMan_PagedBufferedObjectStore_h
#define SMan_PagedBufferedObjectStore_h


#include "../common/Common.h"

#include "../objstore/BufferedObjStore.h"

namespace hazy{
  namespace sman{
    
    template<class VALUE, int ncontent, int ndummy>
    class page{
    public:
      VALUE contents[ncontent];
      char dummy[ndummy];
    };
    
    template<class VALUE, StorageType STORAGE, PropertyType PROPERTY>
    class PagedBufferedObjStore{
    public:
      
      const int NVALUE_PER_PAGE;
      int cpid;
      
      BufferedObjStore< page<VALUE, COMMON_PAGESIZE/sizeof(VALUE), COMMON_PAGESIZE-(int)(COMMON_PAGESIZE/sizeof(VALUE))*sizeof(VALUE)>, STORAGE, PROPERTY > pagestore;
      
      PagedBufferedObjStore(int nbuffer) : NVALUE_PER_PAGE(COMMON_PAGESIZE/sizeof(VALUE)),
	pagestore(BufferedObjStore< page<VALUE, COMMON_PAGESIZE/sizeof(VALUE), COMMON_PAGESIZE-(int)(COMMON_PAGESIZE/sizeof(VALUE))*sizeof(VALUE)>, STORAGE, PROPERTY >(nbuffer))
      {
        cpid = -1;
        std::cout << "INFO: Use PagedBufferedObjStore..." << std::endl;
      }
      
      ~PagedBufferedObjStore(){
        
      }
      
      StatusType apply(int64_t key, void (*func)(VALUE &)){
        
        VALUE v;
        
        this->get(key, v);
        
        func(v);
        
        this->set(key, v);
        
        return STATUS_SUCCESS;
      }
      
      StatusType get(int64_t key, VALUE & value){
        
        int64_t pid = key / NVALUE_PER_PAGE;
        int64_t poffset = key % NVALUE_PER_PAGE;
        
        pagestore.template get_meta<VALUE>(pid, poffset, value);
        
        return STATUS_SUCCESS;
      }
      
      StatusType set(int64_t key, const VALUE & value){
        
        int64_t pid = key / NVALUE_PER_PAGE;
        int64_t poffset = key % NVALUE_PER_PAGE;
        
        pagestore.template set_meta<VALUE>(pid, poffset, value);
        
        return STATUS_SUCCESS;
      }
      
      StatusType load(int64_t key, const VALUE & value){
        
        int64_t pid = key / NVALUE_PER_PAGE;
        int64_t poffset = key % NVALUE_PER_PAGE;
        
        if(cpid < pid){
          pagestore.load(pid, page<VALUE, COMMON_PAGESIZE/sizeof(VALUE), COMMON_PAGESIZE-(int)(COMMON_PAGESIZE/sizeof(VALUE))*sizeof(VALUE)>());
          cpid = pid;
        }
        this->set(key, value);
        
        return STATUS_SUCCESS;
      }
      
    };
    
  }
}



#endif
