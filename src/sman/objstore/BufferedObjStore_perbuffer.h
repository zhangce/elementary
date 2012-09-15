//
//  BufferedObjStore.h
//  SMan
//
//  Created by Ce Zhang on 9/7/12.
//  Copyright (c) 2012 HazyResearch. All rights reserved.
//

#ifndef SMan_BufferedObjStore_h
#define SMan_BufferedObjStore_h

#include "../common/Common.h"

#include "ObjStore.h"
#include "ObjectStore_MM.h"
#include "ObjectStore_FILE.h"

namespace hazy{
  namespace sman{
  
    template<class VALUE, StorageType STORAGE, BufferType BUFFER = BUFFER_NIL>
    class BufferedObjStore{
    public:
      
      pthread_mutex_t* mutex;
      
      ObjStore<VALUE, STORAGE> os;

      int * buffercounts;
      int bufferSize;   // number of object slots in buffer
      VALUE* buffers;    // buffer -- list of objects
      int64_t* buffer2obj;  // buffer-id -> current object it helds
      std::vector<size_t> obj2buffer;  // object -> current buffer
      std::vector<size_t> pagecounts; 
      std::vector<pthread_mutex_t*> pagelocks;
      
      pthread_mutex_t** mutexes;
      
      std::list<size_t> free_buf_list;

      int nget ;
      int nset ;
      int nload ;
      
      int hitset ;
      int hitget ;
      int hitload ;
      
      int nflush ;
      
      
      BufferedObjStore(int _bufferSize){
      
        nget = 0; nset = 0; nload = 0;
        hitget = 0; hitset = 0; hitload = 0;
        nflush = 0;
        
        std::cout << "INFO: " << "Use BufferedObjStore STORAGE_MM." << std::endl;
        bufferSize = _bufferSize;
        buffers = new VALUE[bufferSize];
        buffer2obj = new int64_t[bufferSize];
        mutexes = new pthread_mutex_t*[bufferSize];
        buffercounts = new int[bufferSize];
        
        for(int i=0;i<bufferSize;i++){
          buffer2obj[i] = -1;
          free_buf_list.push_back(i);
          mutexes[i] = new pthread_mutex_t;
          pthread_mutex_init(mutexes[i], NULL);
        }

        mutex = new pthread_mutex_t;
        pthread_mutex_init(mutex, NULL);
      }
      
      ~BufferedObjStore(){
        
        delete [] buffers;
      }
      
      void flush(size_t buf){
        
        nflush ++;
        os.set(buffer2obj[buf], buffers[buf]);
      }
                  
      /*
      size_t getfree(){
        
        if(free_buf_list.empty() == false){
          size_t front = free_buf_list.front();
          free_buf_list.pop_front();
          return front;
        }

        for(size_t i=0;i<bufferSize;++i){
          size_t page = buffer2obj[i];
          if(page != -1){
            flush(i);
            obj2buffer[buffer2obj[i]] = -1;
            buffer2obj[i] = -1;
            return i;
          }
        }
        assert(false);
        return -1;
      }*/
      
      size_t getfree(){
        
        if(free_buf_list.empty() == false){
          size_t front = free_buf_list.front();
          pthread_mutex_lock(mutexes[front]);          
          free_buf_list.pop_front();
          return front;
        }
        for(size_t i=0;i<bufferSize;++i){
          size_t page = buffer2obj[i];
          if(page != -1){
            int rs = pthread_mutex_trylock(mutexes[i]);
            if(rs != 0){
              continue;
            }
            flush(i);
            obj2buffer[buffer2obj[i]] = -1;
            buffer2obj[i] = -1;
            return i;
          }
        }
        return -1;
      }
      
      StatusType apply(int64_t key, void (*func)(VALUE &)){
        
        size_t buf = obj2buffer.at(key);
        if (buf != -1) {
          pthread_mutex_lock(mutexes[buf]);
          if(key != buffer2obj[buf]){
            pthread_mutex_unlock(mutexes[buf]);
            goto loop3;
          }
          func(buffers[buf]);
          pthread_mutex_unlock(mutexes[buf]);
        }else{
        loop3:
          size_t buf = getfree();
          if(buf != -1){
            os.get(key, buffers[buf]);
            obj2buffer[key] = buf;
            buffer2obj[buf] = key;
            func(buffers[buf]);
            pthread_mutex_unlock(mutexes[buf]);
          }else{
            os.apply(key, func);
          }
        }
        
        return STATUS_SUCCESS;
      }

      
      StatusType get(int64_t key, VALUE & value){
        
        nget ++;

        size_t buf = obj2buffer.at(key);
        if (buf != -1) {
          pthread_mutex_lock(mutexes[buf]);
          //std::cout << "*";
          if(key != buffer2obj[buf]){
            pthread_mutex_unlock(mutexes[buf]);
            goto loop;
          }
          hitget ++;
          value = buffers[buf];
          pthread_mutex_unlock(mutexes[buf]);
        }else{
        loop:
          size_t buf = getfree();
          
          if(buf != -1){
            //std::cout << "-";
            os.get(key, buffers[buf]);
            obj2buffer[key] = buf;
            buffer2obj[buf] = key;
            value = buffers[buf];
            pthread_mutex_unlock(mutexes[buf]);
          }else{
            //std::cout << ":";
            os.get(key, value);
          }
        }
                
        return STATUS_SUCCESS;
      }
      
      StatusType set(int64_t key, const VALUE & value){
        
        nset ++;
        
        size_t buf = obj2buffer.at(key);
        if (buf != -1) {
          pthread_mutex_lock(mutexes[buf]);
          if(key != buffer2obj[buf]){
            pthread_mutex_unlock(mutexes[buf]);
            goto loop2;
          }
          hitset ++;
          buffers[buf] = value;
          pthread_mutex_unlock(mutexes[buf]);
        }else{
        loop2:
          size_t buf = getfree();
          
          if(buf != -1){
            os.get(key, buffers[buf]);
            obj2buffer[key] = buf;
            buffer2obj[buf] = key;
            buffers[buf] = value;
            pthread_mutex_unlock(mutexes[buf]);
          }else{
            os.set(key, value);
          }
        }
        
        return STATUS_SUCCESS;
      }
      
      StatusType load(int64_t key, const VALUE & value){
        
        nload ++;
        
        os.load(key, value);
        if(obj2buffer.size() <= key){
          size_t csize = obj2buffer.size();
          for(; csize<=key; csize++){
            obj2buffer.push_back(-1);
            pagecounts.push_back(0);
            pagelocks.push_back(new pthread_mutex_t);
            pthread_mutex_init(pagelocks[csize], NULL);
          }
        }
        return STATUS_SUCCESS;
      }
            
    };
    
  }
}


#endif
