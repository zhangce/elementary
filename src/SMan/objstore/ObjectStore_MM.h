//
//  ObjectStore_MM.h
//  SMan
//
//  Created by Ce Zhang on 9/7/12.
//  Copyright (c) 2012 HazyResearch. All rights reserved.
//

#ifndef SMan_ObjectStore_MM_h
#define SMan_ObjectStore_MM_h

namespace hazy{
  namespace sman{
    
    template<class VALUE, PropertyType PROPERTY>
    class ObjStore<VALUE, STORAGE_MM, PROPERTY>{
      
    public:
      
      VALUE * mm;
      size_t size;
      
      ~ObjStore(){
        delete [] mm;
      }
      
      ObjStore(){
        
        std::cout << "INFO: " << "Use specification STORAGE_MM." << std::endl;
        
        mm = new VALUE[OBJECTSTORE_INIT_CONTAINER_SIZE];
        size = OBJECTSTORE_INIT_CONTAINER_SIZE;
      }
      
      StatusType get(int64_t key, VALUE & value){
      
        value = mm[key];
        return STATUS_SUCCESS;
      }
      
      StatusType set(int64_t key, const VALUE & value){
        
        
        mm[key] = value;
        return STATUS_SUCCESS;
      }
      
      StatusType load(int64_t key, const VALUE & value){
        
        if (key < 0) {
          return STATUS_SET_INVALIDKEY;
        }
        
        if (key >= size) {
          size_t oldsize = size;
          while (key >= size) {
            size *= 2; 
          }
          std::cout << "expanding from " << oldsize << " to " << size << std::endl;
          VALUE * newmm = new VALUE[size];
          memcpy(newmm, mm, oldsize*sizeof(VALUE));
          delete [] mm;
          mm = newmm;
        }
        
        mm[key] = value;
        return STATUS_SUCCESS;
      }

      
      StatusType apply(int64_t key, void (*func)(VALUE &)){
       
        func(mm[key]);
        return STATUS_SUCCESS;
      }
      
    };
    
    
  }
}


#endif
