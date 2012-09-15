//
//  ObjectStore_FILE_NIL.h
//  SMan
//
//  Created by Ce Zhang on 9/7/12.
//  Copyright (c) 2012 HazyResearch. All rights reserved.
//

#ifndef SMan_ObjectStore_FILE_NIL_h
#define SMan_ObjectStore_FILE_NIL_h

namespace hazy{
  namespace sman{
    
    template<class VALUE, PropertyType PROPERTY>
    class ObjStore<VALUE, STORAGE_FILE, PROPERTY>{
      
    public:
      
      int fd;
      int rs_status;
      std::string filename;
      
      ~ObjStore(){
        
      }
      
      ObjStore(){
        
        filename = getNextTmpFileName();
        std::cout << "INFO: " << "Use specification STORAGE_FILE_NIL @ "
                  << filename << "." << std::endl;
        
#ifdef __MACH__
        fd = open(filename.c_str(), O_RDWR | O_CREAT, (mode_t) 0600);
        assert(fd != -1);
        
        rs_status = fcntl(fd, F_NOCACHE);
        assert(rs_status != -1);
#else
        std::cout << "ERROR: " << "Don't implement O_DIRECT on Linux!" << std::endl;
        exit(1);
#endif
        
        
      }
      
      StatusType get(int64_t key, VALUE & value){
        
        
        return STATUS_SUCCESS;
      }
      
      StatusType set(int64_t key, const VALUE & value){
        

        return STATUS_SUCCESS;
      }
      
      StatusType load(int64_t key, const VALUE & value){
        
        
        return STATUS_SUCCESS;
      }
      
      
      StatusType apply(int64_t key, void (*func)(VALUE &)){
        
        
        return STATUS_SUCCESS;
      }
      
    };
    
    
  }
}


#endif
