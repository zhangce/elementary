//
//  IncrementalCorrelationRelation.h
//  elly
//
//  Created by Ce Zhang on 8/6/12.
//  Copyright (c) 2012 University of Wisconsin-Madison. All rights reserved.
//

#ifndef elly_IncrementalCorrelationRelation_h
#define elly_IncrementalCorrelationRelation_h


#include "../utils/Common.h"

#include <vector>

#include "../../../SMan/common/Include.h"

#include "../factors/factor_inits.h"

#include "AbstractCorrelationRelation.h"


namespace mia{
    namespace elly{
        namespace dstruct{
            
            /**
             * \brief Correlation relation in which each factor contains a fixed-size state.
             *
             * \tparam BUFFER Buffer to use for this correlation relation.
             *
             */
            template<hazy::sman::StorageType STORAGE, class TYPE>
            class IncrementalCorrelationRelation : public mia::elly::dstruct::AbstractCorrelationRelation{
                
            public:
                
                //mia::sm::KeyValue_fl<BUFFER, TYPE> kv;
                
                /**
                 * In-memory key value store that maps factor ID to its state.
                 */
              

                //hazy::sman::ObjStore<TYPE, STORAGE, hazy::sman::JAVAHASH_ACCU> kv;

#ifdef BUFFER_DIRECT
              hazy::sman::ObjStore<TYPE, STORAGE, hazy::sman::JAVAHASH_ACCU> kv;
#else
              hazy::sman::PagedBufferedObjStore<TYPE, STORAGE, hazy::sman::JAVAHASH_ACCU> kv;
#endif
              
              //hazy::sman::ObjStore<TYPE, STORAGE, hazy::sman::PROPERTY_NIL> kv;
              
              //IncrementalCorrelationRelation() : kv(hazy::sman::BufferedObjStore<TYPE, STORAGE, hazy::sman::JAVAHASH_ACCU>(100000))
              
              ~IncrementalCorrelationRelation(){
                
              }
              
              void set(int fid, void * ptr){}
              
              void print_status(){
                std::cout << "NFLUSH = " << kv.pagestore.nflush << std::endl;
              }
              
#ifdef BUFFER_DIRECT
              IncrementalCorrelationRelation() : kv(hazy::sman::ObjStore<TYPE, STORAGE, hazy::sman::JAVAHASH_ACCU>())
#else
              IncrementalCorrelationRelation() : kv(hazy::sman::PagedBufferedObjStore<TYPE, STORAGE, hazy::sman::JAVAHASH_ACCU>(COMMON_NBUFFER))
#endif
              
              
              //IncrementalCorrelationRelation() : kv(hazy::sman::ObjStore<TYPE, STORAGE, hazy::sman::PROPERTY_NIL>())
              {}
              
                /**
                 * given a factor ID, return the pointer to its state.
                 */
                void * lookup(int fid){
                    //todo: remember to delete
                    //todo: check whether it is slow
                    
                    //std::cout << "lookup" << fid << std::endl;
                    
                    TYPE rr;
                    kv.get(fid, rr);
                    TYPE * ret = new TYPE;
                    *ret = rr;
                    
                    return ret;
                }
                
                /**
                 * Update the state given new assignment of variables.
                 *
                 * \param fid factor ID
                 * \param func_update function pointer to update function
                 * \param vid variable ID to be updated
                 * \param 
                 *
                 */
                void update(int fid, void (*func_update)(void * , int , int , int ), int vid, int from, int to, bool _lock){
                    
                    if(func_update == NULL){
                        return;
                    }
                    
                    //lock(fid);
                    //kv.adhoc_update(fid, from, to); //TODO ad hoc.
                    //return;
                    //release(fid);
                    
                    //if(_lock){
                    //    lock(fid);
                    //}
                    
                    TYPE rr;
                    kv.get(fid, rr);
                    //std::cout << "~" << rr.counts[from] << std::endl;
                    func_update(&rr, vid, from, to);
                    kv.set(fid, rr);
                    //std::cout << "*" << rr.counts[from] << std::endl;
                    
                    //if(_lock){
                    //    release(fid);
                    //}
                
                }
                
                
                void prepare(){
                    
                    nfactor = 0;
                    
                    if(filetype.compare("tsv") == 0){
                        
                        int fid, vid, aux, aux2, nvar;
                        std::ifstream fin( filename.c_str() );
                        
                        while(fin >> fid >> nvar){
                            
                            TYPE fstate; //todo: change to a better name -- this is more general a class
                            
                            fin >> aux;
                            fstate.init_aux(aux);
                            
                            fin >> aux2;
                            fstate.init_aux2(aux2);
                            
                            for(int i=0;i<nvar;i++){
                                fin >> vid;
                                fstate.init(vid);
                            }
                            
                            kv.load(fid, fstate);
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
