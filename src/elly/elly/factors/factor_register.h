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

/**
 * Function pointer of potential function.
 *
 * \param pointer to state info
 * \param weight index
 * \param aux integer info
 * \param variable position to flip
 * \param tentative assignment of variable
 * \param pointer to weight vector.
 *
 */
typedef double (*FUNC_POTENTIAL)(void * , int, int, int, int, std::vector<double>* );

typedef double (*FUNC_UPPER)(void * , int, int, int, int, std::vector<double>* );

typedef double (*FUNC_LOWER)(void * , int, int, int, int, std::vector<double>* );

/**
 * Function pointer of update function.
 *
 * \param pointer to state info
 * \param variable position to flip
 * \param old assignment of variable
 * \param new assignment of variable to be updated
 *
 */
typedef void (*FUNC_UPDATE)(void * , int , int , int );

/**
 * Function pointer of potential function.
 *
 * \param pointer to state info
 * \param weight index
 * \param aux integer info
 * \param variable position to flip
 * \param new assignment of variable
 * \param training assignment of variable
 * \param pointer to weight vector.
 * \param step size
 *
 */
typedef double (*FUNC_GRADIENT)(void * , int , int , int, int , int, std::vector<double>*, double);

/**
 * Array of potential functions.
 *
 * \sa FUNC_POTENTIAL;
 */
FUNC_POTENTIAL funcs_potential[10];

FUNC_UPPER funcs_upper[10];

FUNC_LOWER funcs_lower[10];

/**
 * Array of update functions.
 *
 * \sa FUNC_UPDATE;
 */
FUNC_UPDATE    funcs_update[10];

/**
 * Array of gradient functions.
 *
 * \sa FUNC_GRADIENT;
 */
FUNC_GRADIENT  funcs_gradient[10];

bool           funcs_incremental[10];

namespace mia{
    namespace elly{
        
        /**
         * \brief Namespace for registered factors.
         */
        namespace factors{
            
            /**
             * Build the mapping from factorID to potential/gradient functions.
             *
             * \sa FUNC_POTENTIAL
             * \sa FUNC_UPDATE
             */
            void register_potentials(){
                funcs_potential[0] = potential_unigram;
                funcs_potential[1] = potential_bigram;
                funcs_potential[2] = potential_ldacount50;
                
                funcs_potential[3] = potential_ldacount50_samedoc;
                funcs_potential[4] = potential_ldacount50_sameword;
                funcs_potential[5] = potential_ldacount50_sametopic;
                
                funcs_potential[6] = potential_mlnclause;
                funcs_potential[7] = potential_lr_unigram;
                
                funcs_upper[0] = NULL;
                funcs_upper[1] = NULL;
                funcs_upper[2] = NULL;
                
                funcs_upper[3] = NULL;
                funcs_upper[4] = NULL;
                funcs_upper[5] = potential_ldacount50_sametopic_upper;
                funcs_upper[6] = NULL;
                funcs_upper[7] = NULL;
              
                funcs_lower[0] = NULL;
                funcs_lower[1] = NULL;
                funcs_lower[2] = NULL;
                
                funcs_lower[3] = NULL;
                funcs_lower[4] = NULL;
                funcs_lower[5] = potential_ldacount50_sametopic_lower;
                funcs_lower[6] = NULL;
                funcs_lower[7] = NULL;
              
                
                funcs_gradient[0] = gradient_unigram;
                funcs_gradient[1] = gradient_zero;
                funcs_gradient[2] = gradient_zero;
                
                funcs_gradient[3] = gradient_zero;
                funcs_gradient[4] = gradient_zero;
                funcs_gradient[5] = gradient_zero;
                
                funcs_gradient[6] = gradient_mlnclause;
                funcs_gradient[6] = gradient_zero;
            }
            
            /**
             * Build the mapping from factorID to update functions.
             *
             * \sa FUNC_UPDATE
             */
            void register_updates(){
                funcs_update[0] = NULL;
                funcs_update[1] = NULL;
                funcs_update[2] = update_ldacount50;
                
                funcs_update[3] = update_ldacount50;
                funcs_update[4] = update_ldacount50;
                funcs_update[5] = update_ldacount50;
                funcs_update[6] = NULL;
                funcs_update[7] = NULL;
                
                funcs_incremental[0] = false;
                funcs_incremental[1] = false;
                funcs_incremental[2] = true;
                
                funcs_incremental[3] = true;
                funcs_incremental[4] = true;
                funcs_incremental[5] = true;
                
                funcs_incremental[6] = false;
              
                funcs_incremental[7] = false;
            }

            /**
             * Given a factorID, return a correlation relation, either mia::elly::dstruct::StandardCorrelationRelation or mia::elly::dstruct::IncrementalCorrelationRelation.
             *
             * \tparam BUFFER Buffer to use for this correlation relation.
             *
             * \param function_id Factor ID.
             * 
             * \return pointer to the new correlation relation (sub class of mia::elly::dstruct::AbstractCorrelationRelation)
             */
            template<hazy::sman::StorageType STORAGE>
            mia::elly::dstruct::AbstractCorrelationRelation * get_correlation_relation(int function_id){

                if(function_id == 0){
                    mia::elly::dstruct::StandardCorrelationRelation<STORAGE> * cr =
                    new mia::elly::dstruct::StandardCorrelationRelation<STORAGE>;
                    return cr;
                }
    
                if(function_id == 1){
                    mia::elly::dstruct::StandardCorrelationRelation<STORAGE> * cr =
                    new mia::elly::dstruct::StandardCorrelationRelation<STORAGE>;
                    return cr;
                }
                
                if(function_id == 6){
                    mia::elly::dstruct::StandardCorrelationRelation<STORAGE> * cr =
                    new mia::elly::dstruct::StandardCorrelationRelation<STORAGE>;
                    return cr;
                }
              
                if(function_id == 7){
                  mia::elly::dstruct::StandardCorrelationRelation<STORAGE> * cr =
                  new mia::elly::dstruct::StandardCorrelationRelation<STORAGE>;
                  return cr;
                }
    
                if(function_id == 2){
                    mia::elly::dstruct::IncrementalCorrelationRelation<STORAGE, factor_ldacount50> * cr =
                    new mia::elly::dstruct::IncrementalCorrelationRelation<STORAGE, factor_ldacount50>;
                    return cr;
                }
                
                if(function_id == 3){
                    mia::elly::dstruct::IncrementalCorrelationRelation<STORAGE, factor_ldacount50> * cr =
                    new mia::elly::dstruct::IncrementalCorrelationRelation<STORAGE, factor_ldacount50>;
                    return cr;
                }
                
                if(function_id == 4){
                    mia::elly::dstruct::IncrementalCorrelationRelation<STORAGE, factor_ldacount50> * cr =
                    new mia::elly::dstruct::IncrementalCorrelationRelation<STORAGE, factor_ldacount50>;
                    return cr;
                }
                
                if(function_id == 5){
                    mia::elly::dstruct::IncrementalCorrelationRelation<STORAGE, factor_ldacount50> * cr =
                    new mia::elly::dstruct::IncrementalCorrelationRelation<STORAGE, factor_ldacount50>;
                    return cr;
                }
        
                assert(false);
                return NULL;
    
}
            

            
            
            


        }
    }
}


#endif
