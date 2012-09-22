//
//  ObjectStore_FILE.h
//  SMan
//
//  Created by Ce Zhang on 9/21/12.
//  Copyright (c) 2012 HazyResearch. All rights reserved.
//

#ifndef SMan_ObjectStore_FILE_h
#define SMan_ObjectStore_FILE_h


#include "../common/Common.h"

namespace hazy{
  namespace sman{
    
    template<class VALUE, PropertyType PROPERTY>
    class ObjStore<VALUE, STORAGE_FILE, PROPERTY>{
      
    public:
      
      std::string filename;
      int fd;
      int objsize;
      size_t size;
      
      ~ObjStore(){
        
      }
      
      ObjStore() {
        
        std::cout << "INFO: " << "Use specification STORAGE_FILE." << std::endl;
        
        filename = getNextTmpFileName();
        
        remove(filename.c_str());
        
#ifdef __MACH__
        fd = open(filename.c_str(), O_RDWR | O_CREAT ,
                  (mode_t) 0600);
#else
        fd = open64(filename.c_str(), O_RDWR | O_CREAT | O_DIRECT | O_LARGEFILE ,
                    (mode_t) 0600);
#endif
        
        assert(fd != -1);
        
        VALUE init;
        objsize = sizeof(VALUE);
        
        size = OBJECTSTORE_INIT_CONTAINER_SIZE;
        
        for(int i=0;i<OBJECTSTORE_INIT_CONTAINER_SIZE;i++){
          
          //pwrite64(fd, &init, objsize, objsize*i);
#ifdef __MACH__
          pwrite(fd, &init, objsize, objsize*i);
#else
          pwrite64(fd, &init, objsize, objsize*i);
#endif
          
        }
        
        
      }
      
      StatusType get(int64_t key, VALUE & value){
        
#ifdef __MACH__
        pread(fd, &value, objsize, objsize*key);
#else
        pread64(fd, &value, objsize, objsize*key);
#endif
        
        return STATUS_SUCCESS;
      }
      
      StatusType set(int64_t key, const VALUE & value){
        
#ifdef __MACH__
        pwrite(fd, &value, objsize, objsize*key);
#else
        pwrite64(fd, &value, objsize, objsize*key);
#endif
        
        return STATUS_SUCCESS;
      }
      
      StatusType load(int64_t key, const VALUE & value){
        
        if (key < 0) {
          return STATUS_SET_INVALIDKEY;
        }
        
        if (key >= size) {
          size_t newsize = size;
          
          while (key >= newsize) {
            newsize *= 2;
          }
          
          VALUE init;
          for(size_t i=size; i<newsize; i++){
#ifdef __MACH__
            pwrite(fd, &init, objsize, objsize*i);
#else
            pwrite64(fd, &init, objsize, objsize*i);
#endif
          }
          
          size = newsize;
          
        }
#ifdef __MACH__
        pwrite(fd, &value, objsize, objsize*key);
#else
        pwrite64(fd, &value, objsize, objsize*key);
#endif
        return STATUS_SUCCESS;
      }
      
      StatusType apply(int64_t key, void (*func)(VALUE &)){
        
        VALUE v;
        
        this->get(key, v);
        
        func(v);
        
        this->set(key, v);
        
        return STATUS_SUCCESS;
      }
      
    };
    
    
  }
}



#endif
