//
//  KV_int_-_MMAP_DIRECT_-_DENSEKEY.h
//  sman
//
//  Created by Ce Zhang on 8/19/12.
//  Copyright (c) 2012 HazyResearch. All rights reserved.
//

#ifndef sman_KV_int___MMAP_DIRECT___DENSEKEY_h
#define sman_KV_int___MMAP_DIRECT___DENSEKEY_h

#include "../KV.h"
#include "../common/Common.h"

#include <assert.h>

#include <sys/ioctl.h>
#include <sys/fcntl.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <vector>

namespace mia{
    namespace sm{
        
        template<class VALUE, KV_Replacement REPLACEMENT>
        class KV<int, VALUE, MMAP, DIRECT, REPLACEMENT, DENSE_KEY>{
            
        private:
            
            int size;
            int actual_size;
            
            int fd;
            
            VALUE * mmap_items;
            
            friend class Iterator;
            
            void resize(int newSize){
                
                size = newSize;
                
                int result_state = lseek(fd, size * sizeof (VALUE) - 1, SEEK_SET);
                assert(result_state != -1);
                
                result_state = write(fd, "", 1);
                assert(result_state == 1);
                
                mmap_items = new(mmap(0, size * sizeof (VALUE), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)) VALUE[size];
                
            }
            
            friend class Iterator;
            
        public:
            
            class Iterator{
                
            public:
                
                KV<int, VALUE, MMAP, DIRECT, REPLACEMENT, DENSE_KEY> * pkv;
                
                int next_idx;
                
                Iterator(KV<int, VALUE, MMAP, DIRECT, REPLACEMENT, DENSE_KEY> * _pkv){
                    next_idx = 0;
                    pkv = _pkv;
                }
                
                bool hasNext(){
                    if(next_idx < pkv->actual_size){
                        return true;
                    }else{
                        return false;
                    }
                }
                
                int next(int & key, VALUE & value){
                    if(next_idx < pkv->actual_size){
                        key = next_idx;
                        value = pkv->mmap_items[key];
                        next_idx++;
                    }else{
                        return NEXT_ERROR_NO_NEXT;
                    }
                    return SUCCESS;
                }
                
            };
            
            Iterator get_iterator(){
                return Iterator(this);
            }
            
            KV(){
                
                std::cout << "Use Specialization KV_int_-_MMAP_DIRECT_-_DENSEKEY ." << std::endl;
                
                size = MMAP_INIT_NELEMENT;
                actual_size = 0;
                
                fd = open(getNextTmpFileName().c_str(), O_RDWR | O_CREAT, (mode_t) 0600);
                assert(fd != -1);
                
                int result_state = lseek(fd, (MMAP_INIT_NELEMENT)* sizeof (VALUE) - 1, SEEK_SET);
                assert(result_state != -1);
                
                result_state = write(fd, "", 1);
                assert(result_state == 1);
                
                mmap_items = new(mmap(0, MMAP_INIT_NELEMENT * sizeof (VALUE), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)) VALUE[MMAP_INIT_NELEMENT];
                
            }
            
            //Iterator get_iterator(){
            //    return Iterator(this);
            //}
            
            KV_Status get(int const & key, VALUE & value){
                
                if(key >= size){
                    return GET_ERROR_KEY_NOT_FOUND;
                }else{
                    value = mmap_items[key];
                }
                
                return SUCCESS;
            }
            
            KV_Status set(int const & key, VALUE const & value){
                
                if(key >= actual_size){
                    return SET_ERROR_KEY_NOT_FOUND;
                }else{
                    mmap_items[key] = value;
                }
                
                return SUCCESS;
            }
            
            KV_Status load(int const & key, VALUE const & value){
                
                if(key >= actual_size){
                    actual_size = key + 1;
                }
                
                if(key >= size){
                    int newSize = size;
                    while(key >= newSize){
                        newSize *= 2;
                    }
                    resize(newSize);
                }
                
                mmap_items[key] = value;
                
                return SUCCESS;
            }
            
        };
        
    }
}


#endif
