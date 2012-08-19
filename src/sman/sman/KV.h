//
//  KV.h
//  sman
//
//  Created by Ce Zhang on 8/18/12.
//  Copyright (c) 2012 HazyResearch. All rights reserved.
//

#ifndef sman_KV_h
#define sman_KV_h


namespace mia{
    namespace sm{
    
        enum KV_Buffer {DIRECT};
        enum KV_Replacement {NIL, LRU};
        enum KV_Storage {MM, MMAP};
        enum KV_Hint {NO_HINT, DENSE_KEY};
    
        enum KV_Status {SUCCESS,
                        
                        GET_ERROR_KEY_NOT_FOUND,
                        
                        SET_ERROR_KEY_NOT_FOUND,
            
                        NEXT_ERROR_NO_NEXT
                        };
        
        /**
         * Interface of key-value storage.
         **/
        template<
                  class KEY,    /** Type of key. **/
                  class VALUE,  /** Type of value. **/
                  KV_Storage STORAGE,   /** Storage to use. **/
                  KV_Buffer BUFFER,     /** Buffer to use. **/
                  KV_Replacement REPLACEMENT,    /** Replace policy to use. **/
                  KV_Hint HINT  /** Performance hint. **/
                >
        class KV{
        
        public:
            
            class Iterator{};
            
            KV(){
                std::cout << "No Specialization to use!" << std::endl;
                throw std::exception();
            }
        
            
            
            KV_Status get(KEY const & key, VALUE & value);
            
            KV_Status set(KEY const & key, VALUE const & value);
            
            KV_Status load(KEY const & key, VALUE const & value);
            
            KV_Status put(KEY const & key, VALUE const & value){
                
                if( set(key, value) != SUCCESS ){
                    return load(key, value);
                }else{
                    return SUCCESS;
                }
                
            }
            
            Iterator get_iterator(){
                return Iterator(this);
            }
            
        };
        
        
        
    }
}


#endif
