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

#include <vector>

#include <math.h>

#include "Log.h"
#include "Config.h"
#include "Timer.h"


#define LOG_2   0.693147180559945
#define MINUS_LOG_THRESHOLD   -18.42


namespace mia {
namespace elly{
    
    class SampleInput{
        
    public:
        
        ~SampleInput(){
            for(int i=0;i<mbs.size();i++){
                delete mbs[i];
            }
        }
        
        std::vector<int> pos_of_sample_variable;
        std::vector<int> auxs;
        
        std::vector<void*> mbs;
        
        std::vector<int> crids;
        std::vector<int> fids;
        
        std::vector<int> funcids;
        
        int vid;
        int vvalue;
        int vdomain;
        
        void print(){
            
            mia::elly::utils::log() << "######### Sample Input for VID=" << vid << 
               " DOMAIN=" << vdomain <<
            " ########" << std::endl;

            for(int nfactor=0;nfactor<mbs.size();nfactor++){
                mia::elly::utils::log() << "Factor #" << nfactor;
                mia::elly::utils::log() << " CRID=" << crids[nfactor];
                mia::elly::utils::log() << " FID=" << fids[nfactor];   
                mia::elly::utils::log() << " FUNCID=" << funcids[nfactor];
                mia::elly::utils::log() << " AUX=" << auxs[nfactor];
                
                mia::elly::utils::log() << " TOSAMPLE=" << pos_of_sample_variable[nfactor] ;
                
                mia::elly::utils::log() << "; ";
                
                std::vector<int> * pmb = (std::vector<int> *) mbs[nfactor];
                
                if(pos_of_sample_variable[nfactor] != -1){
                    for(int nvar = 0; nvar < pmb->size(); nvar++){
                        mia::elly::utils::log() << "V" << nvar << "==" << pmb->at(nvar) << " ";
                    }
                }else{
                    mia::elly::utils::log() << "INC. FACTOR " ;
                }
                
                mia::elly::utils::log() << std::endl;
                
            }
            
            mia::elly::utils::log() << "##################################################" << std::endl;
            
        }
        
    };
    
    
    namespace utils{
                
        struct FactorTuple{
            int factor_id;
            int variable_id;
            int pos;
            int aux;
        };
        
        
        inline bool fast_exact_is_equal(double a, double b){
            return (a <= b && b <= a);
        }
        
        inline double logadd(double log_a, double log_b){
            
            if (log_a < log_b)
            { // swap them
                double tmp = log_a;
                log_a = log_b;
                log_b = tmp;
            } else if (fast_exact_is_equal(log_a, log_b)) {
                // Special case when log_a == log_b. In particular this works when both
                // log_a and log_b are (+-) INFINITY: it will return (+-) INFINITY
                // instead of NaN.
                return LOG_2 + log_a;
            }
            double negative_absolute_difference = log_b - log_a;
            if (negative_absolute_difference < MINUS_LOG_THRESHOLD)
                return (log_a);
            return (log_a + log1p(exp(negative_absolute_difference)));
            
        }

        
        
        
        
        
        
    }
}
}


#endif
