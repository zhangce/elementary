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

#include "../../../SMan/common/Common.h"
#include "boost/program_options.hpp"
namespace po = boost::program_options;

namespace mia {
namespace elly {
    
    /*!
     *  \brief utils used by elly.
     */
    namespace utils{
        
        /*!
         * prepare an object of boost::program_options::options_description
         * used by boost::program_options.
         *
         * \return object of boost::program_options::options_description
         */
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
            ("rt.nepoch", po::value<int>(), "Number of iterations (epochs) to run. [DEFAULT=10]")
            
            ("rt.learn_initstep", po::value<double>(), "Initial step size for SGD learning. [DEFAULT=0.01]")
            ("rt.learn_decay", po::value<double>(), "Decay factor for SGD learning step size after each epoch. [DEFAULT=1.00]")
            
            ("rt.lock", po::value<bool>(), "Use lock for parallel Gibbs sampling. [DEFAULT=true]")
            
            ("rt.is_log_system", po::value<bool>(), "Factor function returns in log scale. [DEFAULT=true]")
            
                    // sys.*    (system)
            ("sys.threads", po::value<int>(), "Number of threads to use. [DEFAULT=1]")
          
                    // exp.*    (experiment)
            ("exp.materialization", po::value<std::string>(), "Materialization strategy to use. {LAZY, VCOC, FCOC, EAGER}.")
            ("exp.storage", po::value<std::string>(), "Storage to use. {STORAGE_MM, STORAGE_HBASE}.")
            ("exp.replacement", po::value<std::string>(), "Replacement strategy to use. {RANDOM, LRU}.")
            ("exp.pagesize_bytes", po::value<int>(), "Pagesize in bytes.")
            ("exp.buffersize_bytes", po::value<int>(), "Buffersize in bytes.")
                      
            ;
            
            return desc;
        }
        
        
        /*!
         * fill in configuration into elly::utils::Config object, which will be
         * used by an elly::Elly object.
         *
         * \param config elly::utils::Config object to fill in
         * \param po::variables_map see details in http://www.boost.org/doc/libs/1_50_0/doc/html/program_options/tutorial.html
         * \param po::options_description see details in http://www.boost.org/doc/libs/1_50_0/doc/html/program_options/tutorial.html
         *
         */
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
            hazy::sman::config_tmp_folder = config.rt_workdir;
            
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
            
            if(vm.count("rt.nepoch")){
                config.rt_nepoch = vm["rt.nepoch"].as<int>();
                elly::utils::log() << "  | rt.nepoch = " << config.rt_nepoch << std::endl;
            }
            
            if(vm.count("rt.learn_initstep")){
                config.rt_learn_initstep = vm["rt.learn_initstep"].as<double>();
                elly::utils::log() << "  | rt.learn_initstep = " << config.rt_learn_initstep << std::endl;
            }
            
            if(vm.count("rt.learn_decay")){
                config.rt_learn_decay = vm["rt.learn_decay"].as<double>();
                elly::utils::log() << "  | rt.learn_decay = " << config.rt_learn_decay << std::endl;
            }
            
            if(vm.count("rt.lock")){
                config.rt_lock = vm["rt.lock"].as<bool>();
                elly::utils::log() << "  | rt.lock = " << config.rt_lock << std::endl;
            }
            
            if(vm.count("rt.is_log_system")){
                config.rt_is_log_system = vm["rt.is_log_system"].as<bool>();
                elly::utils::log() << "  | rt.is_log_system = " << config.rt_is_log_system << std::endl;
            }

            
            if(vm.count("sys.threads")){
                config.sys_nthreads = vm["sys.threads"].as<int>();
                elly::utils::log() << "  | sys.threads = " << config.sys_nthreads << std::endl;
            }
            
          if(vm.count("exp.materialization")){
            config.exp_materialization = vm["exp.materialization"].as<std::string>();
            elly::utils::log() << "  | exp.materialization = " << config.exp_materialization << std::endl;
          }
          
          if(vm.count("exp.storage")){
            config.exp_storage = vm["exp.storage"].as<std::string>();
            elly::utils::log() << "  | exp.storage = " << config.exp_storage << std::endl;
          }
          
          if(vm.count("exp.replacement")){
            config.exp_replacement = vm["exp.replacement"].as<std::string>();
            elly::utils::log() << "  | exp.replacement = " << config.exp_replacement << std::endl;
          }
          
          
          config.exp_pagesize_bytes = COMMON_PAGESIZE;
          std::cout << "  | exp.exp_pagesize_bytes = " << COMMON_PAGESIZE << " bytes." << std::endl;
          
          if(vm.count("exp.buffersize_bytes")){
            config.exp_buffersize_bytes = vm["exp.buffersize_bytes"].as<int>();
            elly::utils::log() << "  | exp.buffersize_bytes = " << config.exp_buffersize_bytes << std::endl;
          }
          
          COMMON_PAGESIZE_BYTES = config.exp_pagesize_bytes;
          COMMON_BUFFERSIZE_BYTES = config.exp_buffersize_bytes;
          COMMON_NBUFFER = COMMON_BUFFERSIZE_BYTES/COMMON_PAGESIZE_BYTES;
          
          if(!(COMMON_NBUFFER>0)){
            std::cout << "config.exp_buffersize_bytes should be at least larger than config.exp_pagesize_bytes" << std::endl;
            assert(false);
          }else{
            std::cout << "  | NBUFFER = " << COMMON_NBUFFER << "" << std::endl;
          }
          
          
            return 0;
            
        }

        /*!
         * Given an object of elly::utils::Config, check whether it is good to use.
         *
         * \param config elly::utils::Config object to check 
         * \throw std::exception if it is not a good elly::utils::Config.
         */
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
          
          if(config.exp_materialization.compare("") == 0){
            throw std::exception(); //TODO
          }
          
          if(config.exp_storage.compare("") == 0){
            throw std::exception(); //TODO
          }
          
          if(config.exp_replacement.compare("") == 0){
            throw std::exception(); //TODO
          }
          
          if(config.exp_pagesize_bytes == -1){
            throw std::exception(); //TODO
          }
          
          if(config.exp_buffersize_bytes == -1){
            throw std::exception(); //TODO
          }
          
          
        }

        /*!
         * 
         * use the input of an main() function, fill in the an object of elly::utils::Config.
         *
         * \param config an object of elly::utils::Config to fill in
         * \param ac argc in main()
         * \param av argv in main()
         * \return 1 if there is "--help" in options; 0 otherwise.
         */
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
                
                elly::utils::logerr() << "ERROR: " << e.what() <<
              std::endl;
                elly::utils::log() << desc << std::endl;
                return 1;
            }
            
            return 0;
        }
                
                
    }
}
}



#endif
