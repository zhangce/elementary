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

#include <semaphore.h>


#define LOG_2   0.693147180559945
#define MINUS_LOG_THRESHOLD   -18.42


namespace mia {
namespace elly{
    
    /**
     * \brief The class for a sample task of one variable.
     */
    class SampleInput{
        
    public:
        
        ~SampleInput(){
        //    for(int i=0;i<mbs.size();i++){
        //        delete mbs[i];
        //    }
        }
        
        /**
         * Each entry corresponds to a factor. Each entry is the position of target variable in that factor.
         */
        std::vector<int> pos_of_sample_variable;

        /**
         * Each entry corresponds to a factor. Each entry is weight index of that factor.
         */
        std::vector<int> auxs;
        
        /**
         * Each entry corresponds to a factor. Each entry is aux info. of that factor.
         */
        std::vector<int> aux2s;
        
        /**
         * Each entry corresponds to a factor. Each entry is pointer to weights vector of that factor.
         */
        std::vector<std::vector<double>* > weights;
        
        /**
         * Each entry corresponds to a factor. Each entry is pointer to factor state
         */
        std::vector<void*> mbs;
        
        /**
         * Each entry corresponds to a factor. Each entry is correlation relation ID.
         */
        std::vector<int> crids;
        
        /**
         * Each entry corresponds to a factor. Each entry is factor state ID.
         */
        std::vector<int> fids;
        
        /**
         * Each entry corresponds to a factor. Each entry is factor function ID.
         */
        std::vector<int> funcids;
        
        /**
         * Target variable ID.
         */
        int vid;
        
        /**
         * Current assignment of target variable.
         */
        int vvalue;
        
        /**
         * Training assignment of target variable. -1 if not unknown.
         */
        int vtrain;
        
        /**
         * Domain of target variable. a variable can take value from 0,...,vdomain-1
         */
        int vdomain;
        
        /**
         * Step size for learning.
         */
        double stepSize;
        
        /**
         * Filled by mia::elly:alg::GibbsSampling for the improvement of new sample assignment compared with orginal assignment.
         */
        double log_improve_ratio;
        
        /**
         * Pring the content.
         */
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





/**
 * Namespace of Mia.
 */
namespace mia{
    
    /**
     * Namespace of Elly -- the inference engine.
     */
    namespace elly{
        
    }
    
}




#endif
