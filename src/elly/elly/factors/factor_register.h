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

typedef double (*FUNC_POTENTIAL)(void * , int, int, int, int, std::vector<double>* );

typedef double (*FUNC_UPPER)(void * , int, int, int, int, std::vector<double>* );

typedef double (*FUNC_LOWER)(void * , int, int, int, int, std::vector<double>* );

typedef void (*FUNC_UPDATE)(void * , int , int , int );

typedef double (*FUNC_GRADIENT)(void * , int , int , int, int , int, std::vector<double>*, double);

FUNC_POTENTIAL funcs_potential[10];

FUNC_UPPER funcs_upper[10];

FUNC_LOWER funcs_lower[10];

FUNC_UPDATE    funcs_update[10];

FUNC_GRADIENT  funcs_gradient[10];

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
                
                funcs_potential[6] = potential_mlnclause;
                
                funcs_upper[0] = NULL;
                funcs_upper[1] = NULL;
                funcs_upper[2] = NULL;
                
                funcs_upper[3] = NULL;
                funcs_upper[4] = NULL;
                funcs_upper[5] = potential_ldacount50_sametopic_upper;
                funcs_upper[6] = NULL;
                
                funcs_lower[0] = NULL;
                funcs_lower[1] = NULL;
                funcs_lower[2] = NULL;
                
                funcs_lower[3] = NULL;
                funcs_lower[4] = NULL;
                funcs_lower[5] = potential_ldacount50_sametopic_lower;
                funcs_lower[6] = NULL;
                
                
                funcs_gradient[0] = gradient_unigram;
                funcs_gradient[1] = gradient_zero;
                funcs_gradient[2] = gradient_zero;
                
                funcs_gradient[3] = gradient_zero;
                funcs_gradient[4] = gradient_zero;
                funcs_gradient[5] = gradient_zero;
                
                funcs_gradient[6] = gradient_mlnclause;
                
            }
            
            void register_updates(){
                funcs_update[0] = NULL;
                funcs_update[1] = NULL;
                funcs_update[2] = update_ldacount50;
                
                funcs_update[3] = update_ldacount50;
                funcs_update[4] = update_ldacount50;
                funcs_update[5] = update_ldacount50;
                
                funcs_update[6] = NULL;
                
                funcs_incremental[0] = false;
                funcs_incremental[1] = false;
                funcs_incremental[2] = true;
                
                funcs_incremental[3] = true;
                funcs_incremental[4] = true;
                funcs_incremental[5] = true;
                
                funcs_incremental[6] = false;
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
                
                if(function_id == 6){
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
