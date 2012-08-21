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
            
            /**
             * \brief Abstract class for unifying mia::elly::dstruct::StandardCorrelationRelation and mia::elly::dstruct::IncrementalCorrelationRelation.
             */
            class AbstractCorrelationRelation {
                
            public:
                
                std::string factor_name;
                
                /**
                 * ID of this correlation relation.
                 */
                int factor_id;
                
                /**
                 * Factor ID -- for potential/update/gradient functions.
                 */
                int function_id;
                
                /**
                 * File that contains this correlation relation.
                 */
                std::string filename;
                
                /**
                 * Type of correlation relation file. {tsv}
                 */
                std::string filetype;
                
                /**
                 * File that contains weights.
                 */
                std::string mapfilename;
                
                /**
                 * In-memory vector that stores weights.
                 */
                std::vector<double> weights;    // assume in-memory weights
                
                /**
                 * load weight file.
                 */
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
                
                /**
                 * Refresh weight file by current weights.
                 */
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
                
                
                /**
                 * In-memory locks for factors in this correlation relation.
                 */
                std::vector<pthread_mutex_t* > sems;    // assume in-memory semaphores
                
                /**
                 * Number of factors being loaded.
                 */
                int nfactor;
                
                /**
                 * Lock a factor.
                 */
                void lock(int fid){
                    //std::cout << "lock" << fid << std::endl;
                    pthread_mutex_lock(sems[fid]);
                    //std::cout << "donelock" << fid << std::endl;
                }
                
                /**
                 * Release lock of a factor.
                 */
                void release(int fid){
                    //std::cout << "unlock" << fid << std::endl;
                    pthread_mutex_unlock(sems[fid]);
                    //std::cout << "doneunlock" << fid << std::endl;
                }
                
                /**
                 * Deconstructor. Release and destroy all locks.
                 */
                ~AbstractCorrelationRelation(){
                    for(int i=0;i<sems.size();i++){
                        pthread_mutex_destroy(sems[i]);
                    }
                }
                
                virtual void * lookup(int fid) = 0;
                
                virtual void prepare() = 0;
                
                virtual void update(int key, void (*func_update)(void * , int , int , int ), int vid, int from, int to, bool lock) = 0;
                
            };
            
            
            
            
            
            
        }
    }
}

#endif
