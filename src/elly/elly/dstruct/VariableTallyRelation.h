//
//  VariableTallyRelation.h
//  elly
//
//  Created by Ce Zhang on 8/7/12.
//  Copyright (c) 2012 University of Wisconsin-Madison. All rights reserved.
//

#ifndef elly_VariableTallyRelation_h
#define elly_VariableTallyRelation_h


namespace mia{
    namespace elly{
        namespace dstruct{
            
            /**
             * \brief Variable tally relation which maps a variable to the int block contains tally info.
             */
            class VariableTallyRelation{
                
            public:
                
                std::string filename;
                std::string filetype;
                
                hazy::sman::ObjStore<mia::sm::IntsBlock, hazy::sman::STORAGE_MM, hazy::sman::PROPERTY_NIL> kv;
                
                //mia::sm::KeyValue_vl<mia::sm::Buffer_mm> kv;
                
                int nvariable;
                
                mia::sm::IntsBlock lookup(int vid){
                    mia::sm::IntsBlock rs;
                    kv.get(vid, rs);
                    return rs;
                }
                
                void tally(int vid, int value){
                    
                    //std::cout << "tally " << vid << " <~" << value << std::endl;
                    
                    mia::sm::IntsBlock block;
                    kv.get(vid, block);
                    block.content[value] ++;
                    kv.set(vid, block);
                }
                                
                void prepare(){
                    
                    nvariable = 0;
                    
                    if(filetype.compare("tsv") == 0){
                        
                        int vid, nfactor, crid, fid, vdomain;
                        std::ifstream fin( filename.c_str() );
                        
                        while(fin >> vid >> vdomain >> nfactor){
                            
                            mia::sm::IntsBlock tallies;
                            for(int i=0;i<vdomain;i++){
                                tallies.append<int>(0);
                            }
                            kv.load(vid, tallies);
                            
                            
                            for(int i=0;i<nfactor;i++){
                                
                                fin >> crid >> fid;
                                
                            }
                            
                            
                            nvariable ++;
                            
                        }
                        
                        mia::elly::utils::log() << "    + # variables = " << nvariable << std::endl;
                        
                    }
                                       
                }
                
            };
            
            
            
        }
    }
}


#endif
