//
//  KV_-_-_MMAP_DIRECT_-_-.h
//  sman
//
//  Created by Ce Zhang on 8/19/12.
//  Copyright (c) 2012 HazyResearch. All rights reserved.
//

#ifndef sman_KV_____MMAP_DIRECT_____h
#define sman_KV_____MMAP_DIRECT_____h


#include "../KV.h"

#include <map>

namespace mia{
    namespace sm{
        
        template<class KEY, class VALUE, KV_Replacement REPLACEMENT, KV_Hint HINT>
        class KV<KEY, VALUE, MMAP, DIRECT, REPLACEMENT, HINT>{
            
        public:
                        
            KV(){
                std::cout << "KV_-_-_MMAP_DIRECT_-_- Not Implemented yet." << std::endl;
                throw std::exception();
            }
                        
        };
        
    }
}


#endif
