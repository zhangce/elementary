//
//  InFileVariableFactorRelation.h
//  elly
//
//  Created by Ce Zhang on 7/27/12.
//  Copyright (c) 2012 University of Wisconsin-Madison. All rights reserved.
//

#ifndef elly_InFileVariableFactorRelation_h
#define elly_InFileVariableFactorRelation_h


#include "../utils/Common.h"

#include <vector>

#include "../../../storageman/storageman/Buffer_mm.h"
#include "../../../storageman/storageman/KeyValue_fl.h"
#include "../../../storageman/storageman/KeyValue_vl.h"

#include "../factors/factor_inits.h"



namespace mia{
    namespace elly{
        namespace dstruct{
            
            class VariableFactorRelation{
                
            public:
                
                std::string filename;
                std::string filetype;
                
                mia::sm::KeyValue_vl<mia::sm::Buffer_mm> kv;
                
                int nvariable;
                
                
                mia::sm::IntsBlock lookup(int vid){
                    return kv.get(vid);
                }
                
                
                void prepare(){
                
                    nvariable = 0;
                    
                    if(filetype.compare("tsv") == 0){
                        
                        int vid, nfactor, crid, fid, vdomain;
                        std::ifstream fin( filename.c_str() );
                        
                        while(fin >> vid >> vdomain >> nfactor){
                            
                            
                            mia::sm::IntsBlock factors;
                            factors.append<int>(nfactor);
                            
                            for(int i=0;i<nfactor;i++){
                                
                                fin >> crid >> fid;
                                
                                factors.append<int>(crid);
                                factors.append<int>(fid);
                                
                            }
                            
                            kv.set(vid, factors);
                            nvariable ++;
                            
                        }
                        
                        mia::elly::utils::log() << "    + # variables = " << nvariable << std::endl;
                        
                    }    
                    
                    /*  //test
                    for(int vid=0;vid < nvariable; vid++){
                        
                        mia::sm::IntsBlock block
                            = kv.get(vid);
                        
                        std::cout <<
                            "vid = " << vid << ": " <<
                            "nfactor = " << block.get<int>(0) << std::endl;
                        
                        for(int fid=1;fid<block.size;fid+=2){
                            std:: cout << "     frrelation = " << 
                                        block.get<int>(fid) << "; "
                            << "factorid = " << block.get<int>(fid+1)
                            << std::endl;
                        }
                        
                    } */
                    
                }
                                    
            };
            
            
            
        }
    }
}


#endif
