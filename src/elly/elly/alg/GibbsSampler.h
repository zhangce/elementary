//
//  GibbsSampler.h
//  elly
//
//  Created by Ce Zhang on 7/28/12.
//  Copyright (c) 2012 University of Wisconsin-Madison. All rights reserved.
//

/**
 *
 * \file This file contains functions that sample assignments
 * for a single variable.
 *
 **/

#ifndef elly_GibbsSampler_h
#define elly_GibbsSampler_h

#include "../utils/Common.h"
#include "../factors/factor_inits.h"

namespace mia {
    namespace elly{
        
        /**
         * Namespace for sampling algorithms.
         */
        namespace alg{
            
                        
            /**
             * Randomly shuffle the assignment of a mia::elly::SampleInput object.
             *
             * \param sampleInput reference to a sample task
             *
             * \return assigned value to mia::elly::SampleInput.
             *
             * \sa mia::elly::SampleInput
             **/
            int Shuffle(mia::elly::SampleInput & sampleInput){
                
                //mia::elly::utils::log() << ">>> Suffle variable ID=" << sampleInput.vid << std::endl;
                
                int targetValue = rand() % sampleInput.vdomain;
                
                //mia::elly::utils::log() << "  | V" << sampleInput.vid << " <~~ " << targetValue << " from " << sampleInput.vvalue << std::endl;

                return targetValue;
                  
            }
            
