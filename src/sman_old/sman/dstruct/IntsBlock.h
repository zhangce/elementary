//
//  IntsBlock.h
//  sman
//
//  Created by Ce Zhang on 8/19/12.
//  Copyright (c) 2012 HazyResearch. All rights reserved.
//

#ifndef sman_IntsBlock_h
#define sman_IntsBlock_h

#include "../common/Common.h"

namespace mia{
    namespace sm{

        /**
         * \brief Pair of integer.
         **/
        class IntPair{
        public:
            int first;
            int second;
        };

        /**
         * \brief A block of integer. Used as a container for variance length object.
         **/
        class IntsBlock{
        public:
            
            int size;
            int content[MemBlockNINT];
    
            IntsBlock() : size(0){}
    
            template<class TYPE>
            void append(TYPE obj){
        
                int nint = sizeof(TYPE)/SIZEOFINT;
                assert(size + nint < MemBlockNINT);
                *((TYPE*) &content[size]) = obj;
                size += nint;
        
            }
    
            template<class TYPE>
            TYPE get(int nint){
                return *((TYPE*) &content[nint]);
            }
    
        };
        
    }
}


#endif
