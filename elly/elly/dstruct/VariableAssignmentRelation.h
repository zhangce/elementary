//
//  VariableAssignmentRelation.h
//  elly
//
//  Created by Ce Zhang on 7/28/12.
//  Copyright (c) 2012 University of Wisconsin-Madison. All rights reserved.
//

#ifndef elly_VariableAssignmentRelation_h
#define elly_VariableAssignmentRelation_h


#include "../utils/Common.h"

#include <vector>

#include "../../../storageman/storageman/Buffer_mm.h"
#include "../../../storageman/storageman/KeyValue_fl.h"
#include "../../../storageman/storageman/KeyValue_vl.h"

#include "../factors/factor_inits.h"


namespace mia{
    namespace elly{
        namespace dstruct{
            
            class VariableAssignmentRelation{
                
            public:
                
                std::string filename;
                std::string filetype;
                
                mia::sm::KeyValue_fl<mia::sm::Buffer_mm, int> kf;
                mia::sm::KeyValue_fl<mia::sm::Buffer_mm, int> kf_domain;
                
                int nvariable;
                
                int lookup(int vid){
                    return kf.get(vid);
                }
                
                int lookup_domain(int vid){
                    return kf_domain.get(vid);
                }
                
                void set(int vid, int value){
                    kf.set(vid, value);
                }
                
                void prepare(){
                                        
                    nvariable = 0;
                    
                    if(filetype.compare("tsv") == 0){
                        
                        int vid, nfactor, crid, fid, vdomain;
                        std::ifstream fin( filename.c_str() );
                        
                        while(fin >> vid >> vdomain >> nfactor){
                            
                            for(int i=0;i<nfactor;i++){
                                fin >> crid >> fid;
                            }
                            
                            kf.set(vid, 0);
                            kf_domain.set(vid, vdomain);
                            
                            nvariable ++;
                            
                        }
                    }    
                    
                    mia::elly::utils::log() << "    + # variables = " << nvariable << std::endl;
                    
                     /*  //test
                     for(int vid=0;vid < nvariable; vid++){
                     
                         int initvalue = kv.get(vid);
                         std::cout << vid << "~>" << initvalue << std::endl;
                     
                     
                     }*/
                    
                }
                
            };
            
            
            
        }
    }
}



#endif