            /**
             * Sample new assignment of a mia::elly::SampleInput object.
             * Also, mia::elly::SampleInput::log_improve_ratio will be filled
             * in by the improvement of probability of new assignment
             * compared with original assignment. If mia::elly::SampleInput
             * contains training data (mia::elly::sampleInput::vtrain>0), also
             * update weights by calling gradient function for each factor.
             *
             * \param sampleInput reference to a sample task
             * \param thread_id a number in 0~#threads
             * \param vector_pool object pool to avoid re-allocate of vectors, vector_pool[thread_id] is a vector.
             * \param is_log_system true if all factors return in log scale, otherwise linear scale (weight vs. exp(weight) ).
             *
             * \return assigned value to mia::elly::SampleInput. Also the 
             *
             * \sa mia::elly::SampleInput
             **/
            int GibbsSampling(mia::elly::SampleInput & sampleInput, int thread_id, std::vector<double>* vector_pool, bool is_log_system = true){
                
                //mia::elly::utils::log() << ">>> Sample variable ID=" << sampleInput.vid << std::endl;
                
                // for each factor
                int crid, fid, aux, vpos, funcid, aux2;
                void* mb;
                
                std::vector<double>* weights;

                std::vector<double>* potentials = &vector_pool[thread_id*3];
                //std::vector<double>* upper_ratios = &vector_pool[thread_id*3+1];
                //std::vector<double>* lower_ratios = &vector_pool[thread_id*3+2];
                
                //std::vector<double> potentials;
                if(sampleInput.vdomain > potentials->size()){
                    for(int i=potentials->size()-1; i<sampleInput.vdomain; i++ ){
                        potentials->push_back(0);
                    
                        //if(has_linear_upper_bound){
                        //    upper_ratios->push_back(0);
                        //    lower_ratios->push_back(0);
                        //}

                    }                    
                }
                
                for(int i=0;i<sampleInput.vdomain;i++){
                   
                    if(is_log_system){
                        potentials->at(i) = 0;
                    }else{
                        potentials->at(i) = 1;
                    }
                    
                    //if(has_linear_upper_bound){
                    //    upper_ratios->at(i) = 1;
                    //    lower_ratios->at(i) = 1;
                    //}
                }
                

                                
                for(int nf=0; nf<sampleInput.fids.size();nf ++){
                    
                    crid = sampleInput.crids[nf];
                    fid = sampleInput.fids[nf];
                    aux = sampleInput.auxs[nf];
                    aux2 = sampleInput.aux2s[nf];
                    mb = sampleInput.mbs[nf];
                    vpos = sampleInput.pos_of_sample_variable[nf];
                    funcid = sampleInput.funcids[nf];
                    weights = sampleInput.weights[nf];
                    
                    //#pragma omp parallel for
                    for(int value=0; value < sampleInput.vdomain; value ++){

                        double potential = funcs_potential[funcid](mb, aux, aux2, vpos, value, weights);
                        
                        //std::cout << "funcid = " << funcid << ", vpos = " << vpos << "; value = " << value << ": " << potential << std::endl;
                        
                        if(is_log_system){
                            potentials->at(value) += potential;
                        }else{
                            potentials->at(value) *= potential;
                            
                            //if(nf == 2){
                                // todo: add in#threads
                            //if(funcs_upper[funcid] != NULL){
                            //    upper_ratios->at(value) *= funcs_upper[funcid](mb, aux, aux2, vpos, value, weights);
                            //
                            //    lower_ratios->at(value) *= potential;
                            //}else{
                            //    upper_ratios->at(value) *= funcs_upper[funcid](mb, aux, aux2, vpos, value, weights);
                            //
                            //    lower_ratios->at(value) *= potential;
                            //}
                            
                            //}
                            //std::cout << "~~~" << potential << std::endl;
                            
                        }
                    }
                    
                    
                }
                
                //for(int value=0; value < sampleInput.vdomain; value ++){
                //    std::cout << lower_ratios->at(value) << ", " << potentials->at(value) << ", " << upper_ratios->at(value) << std::endl;
                //}
                
                double pfunc;
                
                if(is_log_system){
                    pfunc = -10000000;
                }else{
                    pfunc = 0;
                }
                
                for(int value=0; value < sampleInput.vdomain; value ++){
                    
                    if(is_log_system){
                        pfunc = mia::elly::utils::logadd(potentials->at(value), pfunc);
                    }else{
                        pfunc += potentials->at(value);
                    }
                }
                
                //mia::elly::utils::log() << "  | log potential func = " << pfunc << std::endl;
                
                
                double random = drand48();
                //mia::elly::utils::log() << "  | random = " << random << std::endl;
                
                double accum = 0;
                double pmeta = 0;
                int targetValue = -1;
                for(int value=0; value < sampleInput.vdomain; value ++){
                    
                    if(is_log_system){
                        pmeta = exp( potentials->at(value) - pfunc );
                    }else{
                        pmeta = potentials->at(value)/pfunc;
                    }
                    
                    //mia::elly::utils::log() << "    + Pr[V" << sampleInput.vid << " = " << value << "] = " << pmeta << std::endl;
                    
                    accum += pmeta;
                    
                    if(random < accum){
                        targetValue = value;
                        break;
                    }
                    
                }
                
                //if(targetValue == -1){
                //    std::cout << "accum = " << accum << std::endl;
                //}
                assert(targetValue != -1);
                
                if(is_log_system){
                    sampleInput.log_improve_ratio = potentials->at(targetValue) - potentials->at(sampleInput.vvalue);
                }else{
                    sampleInput.log_improve_ratio = potentials->at(targetValue) /potentials->at(sampleInput.vvalue);
                }
                
                double gradient;
                
                if(sampleInput.vtrain >=0){
                    
                    //std::cout << "sample = " << targetValue << "; training = " << sampleInput.vtrain << std::endl;
                    
                    
                    for(int nf=0; nf<sampleInput.fids.size();nf ++){
                        
                        crid = sampleInput.crids[nf];
                        fid = sampleInput.fids[nf];
                        aux = sampleInput.auxs[nf];
                        aux2 = sampleInput.aux2s[nf];
                        mb = sampleInput.mbs[nf];
                        vpos = sampleInput.pos_of_sample_variable[nf];
                        funcid = sampleInput.funcids[nf];
                        weights = sampleInput.weights[nf];
                        
                        gradient = funcs_gradient[funcid](mb, aux, aux2, vpos, sampleInput.vvalue, sampleInput.vtrain, weights, sampleInput.stepSize);
                        
                    }

                }
                
                //mia::elly::utils::log() << "  | V" << sampleInput.vid << " <~~ " << targetValue << " from " << sampleInput.vvalue << std::endl;
                               
                return targetValue;
                
            }
            
        }
    }
}


#endif
