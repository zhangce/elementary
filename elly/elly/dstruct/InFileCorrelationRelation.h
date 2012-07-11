//
//  InFileCorrelationRelation.h
//  elly
//
//  Created by Ce Zhang on 7/10/12.
//  Copyright (c) 2012 University of Wisconsin-Madison. All rights reserved.
//

#ifndef elly_InFileCorrelationRelation_h
#define elly_InFileCorrelationRelation_h

#include "../utils/Common.h"

namespace mia{
    namespace elly{
        namespace dstruct{
            
            class InFileCorrelationRelation{
                
            public:
                
                std::string factor_name;
                int function_id;
                std::string filename;
                std::string filetype;
                
                
                void prepare_init(){
                    
                    
                    
                }
                
                void prepare_line(int fid, int vid, int pos, int aux){
                    
                    //std::cout << fid << vid << pos << aux << std::endl;
                    
                    
                    
                }
                
                
                void prepare(){
                
                    prepare_init();
                    
                    if(filetype.compare("tsv") == 0){
                        
                        int fid, vid, pos, aux;
                        std::ifstream fin( filename.c_str() );
                        
                        while(fin >> fid >> vid >> pos >> aux){
                            prepare_line(fid, vid, pos, aux);
                        }
                        
                    }
                    
                    
                    
                }
                
            };

        
            
            
            
            
        }
    }
}

#endif
