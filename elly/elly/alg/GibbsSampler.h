//
//  GibbsSampler.h
//  elly
//
//  Created by Ce Zhang on 7/28/12.
//  Copyright (c) 2012 University of Wisconsin-Madison. All rights reserved.
//

#ifndef elly_GibbsSampler_h
#define elly_GibbsSampler_h

#include "../utils/Common.h"
#include "../factors/factor_inits.h"

namespace mia {
    namespace elly{
        namespace alg{
            
            int Shuffle(mia::elly::SampleInput & sampleInput){
                
                //mia::elly::utils::log() << ">>> Suffle variable ID=" << sampleInput.vid << std::endl;
                
                int targetValue = rand() % sampleInput.vdomain;
                
                //mia::elly::utils::log() << "  | V" << sampleInput.vid << " <~~ " << targetValue << " from " << sampleInput.vvalue << std::endl;

                return targetValue;
                  
            }
            
            int GibbsSampling(mia::elly::SampleInput & sampleInput){
                
                //mia::elly::utils::log() << ">>> Sample variable ID=" << sampleInput.vid << std::endl;
                
                // for each factor
                int crid, fid, aux, vpos, funcid, aux2;
                double potential;
                void* mb;
                std::vector<double>* weights;
                
                std::vector<double> potentials;
                for(int i=0;i<sampleInput.vdomain;i++){
                    potentials.push_back(0);
                    //potentials.push_back(1);
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
                    
                    for(int value=0; value < sampleInput.vdomain; value ++){

                        potential = funcs_potential[funcid](mb, aux, aux2, vpos, value, weights);

                        //std::cout << "funcid = " << funcid << ", vpos = " << vpos << "; value = " << value << ": " << potential << std::endl;
                        
                        potentials[value] += potential;
                        //potentials[value] *= potential;
                    }
                    
                }
                
                double pfunc = -10000000;;
                //double pfunc = 0;
                for(int value=0; value < sampleInput.vdomain; value ++){
                    
                    pfunc = mia::elly::utils::logadd(potentials[value], pfunc);
                    //pfunc += potentials[value];
                }
                
                //mia::elly::utils::log() << "  | log potential func = " << pfunc << std::endl;
                
                
                double random = drand48();
                //mia::elly::utils::log() << "  | random = " << random << std::endl;
                
                double accum = 0;
                double pmeta = 0;
                int targetValue = -1;
                for(int value=0; value < sampleInput.vdomain; value ++){
                    
                    pmeta = exp( potentials[value] - pfunc );
                    //pmeta = potentials[value]/pfunc;
                    
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
                
                sampleInput.log_improve_ratio = potentials[targetValue] - potentials[sampleInput.vvalue];
                
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
