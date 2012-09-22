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

#include "../../../SMan/common/Common.h"

#include "../factors/factor_inits.h"


namespace mia{
    namespace elly{
        namespace dstruct{
            
            /**
             * \brief Variable assignment relation, which maps VID to its current assignment.
             *
             */
            template<hazy::sman::StorageType STORAGE>
            class VariableAssignmentRelation{
                
            public:
               
                /**
                 * __vf.tsv file's path, which contains the domain of each variable.
                 */
                std::string filename;
                
                /**
                 * Type of input file. {tsv}
                 */
                std::string filetype;
                                
                /**
                 * Key value store which maps VID to its assignment.
                 */
#ifdef BUFFER_DIRECT
              hazy::sman::ObjStore<int, STORAGE, hazy::sman::PROPERTY_NIL> kf;
              
              VariableAssignmentRelation() :
              kf(hazy::sman::ObjStore<int, STORAGE, hazy::sman::PROPERTY_NIL>()){}
#else
              hazy::sman::PagedBufferedObjStore<int, STORAGE, hazy::sman::PROPERTY_NIL> kf;
              
              VariableAssignmentRelation() :
              kf(hazy::sman::PagedBufferedObjStore<int, STORAGE, hazy::sman::PROPERTY_NIL>(COMMON_NBUFFER)){}
#endif
              
              
                
                /**
                 * Key value store which maps VID to its domain.
                 */
                hazy::sman::ObjStore<int, hazy::sman::STORAGE_MM, hazy::sman::PROPERTY_NIL> kf_domain;
                
                 /**
                 * Number of variables being loaded.
                 */
                int nvariable;
                
                /**
                 * Given a variable ID, get its current assignment.
                 */
                int lookup(int vid){
                    int rs;
                    kf.get(vid, rs);
                    return rs;
                }
                
                /**
                 * Given a variable ID, get its domain.
                 */
                int lookup_domain(int vid){
                    int rs;
                    kf_domain.get(vid, rs);
                    return rs;
                }
                
                /**
                 * Given a variable ID and a new value, update the key value store.
                 */
                void set(int vid, int value){
                    kf.set(vid, value);
                }
                
                /**
                 * Load file filename.
                 */
                void prepare(){
                                        
                    nvariable = 0;
                    
                    if(filetype.compare("tsv") == 0){
                        
                        int vid, nfactor, crid, fid, vdomain;
                        std::ifstream fin( filename.c_str() );
                        
                        while(fin >> vid >> vdomain >> nfactor){
                            
                            for(int i=0;i<nfactor;i++){
                                fin >> crid >> fid;
                            }
                            
                            kf.load(vid, 0);
                            kf_domain.load(vid, vdomain);
                            
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
