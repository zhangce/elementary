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

namespace mia {
namespace elly{
    namespace utils{
    
        // return an output stream for logging.
        std::ostream& log(){
            return std::cout;
        }

        // return an output stream for logging errors.
        std::ostream& logerr(){
            return std::cerr;
        }
    
    }
}
}



#endif
