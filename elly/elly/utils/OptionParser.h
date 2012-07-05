//
//  OptionParser.h
//  elly
//
//  Created by Ce Zhang on 7/4/12.
//  Copyright (c) 2012 University of Wisconsin-Madison. All rights reserved.
//

#ifndef elly_OptionParser_h
#define elly_OptionParser_h

#include "Common.h"

#include "/usr/local/include/boost/program_options.hpp"
namespace po = boost::program_options;

namespace elly {
    namespace utils{
        
        po::options_description parse_options_prepare(){
            
            po::options_description desc("Allowed options");
            desc.add_options()
            
            // common
            ("help", "Produce help message")
            ("config", po::value<std::string>(), "Configuration file. command line options have higher priority than config file.")
            
            // ui.*   (input/output)
            ("ui.verbose", po::value<std::string>(), "Verbose level for STDOUT and STDERR. {trace, debug, info}. [DEFAULT=info]")
            ("ui.logfile", po::value<std::string>(), "Path to log file if need one.")
            ("ui.logverbose", po::value<std::string>(), "Verbose level for log file. {trace, debug, info}. [DEFAULT=debug]")
             
            // rt.*   (runtime)
            ("rt.input", po::value<std::string>(), "Where does input factor graph located in. {file://DIR_PATH}.")
            ("rt.output", po::value<std::string>(), "Where does output located in. {file://FILE_PATH}.")
            ("rt.workdir", po::value<std::string>(), "Directory that contains temporary files.")
            
            ("rt.mode", po::value<std::string>(), "Mode that Elly runs in. {map, marginal, mle, learn}. [DEFAULT=marginal]")
            
            ("rt.thin", po::value<int>(), "Collect one sample every #rt.thin iterations. [DEFAULT=1]")
            ("rt.burnin", po::value<int>(), "Ignore the first #rt.burnin collected samples. [DEFAULT=1]")
            
             
            ;
            
            return desc;
        }
        
        int parse_option_fill_in_config(elly::utils::Config &config, po::variables_map &vm, po::options_description & desc){
            
            if(vm.count("help")){
                elly::utils::log() << desc << std::endl;
                return 1;
            }
            
            if(vm.count("ui.verbose")){
                config.ui_verbose = vm["ui.verbose"].as<std::string>();
                elly::utils::log() << "  | ui.verbose = " << config.ui_verbose << std::endl;
            }
            
            if(vm.count("ui.logfile")){
                config.ui_logfile = vm["ui.logfile"].as<std::string>();
                elly::utils::log() << "  | ui.logfile = " << config.ui_logfile << std::endl;
            }
            
            if(vm.count("ui.logverbose")){
                config.ui_logverbose = vm["ui.logverbose"].as<std::string>();
                elly::utils::log() << "  | ui.logverbose = " << config.ui_logverbose << std::endl;
            }
            
            if(vm.count("rt.input")){
                config.rt_input = vm["rt.input"].as<std::string>();
                elly::utils::log() << "  | rt.input = " << config.rt_input << std::endl;
            }
            
            if(vm.count("rt.output")){
                config.rt_output = vm["rt.output"].as<std::string>();
                elly::utils::log() << "  | rt.output = " << config.rt_output << std::endl;
            }
            
            if(vm.count("rt.workdir")){
                config.rt_workdir = vm["rt.workdir"].as<std::string>();
                elly::utils::log() << "  | rt.workdir = " << config.rt_workdir << std::endl;
            }
            
            if(vm.count("rt.mode")){
                config.rt_mode = vm["rt.mode"].as<std::string>();
                elly::utils::log() << "  | rt.mode = " << config.rt_mode << std::endl;
            }
            
            if(vm.count("rt.thin")){
                config.rt_thin = vm["rt.thin"].as<int>();
                elly::utils::log() << "  | rt.thin = " << config.rt_thin << std::endl;
            }            
            
            if(vm.count("rt.burnin")){
                config.rt_burnin = vm["rt.burnin"].as<int>();
                elly::utils::log() << "  | rt.burnin = " << config.rt_burnin << std::endl;
            }
            
            
            return 0;
            
        }

        void parse_option_post(elly::utils::Config &config){
            
            if(config.rt_input.compare("") == 0){
                throw std::exception(); //TODO
            }

            if(config.rt_output.compare("") == 0){
                throw std::exception(); //TODO
            }

            if(config.rt_workdir.compare("") == 0){
                throw std::exception(); //TODO
            }

        }

        
        int parse_options(elly::utils::Config &config, int ac, const char * av[]){
            
            po::options_description desc = parse_options_prepare();
            
            try{
            
                po::variables_map vm;
                po::store(po::parse_command_line(ac, av, desc), vm);
                po::notify(vm);
        
                if(vm.count("config")){
                    elly::utils::log() << ">> Parsing config file " << vm["config"].as<std::string>() <<  std::endl;
                
                    std::string config_file = vm["config"].as<std::string>();
                
                    std::ifstream ifs(config_file.c_str());
                    if (!ifs){
                        elly::utils::log() << "can not open config file: " << config_file << "\n";
                        return 1;
                    }
                
                    po::variables_map file_vm;
                    po::store(parse_config_file(ifs, desc), file_vm);
                    notify(file_vm);
                
                    parse_option_fill_in_config(config, file_vm, desc);
                
                    ifs.close();
                
                }
                
                elly::utils::log() << ">> Parsing command line options... " << std::endl;
                
                int rs_parse_options_post = parse_option_fill_in_config(config, vm, desc);
                
                parse_option_post(config);
                
                return rs_parse_options_post;
            
            }catch( const std::exception& e){
                
                elly::utils::logerr() << "ERROR: " << e.what() << std::endl;
                elly::utils::log() << desc << std::endl;
                return 1;
            }
            
            return 0;
        }
                
                
    }
}



#endif
