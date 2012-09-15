//
//  KV_-_-_MM_DIRECT_-.h
//  sman
//
//  Created by Ce Zhang on 8/18/12.
//  Copyright (c) 2012 HazyResearch. All rights reserved.
//

#ifndef sman_KV_____MM_DIRECT___h
#define sman_KV_____MM_DIRECT___h

#include "../KV.h"

#include <map>

namespace mia{
    namespace sm{
        
        /**
         * \brief Key-value store for arbitary key and value type in main memory.
         **/
        template<class KEY, class VALUE, KV_Replacement REPLACEMENT, KV_Hint HINT>
        class KV<KEY, VALUE, MM, DIRECT, REPLACEMENT, HINT>{
            
        private:
            
            std::map<KEY, VALUE> mm_map;
                        
            friend class Iterator;
            
        public:
            
            class Iterator{
            
            public:
                
                KV<KEY, VALUE, MM, DIRECT, REPLACEMENT, HINT> * pkv;
                
                typename std::map<KEY,VALUE>::iterator it;
                
                Iterator(KV<KEY, VALUE, MM, DIRECT, REPLACEMENT, HINT> * _pkv){
                    
                    pkv = _pkv;
                    it = pkv->mm_map.begin();
                    
                }
                
                bool hasNext(){
                    return (it != pkv->mm_map.end());
                }
                
                int next(KEY & key, VALUE & value){
                    if(it != pkv->mm_map.end()){
                        key = it->first;
                        value = it->second;
                        it++;
                    }else{
                        return NEXT_ERROR_NO_NEXT;
                    }
                    return SUCCESS;
                }
            
            };
            
            KV(){
                std::cout << "Use Specialization KV_-_-_MM_DIRECT_-_- ." << std::endl;
            }
            
            Iterator get_iterator(){
                return Iterator(this);
            }
            
            KV_Status get(KEY const & key, VALUE & value){
                if(mm_map.find(key) == mm_map.end()){
                    return GET_ERROR_KEY_NOT_FOUND;
                }else{
                    value = mm_map[key];
                }
                return SUCCESS;
            }
            
            KV_Status set(KEY const & key, VALUE const & value){
                if(mm_map.find(key) == mm_map.end()){
                    return SET_ERROR_KEY_NOT_FOUND;
                }else{
                    mm_map[key] = value;
                }
                return SUCCESS;
            }
            
            KV_Status load(KEY const & key, VALUE const & value){
                mm_map[key] = value;
                return SUCCESS;
            }
            
            KV_Status put(KEY const & key, VALUE const & value){
                
                if( set(key, value) != SUCCESS ){
                    return load(key, value);
                }else{
                    return SUCCESS;
                }
                
            }

            
            
        };
        
    }
}


#endif
