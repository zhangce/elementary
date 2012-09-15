//
//  ObjStore.h
//  SMan
//
//  Created by Ce Zhang on 9/7/12.
//  Copyright (c) 2012 HazyResearch. All rights reserved.
//

#ifndef SMan_ObjStore_h
#define SMan_ObjStore_h

#include "../common/Common.h"

namespace hazy{
  namespace sman{
    
    /**
     * An ObjStore is a special key-value store that only stores fixed-length
     * values. A general key-value store will be built upon an ObjStore by
     * using slotted pages (a popular design that is used by PG or leveldb).
     * 
     * \todo{In the current implementation, we are assuming integer dense key.
     * In the future we will add in new template parameters "class KEY" and 
     * "HintType HINT" to model this assumption and implement more general cases.}
     *
     */
    template<class VALUE, StorageType STORAGE, PropertyType PROPERTY>
    class ObjStore{
      
    public:
      ObjStore(){
        std::cout << "ERROR: " << "No specification available!" << std::endl;
        exit(1);
      }
      
      /**
       * Get value by looking up key.
       *
       * \todo{In the current implementation, we assume all keys are valid and appear.}
       */
      StatusType get(int64_t key, VALUE & value){}
  
      /**
       * Update value for certain key.
       *
       * \todo{In the current implementation, we assume all keys are valid and appear.}
       */
      StatusType set(int64_t key, const VALUE & value){}
  
      /**
       * Load value for certain key.
       *
       * \todo{In the current implementation, we assume all keys are valid.}
       */
      StatusType load(int64_t key, const VALUE & value){}

      /**
       * Update value for certain key by applying a function to it.
       *
       * \todo{In the current implementation, we assume all keys are valid and appear.}
       */
      StatusType apply(int64_t key, void (*func)(VALUE &)){}
            
    };
    
    
  }
}


#endif
