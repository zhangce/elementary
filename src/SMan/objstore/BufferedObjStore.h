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

//#include <boost/heap/binomial_heap.hpp>

namespace hazy{
  namespace sman{
    
    class BufferPriority{
      int buffer_id;
      int buffer_priority_of_remove;
    };
    
    template<class VALUE, StorageType STORAGE, PropertyType PROPERTY>
    class BufferedObjStore{
    public:
      
      pthread_mutex_t* mutex;
      
      ObjStore<VALUE, STORAGE, PROPERTY> os;
      
      int * buffercounts;
      int bufferSize;   // number of object slots in buffer
      VALUE* buffers;    // buffer -- list of objects
      int64_t* buffer2obj;  // buffer-id -> current object it helds
      
      int64_t* priority_of_hold;
      bool* buffer_dirties;
      
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
      
      int64_t timestamp;
      
      BufferedObjStore(int _bufferSize){
        
        timestamp = RAND_MAX;
        
        nget = 0; nset = 0; nload = 0;
        hitget = 0; hitset = 0; hitload = 0;
        nflush = 0;
        
        std::cout << "INFO: " << "Use BufferedObjStore.";
        
        if(hazy::sman::BUFFERTYPE == hazy::sman::BUFFER_LRU){
          std::cout << " @ LRU." << std::endl;
        }
        
        if(hazy::sman::BUFFERTYPE == hazy::sman::BUFFER_RANDOM){
          std::cout << " @ RANDOM." << std::endl;
        }
        
        
        bufferSize = _bufferSize;
        buffers = new VALUE[bufferSize];
        buffer2obj = new int64_t[bufferSize];
        mutexes = new pthread_mutex_t*[bufferSize];
        buffercounts = new int[bufferSize];
        priority_of_hold = new int64_t[bufferSize];
        buffer_dirties = new bool[bufferSize];
        
        for(int i=0;i<bufferSize;i++){
          buffer2obj[i] = -1;
          free_buf_list.push_back(i);
          mutexes[i] = new pthread_mutex_t;
          pthread_mutex_init(mutexes[i], NULL);
          priority_of_hold[i] = -1;
          buffer_dirties[i] = false;
        }
        
        mutex = new pthread_mutex_t;
        pthread_mutex_init(mutex, NULL);
      }
      
      ~BufferedObjStore(){
        
        delete [] buffers;
      }
      
      void update_priority_of_hold(size_t buf){
        
        if(hazy::sman::BUFFERTYPE == hazy::sman::BUFFER_LRU){
          priority_of_hold[buf] = (++timestamp);
        }
        
        if(hazy::sman::BUFFERTYPE == hazy::sman::BUFFER_RANDOM){
          timestamp = RAND_MAX;
          priority_of_hold[buf] = rand();
        }
        
      }
      
      void flush(size_t buf){
 
        if(buffer_dirties[buf] == true){
          COMMON_NFLUSH ++;
          nflush ++;
          os.set(buffer2obj[buf], buffers[buf]);
        }
      }
      
      size_t getfree2(){
        
        if(free_buf_list.empty() == false){
          size_t front = free_buf_list.front();
          //pthread_mutex_lock(mutexes[front]);
          free_buf_list.pop_front();
          return front;
        }
        
        int not_hold = -1;
        int64_t min = timestamp + 1;
        //std::cout << min << std::endl;
        for(size_t i=0;i<bufferSize;++i){
          if(priority_of_hold[i] < min){
            min = priority_of_hold[i];
            not_hold = i;
          }
        }
        
        //std::cout << "flush buf " << not_hold << std::endl;
        
        size_t page = buffer2obj[not_hold];
        //if(page != -1){
            
          int rs = pthread_mutex_trylock(pagelocks[page]);
            
          //  if(rs != 0){
          //    continue;
          //  }
            
          //if(page != buffer2obj[not_hold]){
          //  pthread_mutex_unlock(pagelocks[page]);
          //  continue;
          //}
            
          flush(not_hold);
          obj2buffer[page] = -1;
          buffer2obj[not_hold] = -1;
          buffer_dirties[not_hold] = false;
            
          pthread_mutex_unlock(pagelocks[page]);
            
          return not_hold;
        //}
        
        //return -1;
      }
      
