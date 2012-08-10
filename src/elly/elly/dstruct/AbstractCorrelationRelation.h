//
//  AbstractCorrelationRelation.h
//  elly
//
//  Created by Ce Zhang on 8/6/12.
//  Copyright (c) 2012 University of Wisconsin-Madison. All rights reserved.
//

#ifndef elly_AbstractCorrelationRelation_h
#define elly_AbstractCorrelationRelation_h

namespace mia{
    namespace elly{
        namespace dstruct{
            
            //template<class TYPE = mia::sm::IntsBlock>
            class AbstractCorrelationRelation {
                
            public:
                
                std::string factor_name;
                
                int factor_id;
                int function_id;
                std::string filename;
                std::string filetype;
                
                std::string mapfilename;
                std::vector<double> weights;    // assume in-memory weights
                
                void prepare_weights(){
                    
                    if(mapfilename.compare("") == 0){
                        return;
                    }
                    
                    std::ifstream fin( mapfilename.c_str() );
                    double weight;
                    
                    int nweight = 0;
                    while(fin >> weight){
                        weights.push_back(weight);
                        nweight ++;
                    }
                    
                    mia::elly::utils::log() << "    + # weights = " << nweight << std::endl;
                    
                    fin.close();
                    
                }
                
                void dump_weights(){
                    
                    if(mapfilename.compare("") == 0){
                        mia::elly::utils::log() << "    + no weights " << std::endl;
                        return;
                    }
                    
                    std::ofstream fout( mapfilename.c_str() );
                    
                    int nweight = 0;
                    
                    for(int i=0;i<weights.size();i++){
                        fout << weights[i] << std::endl;
                        nweight ++;
                    }
                    
                    mia::elly::utils::log() << "    + # weights = " << nweight << std::endl;
                    
                    fout.close();
                    
                }
                
                
                
                std::vector<pthread_mutex_t* > sems;    // assume in-memory semaphores
                
                int nfactor;
                
                void lock(int fid){
                    //std::cout << "lock" << fid << std::endl;
                    pthread_mutex_lock(sems[fid]);
                    //std::cout << "donelock" << fid << std::endl;
                }
                
                void release(int fid){
                    //std::cout << "unlock" << fid << std::endl;
                    pthread_mutex_unlock(sems[fid]);
                    //std::cout << "doneunlock" << fid << std::endl;
                }
                
                ~AbstractCorrelationRelation(){
                    for(int i=0;i<sems.size();i++){
                        pthread_mutex_destroy(sems[i]);
                    }
                }
                
                virtual void * lookup(int fid) = 0;
                
                virtual void prepare() = 0;
                
                virtual void update(int key, void (*func_update)(void * , int , int , int ), int vid, int from, int to) = 0;
                
            };
            
            
            
            
            
            
        }
    }
}

#endif
