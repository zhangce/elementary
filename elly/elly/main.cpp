//
//  main.cpp
//  elly
//
//  Created by Ce Zhang on 7/4/12.
//  Copyright (c) 2012 University of Wisconsin-Madison. All rights reserved.
//

#include <iostream>
#include "utils/OptionParser.h"


int main(int argc, const char * argv[])
{

    int rs_parse_options = elly::utils::parse_options(argc, argv);

    std::cout << "##### Elly " << elly::utils::config::version_number << " #####"<< std::endl;
    
    if (rs_parse_options == 1){
        return 0;
    }

    
    
    
    return 0;
}
