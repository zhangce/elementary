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
                
                int nfactor;
                
                virtual void * lookup(int fid) = 0;
                
                virtual void prepare() = 0;
                
                virtual void update(int key, void * new_content) = 0;
                
            };
            
            
            
            
            
            
        }
    }
}

#endif
