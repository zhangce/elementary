//
//  Materialization_full.h
//  elly
//
//  Created by Ce Zhang on 9/15/12.
//  Copyright (c) 2012 University of Wisconsin-Madison. All rights reserved.
//

#ifndef elly_Materialization_fcoc_h
#define elly_Materialization_fcoc_h

#include "../dstruct/StandardCorrelationRelation.h"

#include "Materialization.h"

namespace mia{
  namespace elly{
    
    namespace mat{
      
      template<hazy::sman::StorageType STORAGE>
      class Materialization_fcoc : public mia::elly::mat::Materialization {
        
      public:
        
        mia::elly::utils::FactorFileParser<STORAGE> * parserrs;
        
        Materialization_fcoc(mia::elly::utils::FactorFileParser<STORAGE> * _parserrs){
          
          parserrs = _parserrs;
          
        }
        
        void * get_parserrs(){
          return parserrs;
        }
        
        mia::sm::IntsBlock vt_lookup(int64_t vid){
          return parserrs->vt.lookup(vid);
        }
        
        std::vector<mia::elly::dstruct::AbstractCorrelationRelation* > mat_crs;
        
        
        void materialize(){
          
          mia::elly::utils::log() << ">> Materializing for F-COC...." << std::endl;
          
            //std::vector<mia::elly::dstruct::AbstractCorrelationRelation* > crs;
          for(int crid=0; crid<parserrs->crs.size(); crid++){
              
            if(funcs_incremental[parserrs->crs[crid]->function_id] == false){
              //is standard_correlation_relation
            
              mia::elly::dstruct::StandardCorrelationRelation<STORAGE> * oscr = (mia::elly::dstruct::StandardCorrelationRelation<STORAGE> * ) parserrs->crs[crid];
              
              mia::elly::dstruct::StandardCorrelationRelation<STORAGE> * scr =
              new mia::elly::dstruct::StandardCorrelationRelation<STORAGE>();
              
              mat_crs.push_back(scr);
              
              int aux, aux2, vid, nvariable, vvalue;
              
              scr->nfactor = oscr->nfactor;
              
              for(int fid=0;fid<oscr->nfactor;fid++){
                
                mia::sm::IntsBlock * intblock =  (mia::sm::IntsBlock*) oscr->lookup(fid);
                factor_vidblock_unigram fstate;
                
                aux = intblock->content[0];
                aux2 = intblock->content[1];
                
                
                // state: aux, aux2, nvariable, <vid, value>...
                fstate.init_aux(aux);
                fstate.init_aux2(aux2);
                
                nvariable = intblock->size - 2;
                //fstate.init(nvariable);
                
                
                for(int nvid = 2; nvid < intblock->size; nvid ++){
                  vid = intblock->content[2];
                  vvalue = parserrs->va.lookup(vid);
                  
                  fstate.init(vid);
                  fstate.init(vvalue);
                  
                }
                
                scr->kv.load(fid, fstate.state);
                
                delete intblock;
                
              }
              
            }else{
              mat_crs.push_back(parserrs->crs[crid]);
            }
            
          }
          
        }
        
        int getNVariable(){
          return parserrs->va.nvariable;
        }
        
        
        int va_lookup(int64_t vid){
          
          return parserrs->va.lookup(vid);
        }
        
        int getNCRS(){
          return parserrs->crs.size();
        }
        
        mia::elly::dstruct::AbstractCorrelationRelation * getCRS(int ncrs){
          return parserrs->crs[ncrs];
        }
        
        
        void update(mia::elly::SampleInput & sampleInput, int newvalue, bool tally = false, bool lock = true){
          
          parserrs->va.set(sampleInput.vid, newvalue);
          
          if(tally == true){
            parserrs->vt.tally(sampleInput.vid, newvalue);
          }
          
          int crid, fid, aux, vpos, funcid, aux2;
          void * mb;
          
          for(int nf=0; nf<sampleInput.fids.size();nf ++){
            
            crid = sampleInput.crids[nf];
            fid = sampleInput.fids[nf];
            aux = sampleInput.auxs[nf];
            mb = sampleInput.mbs[nf];
            vpos = sampleInput.pos_of_sample_variable[nf];
            funcid = sampleInput.funcids[nf];
            
            if(vpos == -1){
                            
              parserrs->crs[crid]->update(fid, funcs_update[funcid], sampleInput.vid, sampleInput.vvalue, newvalue, !lock);
              
            }else{
              
              mia::sm::IntsBlock * mb =
                (mia::sm::IntsBlock*) this->mat_crs[crid]->lookup(fid);
              
              //std::cout << sampleInput.vid << ", " << mb->get<int>(2+vpos*2) << std::endl;
              
              mb->content[2+vpos*2 + 1] = newvalue;
              
              this->mat_crs[crid]->set(fid, mb);
              
            }
            
          }
          
          if(lock == true){
            
            
            for(int nf=0; nf<sampleInput.fids.size();nf ++){
              
              crid = sampleInput.crids[nf];
              fid = sampleInput.fids[nf];
              
              parserrs->crs[crid]->release(fid);
              
            }
          }
        }
        
