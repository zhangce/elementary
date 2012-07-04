//
//  OptionParser.h
//  elly
//
//  Created by Ce Zhang on 7/4/12.
//  Copyright (c) 2012 University of Wisconsin-Madison. All rights reserved.
//

#ifndef elly_OptionParser_h
#define elly_OptionParser_h

#include <string>
#include <fstream>
#include <iostream>
#include "Config.h"

#include "/usr/local/include/boost/program_options.hpp"
namespace po = boost::program_options;

namespace elly {
    namespace utils{
        
        po::options_description parse_options_prepare(){
            
            utils::config::version_number = "0.1";
            
            po::options_description desc("Allowed options");
            desc.add_options()
            ("help", "produce help message")
            ("verbose", po::value<int>(), "set verbose level. (1-5). [DEFAULT=1]")
            ("config", po::value<std::string>(), "configuration file. command line options have higher priority than config file.")
            
            ;
            
            return desc;
        }
        
        int parse_options_post(po::variables_map &vm, po::options_description & desc){
            
            if(vm.count("help")){
                std::cout << desc << std::endl;
                return 1;
            }
            
            
            return 0;
            
        }
        
        
        int parse_options(int ac, const char * av[]){
            
            po::options_description desc = parse_options_prepare();
            
            po::variables_map vm;
            po::store(po::parse_command_line(ac, av, desc), vm);
            po::notify(vm);
        
            if(vm.count("config")){
                std::cout << "config file = " << vm["config"].as<std::string>() << std::endl;
                
                std::string config_file = vm["config"].as<std::string>();
                
                std::ifstream ifs(config_file.c_str());
                if (!ifs){
                    std::cout << "can not open config file: " << config_file << "\n";
                    return 1;
                }
                
                po::variables_map file_vm;
                po::store(parse_config_file(ifs, desc), file_vm);
                notify(file_vm);
                
                parse_options_post(file_vm, desc);
                
                ifs.close();
                
            }
            
            return parse_options_post(vm, desc);
        }
                
                
    }
}



#endif
