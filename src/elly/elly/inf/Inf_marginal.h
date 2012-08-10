//
//  inf_marginal.h
//  elly
//
//  Created by Ce Zhang on 7/4/12.
//  Copyright (c) 2012 University of Wisconsin-Madison. All rights reserved.
//

#ifndef elly_inf_marginal_h
#define elly_inf_marginal_h

#include "../utils/Config.h"

namespace mia{
namespace elly {
    namespace inf{
        
        class Inf_marginal{
        public:
            
            mia::elly::utils::Config config;
            
            Inf_marginal(mia::elly::utils::Config &_config){
                config = _config;
            }
            
            void infer(){
                mia::elly::utils::log() << ">> Executing marginal inference..." << std::endl;
            }
            
        };
        
    }
}
}


#endif
