//
//  Config.h
//  elly
//
//  Created by Ce Zhang on 7/4/12.
//  Copyright (c) 2012 University of Wisconsin-Madison. All rights reserved.
//

#ifndef elly_Config_h
#define elly_Config_h

#include <string>

namespace mia{
namespace elly{
    namespace utils{
        
        /**
         * Configurations to run elly. \sa mia::elly::utils::OptionParser.
         */
        class Config{
        public:
           
            std::string version_number;
         

            std::string ui_verbose;
            std::string ui_logfile;
            std::string ui_logverbose;
            
            std::string rt_input;
            std::string rt_output;
            std::string rt_workdir;
            
            std::string rt_mode;
            
            int rt_thin;
            int rt_burnin;
            int rt_nepoch;
            
            double rt_learn_initstep;
            double rt_learn_decay;
            
            bool rt_lock;
            bool rt_is_log_system;
            
            int sys_nthreads;
            
            bool io_ismln;
            std::string io_mln;
            
            Config(){
                
                version_number = "0.1";
                
                ui_verbose = "info";
                ui_logfile = "";
                ui_logverbose = "debug";
                
                rt_input   = "";
                rt_output  = "";
                rt_workdir = "";
                
                rt_mode    = "";
                
                rt_thin    = 1;
                rt_burnin  = 1;
                rt_nepoch  = 10;
                
                rt_learn_initstep = 0.01;
                rt_learn_decay = 1.0;
                
                sys_nthreads = 1;
                
                io_ismln = false;
                
                io_mln = "";
                
                rt_lock = true;
                
                rt_is_log_system = true;
                
            }
            
            
        };
        
    }
}
}



#endif
