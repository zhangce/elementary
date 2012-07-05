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

namespace elly {
    namespace inf{
        
        class Inf_marginal{
        public:
            
            elly::utils::Config config;
            
            Inf_marginal(elly::utils::Config &_config){
                config = _config;
            }
            
            void infer(){
                elly::utils::log() << ">> Executing marginal inference..." << std::endl;
            }
            
        };
        
    }
}


#endif
