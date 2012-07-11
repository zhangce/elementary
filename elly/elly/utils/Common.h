//
//  Common.h
//  elly
//
//  Created by Ce Zhang on 7/4/12.
//  Copyright (c) 2012 University of Wisconsin-Madison. All rights reserved.
//

#ifndef elly_Common_h
#define elly_Common_h

#include <string>
#include <fstream>
#include <iostream>

#include "Log.h"
#include "Config.h"


namespace elly{
    namespace utils{
                
        struct FactorTuple{
            int factor_id;
            int variable_id;
            int pos;
            int aux;
        };
        
    }
}



#endif
