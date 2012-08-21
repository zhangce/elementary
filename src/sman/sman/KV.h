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
    
        /**
         * Error codes returned by KV's function.
         **/
        enum KV_Status {SUCCESS,
                        
                        GET_ERROR_KEY_NOT_FOUND,
                        
                        SET_ERROR_KEY_NOT_FOUND,
            
                        NEXT_ERROR_NO_NEXT
                        };
        
        /**
         * \brief Interface of key-value storage.
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
            
            /**
             * \brief Iterator for key-value store.
             **/
            class Iterator{
            public:
                
                /**
                 * Whether there are more (key, value) pair to visit.
                 *
                 * \return true if any.
                 **/
                bool hasNext();
                
                /**
                 * Fill in key and value using the next (key, value) pair. 
                 *
                 * \param reference to key
                 * \param reference to value
                 *
                 * \return KV_Status
                 **/
                int next(KEY & key, VALUE & value);
            };
            
            /**
             * Constructor of key-value store.
             **/
            KV(){
                std::cout << "No Specialization to use!" << std::endl;
                throw std::exception();
            }
        
            /**
             * Fill in value by looking up key.
             **/
            KV_Status get(KEY const & key, VALUE & value);
            
            /**
             * Update key's value to value.
             **/
            KV_Status set(KEY const & key, VALUE const & value);
            
            /**
             * Insert a (key, value) pair.
             **/
            KV_Status load(KEY const & key, VALUE const & value);
            
            /**
             * Update or insert a (key,value) pair. Wrapper for set() and load()
             **/
            KV_Status put(KEY const & key, VALUE const & value){
                
                if( set(key, value) != SUCCESS ){
                    return load(key, value);
                }else{
                    return SUCCESS;
                }
                
            }
            
            /**
             * Return an iterator to use.
             **/
            Iterator get_iterator(){
                return Iterator(this);
            }
            
        };
        
        
        
    }
}


#endif
