//
//  factor_inits.h
//  elly
//
//  Created by Ce Zhang on 7/26/12.
//  Copyright (c) 2012 University of Wisconsin-Madison. All rights reserved.
//

#ifndef elly_factor_inits_h
#define elly_factor_inits_h


#include "../../../storageman/storageman/KeyValue_fl.h"
#include "../../../storageman/storageman/KeyValue_vl.h"

#include "../dstruct/VariableAssignmentRelation.h"

#include "../utils/Common.h"



class factor_ldacount50{

public:
    int counts[50];
    
    void init_aux2(int aux2){
    }
    
    void init_aux(int aux){
        for(int i=0;i<50;i++){
            counts[i] = 0;
        }
    }
    
    void init(int vid){
        counts[0] ++;
    }
    
};


class factor_vidblock_unigram{
  
public:
    mia::sm::IntsBlock state;

    
    
    void init_aux(int aux){
        state.append<int>(aux);
    }
    
    void init_aux2(int aux2){
        state.append<int>(aux2);
    }
    
    void init(int vid){
        state.append<int>(vid);
    }
        
};


double potential_mlnclause(void * mb, int aux, int aux2, int vpos, int value, std::vector<double>* weights){
    
    std::vector<int>* pmb = (std::vector<int>*)mb;
    
    //bool isNegative = ( (aux2 & (1 << vpos)) != 0);
    //std::cout << aux2 << " pos " << vpos << "; is negative = " << isNegative << std::endl;
    
    //std::cout << "vpos = " << vpos << "; weight = " << weights->at(aux) << "; sign = " << aux2 << "; #var = " << pmb->size() << std::endl;
    
    bool isTrue = false;
    bool isNegative;
    int cvalue = 0;
    for(int i=0;i<pmb->size();i++){
        
        cvalue = pmb->at(i);
        if(i == vpos){
            cvalue = value;
        }
        
        isNegative = ( (aux2 & (1 << i)) != 0);
        if( (cvalue==1 && isNegative==false) || (cvalue==0 && isNegative==true) ){
            isTrue = true;
            break;
        }
           
    }
    
    if(isTrue){
        return weights->at(aux);
    }else{
        return 0;
    }
    
    return 0;
}



double gradient_mlnclause(void * mb, int aux, int aux2, int vpos, int value, int vtrain, std::vector<double>* weights, double step){
    
    std::vector<int>* pmb = (std::vector<int>*)mb;
    
    //bool isNegative = ( (aux2 & (1 << vpos)) != 0);
    //std::cout << aux2 << " pos " << vpos << "; is negative = " << isNegative << std::endl;
    
    //std::cout << "vpos = " << vpos << "; weight = " << weights->at(aux) << "; sign = " << aux2 << "; #var = " << pmb->size() << std::endl;
    
    bool isTrue = false;
    bool isTrue_train = false;
    bool isNegative;
    int cvalue = 0;
    for(int i=0;i<pmb->size();i++){
        
        cvalue = pmb->at(i);
        if(i == vpos){
            cvalue = value;
        }
        
        isNegative = ( (aux2 & (1 << i)) != 0);
        if( (cvalue==1 && isNegative==false) || (cvalue==0 && isNegative==true) ){
            isTrue = true;
            break;
        }
        
    }
    
    for(int i=0;i<pmb->size();i++){
        
        cvalue = pmb->at(i);
        if(i == vpos){
            cvalue = vtrain;
        }
        
        isNegative = ( (aux2 & (1 << i)) != 0);
        if( (cvalue==1 && isNegative==false) || (cvalue==0 && isNegative==true) ){
            isTrue_train = true;
            break;
        }
        
    }
    
    double gradient = isTrue_train - isTrue;    //TODO: double check
    
    
    
    //if(isTrue_train != isTrue){
    //    std::cout << isTrue << ", train " << isTrue_train << ", gradient = " << gradient << std::endl;
    //}
        
    weights->at(aux) = weights->at(aux) + step * gradient;
    
    return vtrain - value;
}






double potential_ldacount50(void * state, int aux, int aux2, int vpos, int value, std::vector<double>* weights){
    factor_ldacount50 * pstate = (factor_ldacount50*) state;
    return log(pstate->counts[value]+1);
}


double potential_ldacount50_samedoc(void * state, int aux, int aux2, int vpos, int value, std::vector<double>* weights){
    factor_ldacount50 * pstate = (factor_ldacount50*) state;
    return 1.0 + pstate->counts[value];
    
    //std::cout << pstate->counts[value] << std::endl;
    
    //return +log(1.0 + pstate->counts[value]);
}

double potential_ldacount50_sametopic(void * state, int aux, int aux2, int vpos, int value, std::vector<double>* weights){
    factor_ldacount50 * pstate = (factor_ldacount50*) state;
    return 1.0/(1.0 + pstate->counts[value]);
    
    //std::cout << pstate->counts[value] << std::endl;
    
    //return -log(1.0 + pstate->counts[value]);
}

double potential_ldacount50_sameword(void * state, int aux, int aux2, int vpos, int value, std::vector<double>* weights){
    factor_ldacount50 * pstate = (factor_ldacount50*) state;
    
    //std::cout << pstate->counts[value] << std::endl;
    return 0.1 + pstate->counts[value];
    //return +log(0.1 + pstate->counts[value]);
}


void update_ldacount50(void * state, int vpos, int from, int to){
    factor_ldacount50 * pstate = (factor_ldacount50*) state;
    
    if(from == to){
        return;
    }
    
    pstate->counts[from] = pstate->counts[from] - 1;
    pstate->counts[to] = pstate->counts[to] + 1;
    
}


double potential_unigram(void * mb, int aux, int aux2, int vpos, int value, std::vector<double>* weights){
    
    std::vector<int>* pmb = (std::vector<int>*)mb;
    
    return weights->at(aux) * value;
    
}

double gradient_zero(void * mb, int aux, int aux2, int vpos, int value, int vtrain, std::vector<double>* weights, double step){
    return 0;
}

double gradient_unigram(void * mb, int aux, int aux2, int vpos, int value, int vtrain, std::vector<double>* weights, double step){
    
    double gradient = vtrain - value;
    
    //std::cout << "grad for aux " << aux << " is " << gradient << " (currently) " << weights->at(aux) << " step = " << step << std::endl;
    
    weights->at(aux) = weights->at(aux) + step * gradient;
    
    return vtrain - value;
}

double potential_bigram(void * mb, int aux, int aux2, int vpos, int value, std::vector<double>* weights){
    
    /*
    std::vector<int>* pmb = (std::vector<int>*)mb;
    
    //std::cout << "vpos = " << vpos << std::endl;
    
    if(vpos == 0){
        //std::cout << "pmb->at(1) = " << pmb->at(1) << std::endl;
        return 1.0 * value * pmb->at(1) * aux;
    }
    
    if(vpos == 1){
        //std::cout << "pmb->at(0) = " << pmb->at(0) << std::endl;
        return 1.0 * pmb->at(0) * value * aux;
    }
    */
    return 0;
    assert(false);
    return 0;
    
}


/*
void potential_unigram(void * _state, mia::elly::dstruct::VariableAssignmentRelation & va){
    
    mia::sm::IntsBlock* state = &(((factor_vidblock_unigram*) _state)->state);
    std::cout << state->content[0] << std::endl;
    std::cout << state->content[1] << std::endl;

}
*/

#endif






