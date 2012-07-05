//
//  main.cpp
//  elly
//
//  Created by Ce Zhang on 7/4/12.
//  Copyright (c) 2012 University of Wisconsin-Madison. All rights reserved.
//

#include "utils/Common.h"

#include "utils/OptionParser.h"


namespace elly{
    
    class elly{
    public:
      
        elly(){
            
        }
        
        ~elly(){}
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

    
    
    
    return 0;
}
