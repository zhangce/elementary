//
//  Log.h
//  elly
//
//  Created by Ce Zhang on 7/4/12.
//  Copyright (c) 2012 University of Wisconsin-Madison. All rights reserved.
//

#ifndef elly_Log_h
#define elly_Log_h

#include <iostream>

namespace elly{
    namespace utils{
    
        // TODO:
        std::ostream& log(){
            return std::cout;
        }
        
        std::ostream& logerr(){
            return std::cerr;
        }
    
    }
}



#endif
