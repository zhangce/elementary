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

#include "../../../sman/sman/Include.h"

#include "../factors/factor_inits.h"

namespace mia{
    namespace elly{
        namespace dstruct{
            
            /**
             * \brief Variable factor relation which maps each variable to the set of factors.
             */
            class VariableFactorRelation{
                
            public:
                
                /**
                 * __vf file's path.
                 */
                std::string filename;
                
                /**
                 * type of input file. {tsv}
                 */
                std::string filetype;
                
                
                /**
                 * Key value store that maps VID to factor block.
                 */
                mia::sm::KV<int, mia::sm::IntsBlock, mia::sm::MMAP, mia::sm::DIRECT, mia::sm::NIL, mia::sm::DENSE_KEY> kv;

                /**
                 * Number of vairables being loaded.
                 */
                int nvariable;
                
                /**
                 * Given a VID, get factor block.
                 */
                mia::sm::IntsBlock lookup(int vid){
                    mia::sm::IntsBlock block;
                    kv.get(vid, block);
                    return block;
                }
                
                /**
                 * Load from file filename;
                 */
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
                            
                            kv.load(vid, factors);
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
