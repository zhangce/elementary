//
//  Materialization_vcoc.h
//  elly
//
//  Created by Ce Zhang on 9/16/12.
//  Copyright (c) 2012 University of Wisconsin-Madison. All rights reserved.
//

#ifndef elly_Materialization_vcoc_h
#define elly_Materialization_vcoc_h

#include "../dstruct/VariableFactorRelation.h"
#include "Materialization.h"

namespace mia{
  namespace elly{
    
    namespace mat{
      
      template<hazy::sman::StorageType STORAGE>
      class Materialization_vcoc : public mia::elly::mat::Materialization {
        
      public:
        
        mia::elly::utils::FactorFileParser<STORAGE> * parserrs;
        
        Materialization_vcoc(mia::elly::utils::FactorFileParser<STORAGE> * _parserrs){
          
          parserrs = _parserrs;
          
        }
        
        int va_lookup(int64_t vid){
          
          return parserrs->va.lookup(vid);
        }
        
        void * get_parserrs(){
          return parserrs;
        }
        
        mia::sm::IntsBlock vt_lookup(int64_t vid){
          return parserrs->vt.lookup(vid);
        }
        
        int getNCRS(){
          return parserrs->crs.size();
        }
        
        mia::elly::dstruct::AbstractCorrelationRelation * getCRS(int ncrs){
          return parserrs->crs[ncrs];
        }
        
        mia::elly::dstruct::VariableFactorRelation<STORAGE> vf;
        
        void materialize(){
          
          mia::elly::utils::log() << ">> Materializing for V-COC...." << std::endl;
          
          vf.nvariable = parserrs->vf.nvariable;
          
          int nfactor, crid, fid, aux, aux2, nvar;
          for(int vid=0;vid<parserrs->vf.nvariable;vid++){
            
              // nfactor, <crid, fid, aux, aux2, nvar, <vid, vvalue>... >...
            mia::sm::IntsBlock ff;
            
            mia::sm::IntsBlock factors = parserrs->vf.lookup(vid);
            nfactor = factors.content[0];
            ff.append<int>(nfactor);
            
            for(int nf=0;nf<nfactor;nf++){
              crid = factors.content[1+nf*2];
              fid  = factors.content[1+nf*2+1];
            
              ff.append<int>(crid);
              ff.append<int>(fid);
              
              if(funcs_incremental[parserrs->crs[crid]->function_id] == false){
                                
                mia::sm::IntsBlock * mb =
                (mia::sm::IntsBlock*) parserrs->crs[crid]->lookup(fid);
                
                aux = mb->get<int>(0);
                aux2 = mb->get<int>(1);
                
                ff.append<int>(aux);
                ff.append<int>(aux2);
                
                nvar = mb->size - 2;
                ff.append<int>(nvar);
                
                for(int nmbv=2; nmbv < mb->size; nmbv += 2){
                  
                  ff.append<int>(mb->get<int>(nmbv));

                }
                
              }else{
                std::cout << "ERROR: DO NOT SUPPORT MIX'ED FACTOR FOR V-COC." << std::endl;
                assert(false);
              }
            }
            
            vf.kv.load(vid, ff);
            
          }
                    
        }
        
        int getNVariable(){
          return parserrs->va.nvariable;
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
          mia::sm::IntsBlock factors = vf.lookup(vid);
          
          int crid;
          int fid;
          int mbvid;
          int mbvvalue;
          int aux;
          int aux2;
          int nvar;
          
          int currentNFactor = 0;
          int currentNVariable = 0;
          
          if(lock == true){
            for(int factor=1;factor<factors.size;){
              //<crid, fid, aux, aux2, nvar, <vid...> >
              
              crid = factors.get<int>(factor);
              factor ++;
              fid = factors.get<int>(factor);
              factor ++;
              factor ++;
              factor ++;
              nvar = factors.get<int>(factor);
              factor += nvar;
              
              factor ++;
              
              parserrs->crs[crid]->lock(fid); // add warning for dead lock --- add a checker
            }
          }
          
          //std::cout << "###" << std::endl;
          
          rs.vid = vid;
          rs.vvalue = parserrs->va.lookup(vid);
          rs.vdomain = parserrs->va.lookup_domain(vid);
          
          // for each factor
          for(int factor=1;factor<factors.size;){
            
            std::vector<int> * empty = new std::vector<int>;
            
              // <crid, fid, aux, aux2, nvar, <vid...> >
            crid = factors.get<int>(factor);
            factor ++;
            fid = factors.get<int>(factor);
            
            if(funcs_incremental[parserrs->crs[crid]->function_id] == true){
              std::cout << "ERROR: DO NOT SUPPORT MIX'ED FACTOR FOR V-COC." << std::endl;
              assert(false);
            }
            
            rs.crids.push_back(crid);
            rs.fids.push_back(fid);
            
            rs.funcids.push_back(parserrs->crs[crid]->function_id);
            rs.weights.push_back(&parserrs->crs[crid]->weights);
            
            factor ++;
            aux = factors.get<int>(factor);
            factor ++;
            aux2 = factors.get<int>(factor);
            factor ++;
            nvar = factors.get<int>(factor);
            
            rs.auxs.push_back(aux);
            rs.aux2s.push_back(aux2);
            
            currentNVariable = 0;
            for(int nvid=0; nvid<nvar; nvid ++){
              factor ++;
              
              mbvid = factors.get<int>(factor);
              mbvvalue = parserrs->va.lookup(mbvid);
              
              //std::cout << vid << ", " << mbvid << std::endl;
              
              if(mbvid == vid){
                
                //std::cout << rs.pos_of_sample_variable.size() << "~~~" << currentNFactor << std::endl;
                
                assert(rs.pos_of_sample_variable.size() == currentNFactor); // variable ID is distinct in each factor! same varialbe do not appear twice.
                
                rs.pos_of_sample_variable.push_back(currentNVariable);
                
              }
              
              empty->push_back(mbvvalue);
              
              currentNVariable ++;
              
            }
            
            rs.mbs.push_back((void*)empty);
            currentNFactor ++;
            factor ++;
          
          }
            
        }
        
      };
      
    }
  }
}


#endif
