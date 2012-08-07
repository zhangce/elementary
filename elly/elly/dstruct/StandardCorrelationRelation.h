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

#include "../../../storageman/storageman/Buffer_mm.h"
#include "../../../storageman/storageman/KeyValue_fl.h"
#include "../../../storageman/storageman/KeyValue_vl.h"

#include "../factors/factor_inits.h"

#include "AbstractCorrelationRelation.h"



namespace mia{
    namespace elly{
        namespace dstruct{
            
            template<template<template<class C> class A, class B> class BUFFER>
            class StandardCorrelationRelation : public mia::elly::dstruct::AbstractCorrelationRelation{
                
            public:
                                
                mia::sm::KeyValue_vl<BUFFER> kv;
                
                void * lookup(int fid){
                        //todo: remember to delete
                        //todo: check whether it is slow
                    
                    mia::sm::IntsBlock rr = kv.get(fid);
                    
                    mia::sm::IntsBlock * ret = new mia::sm::IntsBlock;
                    *ret = rr;
                    
                    return ret;
                }
                
                void update(int fid, void (*func_update)(void * , int , int , int ), int vid, int from, int to){
                    assert(false);  // it is so weird that a StandardCorrelationRelation needs any update because nothing will change
                }
                                
                void prepare(){
                    
                    nfactor = 0;
                                        
                    if(filetype.compare("tsv") == 0){
                        
                        int fid, vid, aux, nvar;
                        std::ifstream fin( filename.c_str() );
                        
                        while(fin >> fid >> nvar){
                            
                            factor_vidblock_unigram fstate; //todo: change to a better name -- this is more general a class
                            
                            fin >> aux;
                            fstate.init_aux(aux);
                            
                            for(int i=0;i<nvar;i++){
                                fin >> vid;
                                fstate.init(vid);
                            }
                            
                            kv.set(fid, fstate.state);
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
