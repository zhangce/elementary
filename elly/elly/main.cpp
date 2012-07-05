//
//  main.cpp
//  elly
//
//  Created by Ce Zhang on 7/4/12.
//  Copyright (c) 2012 University of Wisconsin-Madison. All rights reserved.
//

#include "utils/Common.h"

#include "utils/OptionParser.h"

#include "inf/Inf_marginal.h"


namespace elly{
    
    class Elly{
    public:
        
        elly::utils::Config config;
        
        Elly(elly::utils::Config &_config){
            config = _config;
        }
        
        void run(){
            
            if(config.rt_mode.compare("map") == 0){
                
            }

            if(config.rt_mode.compare("marginal") == 0){
                elly::inf::Inf_marginal inf(config);
                inf.infer();
            }

            if(config.rt_mode.compare("mle") == 0){
                
            }

            if(config.rt_mode.compare("learn") == 0){
                
            }

            
        }
        
        ~Elly(){}
    };
    
}



int main(int argc, const char * argv[])
{

    elly::utils::Config config;
    
    elly::utils::log() << "##### Elly " << 
            config.version_number << 
            " #####" << std::endl;
    
    int rs_parse_options = elly::utils::parse_options(config, argc, argv);

    if (rs_parse_options != 0){
        return 0;
    }

    elly::Elly elly_instance(config);
    elly_instance.run();
    
    
    return 0;
}
