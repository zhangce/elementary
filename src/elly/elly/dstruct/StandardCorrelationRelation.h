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

#include <vector>

#include "../../../sman/sman/Include.h"

#include "../factors/factor_inits.h"

#include "AbstractCorrelationRelation.h"



namespace mia{
    namespace elly{
        
        /**
         * Namespace for relations.
         */
        namespace dstruct{
            
            /**
             * Correlation relation in which each factor contains a list of variable IDs
             *
             * \tparam BUFFER Buffer to use for this correlation relation.
             *
             */
            template<mia::sm::KV_Storage STORAGE>
            class StandardCorrelationRelation : public mia::elly::dstruct::AbstractCorrelationRelation{
                
            public:
                         
                /**
                 * Key value store that maps factorID to block of VIDs. 
                 *
                 * \sa  mia::sm::KeyValue_vl
                 */
                mia::sm::KV<int, mia::sm::IntsBlock, STORAGE, mia::sm::DIRECT, mia::sm::NIL, mia::sm::DENSE_KEY> kv;
                
                
                /**
                 * Given a factor state ID, returns the pointer to VID block.
                 *
                 * \param fid factor state ID.
                 *
                 * \return pointer to VID block.
                 */
                void * lookup(int fid){
                        //todo: remember to delete
                        //todo: check whether it is slow
                    
                    mia::sm::IntsBlock rr;
                    kv.get(fid, rr);
                    
                    mia::sm::IntsBlock * ret = new mia::sm::IntsBlock;
                    //kv.get(fid, *ret);
                    *ret = rr;
                    
                    return ret;
                }
                
                /**
                 * Update factor state -- empty for mia::elly::dstruct::StandardCorrelationRelation. 
                 *
                 * \sa mia::elly::dstruct::IncrementalCorrelationRelation.
                 */
                void update(int fid, void (*func_update)(void * , int , int , int ), int vid, int from, int to, bool lock){
                    assert(false);  // it is so weird that a StandardCorrelationRelation needs any update because nothing will change
                }
                 
                /**
                 * Load factor from file mia::elly::dstruct::AbstractCorrelationRelation::filename. This function will fill in mia::elly::dstruct::StandardCorrelationRelation::kv.
                 */
                void prepare(){
                    
                    nfactor = 0;
                                        
                    if(filetype.compare("tsv") == 0){
                        
                        int fid, vid, aux, aux2, nvar;
                        std::ifstream fin( filename.c_str() );
                        
                        while(fin >> fid >> nvar){
                            
                            factor_vidblock_unigram fstate; //todo: change to a better name -- this is more general a class
                            
                            
                            fin >> aux;
                            fstate.init_aux(aux);
                            
                            fin >> aux2;
                            fstate.init_aux2(aux2);
                            
                            for(int i=0;i<nvar;i++){
                                fin >> vid;
                                fstate.init(vid);
                            }
                            
                            kv.load(fid, fstate.state);
                            nfactor ++;
                            
                            pthread_mutex_t * sem = new pthread_mutex_t;
                            pthread_mutex_init(sem, NULL);
                            sems.push_back(sem);
                            
                        }
                        
                    }
                    
                    mia::elly::utils::log() << "    + # factors = " << nfactor << std::endl;
                    
                }
                
            };

        
            
            
            
            
        }
    }
}

#endif