        void retrieve(int vid, mia::elly::SampleInput & rs, bool train = false, bool lock = true){
          
          for(int i=0;i<rs.mbs.size();i++){
            delete rs.mbs[i];
          }
          
          rs.aux2s.clear();
          rs.auxs.clear();
          rs.pos_of_sample_variable.clear();
          rs.weights.clear();
          rs.mbs.clear();
          rs.crids.clear();
          rs.fids.clear();
          rs.funcids.clear();
          
          if(train == true){
            rs.vtrain = parserrs->vtrain.lookup(vid);
          }else{
            rs.vtrain = -2;
          }
          
          // first get a list of factors of vid
          mia::sm::IntsBlock factors = parserrs->vf.lookup(vid);
          
          int crid;
          int fid;
          int mbvid;
          int mbvvalue;
          int aux;
          int aux2;
          
          int currentNFactor = 0;
          int currentNVariable = 0;
          
          
          if(lock == true){
            for(int factor=1;factor<factors.size;factor+=2){
              crid = factors.get<int>(factor);
              fid = factors.get<int>(factor+1);
              
              parserrs->crs[crid]->lock(fid); // add warning for dead lock --- add a checker
            }
          }
                    
          rs.vid = vid;
          //rs.vvalue = parserrs->va.lookup(vid);
          rs.vdomain = parserrs->va.lookup_domain(vid);
          
          // for each factor
          for(int factor=1;factor<factors.size;factor+=2){
            
            crid = factors.get<int>(factor);
            fid = factors.get<int>(factor+1);
            
            rs.crids.push_back(crid);
            rs.fids.push_back(fid);
            
            rs.funcids.push_back(parserrs->crs[crid]->function_id);
            rs.weights.push_back(&parserrs->crs[crid]->weights);
            
            // if not incremental factor
            if(funcs_incremental[parserrs->crs[crid]->function_id] == false){
              
              std::vector<int> * empty = new std::vector<int>;
              
              mia::sm::IntsBlock * mb =
                (mia::sm::IntsBlock*) this->mat_crs[crid]->lookup(fid);
              
              aux = mb->get<int>(0);
              aux2 = mb->get<int>(1);
              
              rs.auxs.push_back(aux);
              rs.aux2s.push_back(aux2);
              
              // for each variables in that factor, fetch current values
              currentNVariable = 0;
                            
              for(int nmbv=2; nmbv < mb->size; nmbv += 2){
                
                mbvid = mb->get<int>(nmbv);
                mbvvalue = mb->get<int>(nmbv + 1);
                                
                if(mbvid == vid){
                  
                  rs.vvalue = mbvvalue;
                  
                  //std::cout << rs.pos_of_sample_variable.size() << "~~~" << currentNFactor << std::endl;
                  
                  assert(rs.pos_of_sample_variable.size() == currentNFactor); // variable ID is distinct in each factor! same varialbe do not appear twice.
                  
                  rs.pos_of_sample_variable.push_back(currentNVariable);
                  
                }
                
                empty->push_back(mbvvalue);
                
                currentNVariable ++;
                
              }
              
              delete (mia::sm::IntsBlock *) mb;
              rs.mbs.push_back((void*)empty);
              
            }else{
              
              rs.auxs.push_back(-1);
              rs.aux2s.push_back(-1);
              
              rs.pos_of_sample_variable.push_back(-1);
              void * mb = (void*) parserrs->crs[crid]->lookup(fid);
              
              //std::cout << mb << std::endl;
              
              rs.mbs.push_back(mb);
              
            }
            
            currentNFactor ++;
          }
          
          
        }
        
      };
      
    }
  }
}


#endif