      StatusType apply(int64_t key, void (*func)(VALUE &)){
        
        pthread_mutex_lock(pagelocks[key]);
        
        size_t buf = obj2buffer.at(key);
        if (buf != -1) {
          update_priority_of_hold(buf);
          func(buffers[buf]);
        }else{
          size_t buf = getfree2();
          if(buf != -1){
            os.get(key, buffers[buf]);
            obj2buffer[key] = buf;
            buffer2obj[buf] = key;
            func(buffers[buf]);
            update_priority_of_hold(buf);
          }else{
            os.apply(key, func);
          }
        }
        
        pthread_mutex_unlock(pagelocks[key]);
        
        return STATUS_SUCCESS;
      }
      
      template<class CONTENT_TYPE>
      StatusType set_meta(int64_t key, int64_t offset, CONTENT_TYPE const & content){
        nget ++;
        
        pthread_mutex_lock(pagelocks[key]);
        
        size_t buf = obj2buffer.at(key);
        if (buf != -1) {
          
          //std::cout << "~";
          
          hitget ++;
          buffers[buf].contents[offset] = content;
          buffer_dirties[buf] = true;
          
          update_priority_of_hold(buf);
        }else{
          
          //std::cout << "*" << key << std::endl;
          
          size_t buf = getfree2();
          
          //std::cout << "~" << key << std::endl;
          
          if(buf != -1){
            
	    //usleep(3);
            os.get(key, buffers[buf]);
            obj2buffer[key] = buf;
            buffer2obj[buf] = key;
            buffers[buf].contents[offset] = content;
            buffer_dirties[buf] = true;
            
            update_priority_of_hold(buf);
            
          }else{
            //std::cout << ":";
            VALUE value;
            os.get(key, value);
            value.contents[offset] = content;
            os.set(key, value);
          }
        }
        
        pthread_mutex_unlock(pagelocks[key]);
        
        return STATUS_SUCCESS;
        
      }
      
      template<class CONTENT_TYPE>
      StatusType get_meta(int64_t key, int64_t offset, CONTENT_TYPE & content){
        nget ++;
        
        pthread_mutex_lock(pagelocks[key]);
        
        size_t buf = obj2buffer.at(key);
        if (buf != -1) {
          
          //std::cout << "~";
          
          hitget ++;
          content = buffers[buf].contents[offset];
          
          // lru
          update_priority_of_hold(buf);
          
        }else{
          
          //std::cout << "*" << key << std::endl;
          
          size_t buf = getfree2();
          
          //std::cout << "~" << key << std::endl;
          
          if(buf != -1){
            
	    //usleep(3);
            os.get(key, buffers[buf]);
            obj2buffer[key] = buf;
            buffer2obj[buf] = key;
            content = buffers[buf].contents[offset];
            
            update_priority_of_hold(buf);
            
          }else{
            //std::cout << ":";
            VALUE value;
            os.get(key, value);
            content = value.contents[offset];
          }
        }
        
        pthread_mutex_unlock(pagelocks[key]);
        
        return STATUS_SUCCESS;
        
      }
      
      StatusType get(int64_t key, VALUE & value){
        
        //std::cout << key << std::endl;
        
        nget ++;
        
        pthread_mutex_lock(pagelocks[key]);
        
        size_t buf = obj2buffer.at(key);
        if (buf != -1) {
          
          //std::cout << "~";
          
          hitget ++;
          value = buffers[buf];
          update_priority_of_hold(buf);
        }else{
          
          //std::cout << "*" << key << std::endl;
          
          size_t buf = getfree2();
          
          //std::cout << "~" << key << std::endl;
          
          if(buf != -1){
            
            os.get(key, buffers[buf]);
            obj2buffer[key] = buf;
            buffer2obj[buf] = key;
            value = buffers[buf];
            
            update_priority_of_hold(buf);
            
          }else{
            //std::cout << ":";
            os.get(key, value);
          }
        }
        
        pthread_mutex_unlock(pagelocks[key]);
        
        return STATUS_SUCCESS;
      }
      
      StatusType set(int64_t key, const VALUE & value){
        
        nset ++;
        
        pthread_mutex_lock(pagelocks[key]);
        
        size_t buf = obj2buffer.at(key);
        if (buf != -1) {
          hitset ++;
          buffers[buf] = value;
          buffer_dirties[buf] = true;
          update_priority_of_hold(buf);
        }else{
          
          size_t buf = getfree2();
          
          if(buf != -1){
            os.get(key, buffers[buf]);
            obj2buffer[key] = buf;
            buffer2obj[buf] = key;
            buffers[buf] = value;
            buffer_dirties[buf] = true;
            update_priority_of_hold(buf);
          }else{
            os.set(key, value);
          }
        }
        
        pthread_mutex_unlock(pagelocks[key]);
        
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
