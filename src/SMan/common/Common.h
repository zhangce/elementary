//
//  Common.h
//  SMan
//
//  Created by Ce Zhang on 9/7/12.
//  Copyright (c) 2012 HazyResearch. All rights reserved.
//

#ifndef SMan_Common_h
#define SMan_Common_h

//#define COMMON_PAGESIZE 409600

int COMMON_NFLUSH = 0;

std::string rt_libpath;
std::string rt_accumulo_instance;

#define OBJECTSTORE_INIT_CONTAINER_SIZE 100

#include <iostream>
#include <inttypes.h>
#include <assert.h>
#include <string>

#include <fcntl.h>
#include <vector>
#include <list>

#include <semaphore.h>

#include <string.h>
#include <stdio.h>

#include "Timer.h"

#include <jni.h>

namespace hazy {
  namespace sman{
    
    
    JavaVM *jvm = NULL;

    jclass cls;
    
    jmethodID id_get;
    jmethodID id_set;
    jmethodID id_load;
    jmethodID id_init;
    jmethodID id_reg;
      
    
    enum BufferType {
      BUFFER_NIL   = 0x00000001,
      BUFFER_LRU   = 0x00000010,
      BUFFER_RANDOM= 0x00000100
    };
    
    BufferType BUFFERTYPE;
    
    enum PropertyType {
      PROPERTY_NIL = 0x00000000,
      JAVAHASH_MM  = 0x00000001,
      JAVAHASH_ACCU= 0x00000010
    };
    
    enum StorageType {
      STORAGE_MM   = 0x00000001,
      STORAGE_MMAP = 0x00000010,
      STORAGE_FILE = 0x00000100,
      STORAGE_JHASH= 0x00010000,
      STORAGE_HBASE= 0x00100000
    };

    int hbaseid = 0;

    enum StatusType {
      STATUS_SUCCESS         = 0x00000001,
      STATUS_GET_KEYNOTFOUND = 0x00000010,
      STATUS_SET_INVALIDKEY  = 0x00000100
    };
    

    int ntmp = 0;
    std::string config_tmp_folder = "/tmp/";
    std::string getNextTmpFileName(){
      char tmp[1000];
      sprintf(tmp, "%s/%d", config_tmp_folder.c_str(), (++ntmp));
      return std::string(tmp);
    }
    
    
  }
}

#endif
