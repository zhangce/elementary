//
//  factor_register.h
//  elly
//
//  Created by Ce Zhang on 8/6/12.
//  Copyright (c) 2012 University of Wisconsin-Madison. All rights reserved.
//

#ifndef elly_factor_register_h
#define elly_factor_register_h

#include "factor_inits.h"

#include "../dstruct/StandardCorrelationRelation.h"
#include "../dstruct/IncrementalCorrelationRelation.h"
#include "../dstruct/AbstractCorrelationRelation.h"

typedef double (*FUNC_POTENTIAL)(void * , int, int, int);
typedef void (*FUNC_UPDATE)(void * , int , int , int );

FUNC_POTENTIAL funcs_potential[10];
FUNC_UPDATE    funcs_update[10];

bool           funcs_incremental[10];

namespace mia{
    namespace elly{
        namespace factors{
            
    
            void register_potentials(){
                funcs_potential[0] = potential_unigram;
                funcs_potential[1] = potential_bigram;
                funcs_potential[2] = potential_ldacount50;
                
                funcs_potential[3] = potential_ldacount50_samedoc;
                funcs_potential[4] = potential_ldacount50_sameword;
                funcs_potential[5] = potential_ldacount50_sametopic;
                
                
            }
            
            void register_updates(){
                funcs_update[0] = NULL;
                funcs_update[1] = NULL;
                funcs_update[2] = update_ldacount50;
                
                funcs_update[3] = update_ldacount50;
                funcs_update[4] = update_ldacount50;
                funcs_update[5] = update_ldacount50;
                
                funcs_incremental[0] = false;
                funcs_incremental[1] = false;
                funcs_incremental[2] = true;
                
                funcs_incremental[3] = true;
                funcs_incremental[4] = true;
                funcs_incremental[5] = true;
            }

            template<template<template<class C> class A, class B> class BUFFER>
            mia::elly::dstruct::AbstractCorrelationRelation * get_correlation_relation(int function_id){

                if(function_id == 0){
                    mia::elly::dstruct::StandardCorrelationRelation<BUFFER> * cr =
                    new mia::elly::dstruct::StandardCorrelationRelation<BUFFER>;
                    return cr;
                }
    
                if(function_id == 1){
                    mia::elly::dstruct::StandardCorrelationRelation<BUFFER> * cr =
                    new mia::elly::dstruct::StandardCorrelationRelation<BUFFER>;
                    return cr;
                }
    
                if(function_id == 2){
                    mia::elly::dstruct::IncrementalCorrelationRelation<BUFFER, factor_ldacount50> * cr =
                    new mia::elly::dstruct::IncrementalCorrelationRelation<BUFFER, factor_ldacount50>;
                    return cr;
                }
                
                if(function_id == 3){
                    mia::elly::dstruct::IncrementalCorrelationRelation<BUFFER, factor_ldacount50> * cr =
                    new mia::elly::dstruct::IncrementalCorrelationRelation<BUFFER, factor_ldacount50>;
                    return cr;
                }
                
                if(function_id == 4){
                    mia::elly::dstruct::IncrementalCorrelationRelation<BUFFER, factor_ldacount50> * cr =
                    new mia::elly::dstruct::IncrementalCorrelationRelation<BUFFER, factor_ldacount50>;
                    return cr;
                }
                
                if(function_id == 5){
                    mia::elly::dstruct::IncrementalCorrelationRelation<BUFFER, factor_ldacount50> * cr =
                    new mia::elly::dstruct::IncrementalCorrelationRelation<BUFFER, factor_ldacount50>;
                    return cr;
                }
        
                assert(false);
                return NULL;
    
}
            

            
            
            


        }
    }
}


#endif
