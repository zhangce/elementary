//
//  KV_int_-_MM_DIRECT_-_NOHINT.h
//  sman
//
//  Created by Ce Zhang on 8/18/12.
//  Copyright (c) 2012 HazyResearch. All rights reserved.
//

#ifndef sman_KV_int___MM_DIRECT___NOHINT_h
#define sman_KV_int___MM_DIRECT___NOHINT_h

#include "../KV.h"
#include "../dstruct/SlottedPage.h"

#include <vector>

namespace mia{
    namespace sm{
        
        template<class VALUE, KV_Replacement REPLACEMENT>
        class KV<int, VALUE, MM, DIRECT, REPLACEMENT, DENSE_KEY>{
            
        private:
            
            std::vector<VALUE> mm_vec;
            
            friend class Iterator;
            
        public:
                     
            class Iterator{
                
            public:
                
                KV<int, VALUE, MM, DIRECT, REPLACEMENT, DENSE_KEY> * pkv;
                
                int next_idx;
                
                Iterator(KV<int, VALUE, MM, DIRECT, REPLACEMENT, DENSE_KEY> * _pkv){
                    next_idx = 0;
                    pkv = _pkv;
                }
                
                bool hasNext(){
                    if(next_idx < pkv->mm_vec.size()){
                        return true;
                    }else{
                        return false;
                    }
                }
                
                int next(int & key, VALUE & value){
                    if(next_idx < pkv->mm_vec.size()){
                        key = next_idx;
                        value = pkv->mm_vec[key];
                        next_idx++;
                    }else{
                        return NEXT_ERROR_NO_NEXT;
                    }
                    return SUCCESS;
                }
                
            };
            
            KV(){
                std::cout << "Use Specialization KV_int_-_MM_DIRECT_-_DENSEKEY ." << std::endl;
            }
            
            Iterator get_iterator(){
                return Iterator(this);
            }
            
            KV_Status get(int const & key, VALUE & value){
                
                if(key >= mm_vec.size()){
                    return GET_ERROR_KEY_NOT_FOUND;
                }else{
                    value = mm_vec[key];
                }
                
                return SUCCESS;
            }
            
            KV_Status set(int const & key, VALUE const & value){
                
                if(key >= mm_vec.size()){
                    return SET_ERROR_KEY_NOT_FOUND;
                }else{
                    mm_vec[key] = value;
                }
                
                return SUCCESS;
            }
            
            KV_Status load(int const & key, VALUE const & value){
                
                 if(key >= mm_vec.size()){
                     VALUE v;
                        
                     for(int i=mm_vec.size(); i<=key; i++){
                         mm_vec.push_back(v);
                     }
                 }
                mm_vec[key] = value;

                return SUCCESS;
            }
            
        };
        
    }
}


#endif
