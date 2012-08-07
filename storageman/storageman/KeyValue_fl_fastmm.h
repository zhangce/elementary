//
//  KeyValue_fl_fastmm.h
//  elly
//
//  Created by Ce Zhang on 8/7/12.
//  Copyright (c) 2012 University of Wisconsin-Madison. All rights reserved.
//

#ifndef elly_KeyValue_fl_fastmm_h
#define elly_KeyValue_fl_fastmm_h

#include <assert.h>

namespace mia{
    namespace sm{
        
        
        template<class TYPE>
        class KeyValue_fl_fastmm{
        public:
            
            std::vector<TYPE> buf;
            int ckey;
            
            KeyValue_fl_fastmm(){ckey=-1;}
            
            TYPE get(int key){
                
                return buf[key];
                
            }
            
            void adhoc_update(int key, int from, int to){
                
                //char aaa[100];
                //sprintf(aaa, "%d\n", buf[key].counts[from]);
                
                //std::cout << aaa << std::endl;
                
                //int a = buf[key].counts[from];
                
                buf[key].counts[from] --;
                buf[key].counts[to] ++;
                
                //int b = buf[key].counts[from];
                
                //sprintf(aaa, "%d\n\n", buf[key].counts[from]);
                //std::cout << aaa << std::endl;
                
                //if(b < 0){
                //    sprintf(aaa, "%d\n", a);
                //    std::cout << aaa << std::endl;
                //
                //    sprintf(aaa, "%d\n\n", b);
                //    std::cout << aaa << std::endl;
                //}
                
                //std::cout << buf[key].counts[from]  << std::endl;
                //assert(buf[key].counts[from] >= 0);
            }
            
            void set(int key, TYPE block){
                
                //std::cout << "want to set key " << key << " to " << block << std::endl;
                
                assert(key <= ckey + 1 && key >= 0);    // only inc. set or inplace set
                                
                if(key == ckey + 1){    // push
                    
                    buf.push_back(block);
                 
                }else{                  // inplace update
                    
                    //std::cout << key << " " << buf.size() << std::endl;
                    buf[key] = block;
                    
                }
                
                if(ckey < key){
                    ckey = key;
                }
                
                
            }
            
            
        };
        
        
    }}




#endif
