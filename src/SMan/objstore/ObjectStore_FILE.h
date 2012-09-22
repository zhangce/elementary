//
//  ObjectStore_FILE.h
//  SMan
//
//  Created by Ce Zhang on 9/21/12.
//  Copyright (c) 2012 HazyResearch. All rights reserved.
//

#ifndef SMan_ObjectStore_FILE_h
#define SMan_ObjectStore_FILE_h

//#ifndef __MACH__
//#define __MACH__ 1
//#endif

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
	//fd = open64(filename.c_str(), O_RDWR | O_CREAT | O_DIRECT ,
	fd = open64(filename.c_str(), O_RDWR | O_CREAT | O_DIRECT,
                    (mode_t) 0600);
#endif
        
        assert(fd != -1);
        
        
	//std::cout << "***" << sizeof(VALUE) << std::endl;
        
	objsize = sizeof(VALUE);
        
        size = OBJECTSTORE_INIT_CONTAINER_SIZE;
        
        for(int i=0;i<OBJECTSTORE_INIT_CONTAINER_SIZE;i++){
          
          //pwrite64(fd, &init, objsize, objsize*i);
#ifdef __MACH__
	  VALUE init;
          pwrite(fd, &init, objsize, objsize*i);
#else
	  VALUE * init;
	  std::cout << "~~~" << getpagesize() << std::endl;
	  //std::cout << "###" << 
	  int rr = posix_memalign(&init, getpagesize(), sizeof(VALUE)) ;//<< std::endl; 
	  std::cout << "###" << rr << std::endl;
	  assert(rr == 0);
	  //std::cout << 
	  rr = pwrite64(fd, init, objsize, objsize*i) ;//<< std::endl;
	  std::cout << "~~~" << rr << std::endl;
	  assert(rr > 0);
	  free(init);
#endif
          
        }
        
        
      }
      
      StatusType get(int64_t key, VALUE & value){
        
	//std::cout << "~~~~~~~~2" << std::endl;

#ifdef __MACH__
        pread(fd, &value, objsize, objsize*key);
#else
	VALUE * init;
	//std::cout << "###" << 
	posix_memalign(&init, getpagesize(), sizeof(VALUE)) ;// << std::endl; 
        pread64(fd, init, objsize, objsize*key);
	value = *init;
	free(init);
#endif
	
        //std::cout << "~~~~~~~~3" << std::endl;

        return STATUS_SUCCESS;
      }
      
      StatusType set(int64_t key, const VALUE & value){
        
        //std::cout << "~~~~~~~~~~1" << std::endl;

#ifdef __MACH__
        pwrite(fd, &value, objsize, objsize*key);
#else
        VALUE * init;
        //std::cout << "###" << 
        posix_memalign(&init, getpagesize(), sizeof(VALUE)) ;//;<< std::endl;
        *init = value;
        pwrite64(fd, init, objsize, objsize*key);
        free(init);
#endif
	

	//std::cout << "~~~~~~~~~~1.5" << std::endl;
        
        return STATUS_SUCCESS;
      }
      
      StatusType load(int64_t key, const VALUE & value){
	//std::cout << "~~~~~~~~~~5" << std::endl;
        if (key < 0) {
          return STATUS_SET_INVALIDKEY;
        }
        
        if (key >= size) {
          size_t newsize = size;
          
          while (key >= newsize) {
            newsize *= 2;
          }
          
          
          for(size_t i=size; i<newsize; i++){
#ifdef __MACH__
	    VALUE init;
            pwrite(fd, &init, objsize, objsize*i);
#else
	    VALUE * init;
	    //std::cout << "###" << 
	    posix_memalign(&init, getpagesize(), sizeof(VALUE)) ;//<< std::endl; 
	    free(init);
#endif
          }
          
          size = newsize;
          
        }
#ifdef __MACH__
        pwrite(fd, &value, objsize, objsize*key);
#else
	
	VALUE * init;
	//std::cout << "###" << 
	posix_memalign(&init, getpagesize(), sizeof(VALUE)) ;//<< std::endl; 
	*init = value;
        pwrite64(fd, init, objsize, objsize*key);
	free(init);
#endif

	//std::cout << "~~~~~~~~~~5.1" << std::endl;
        return STATUS_SUCCESS;
      }
      
      StatusType apply(int64_t key, void (*func)(VALUE &)){
        

	//std::cout << "~~~~~~~~~~6" << std::endl;
        VALUE v;
        
        this->get(key, v);
        
        func(v);
        
        this->set(key, v);

	//std::cout << "~~~~~~~~~~6.1" << std::endl;
        
        return STATUS_SUCCESS;
      }
      
    };
    
    
  }
}



#endif
