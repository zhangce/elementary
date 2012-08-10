//
//  Factor.h
//  elly
//
//  Created by Ce Zhang on 7/6/12.
//  Copyright (c) 2012 University of Wisconsin-Madison. All rights reserved.
//

#ifndef elly_Factor_h
#define elly_Factor_h

namespace elly {
    namespace adhoc{
        
        template<class STATE>
        class Factor{
            
            virtual double potential(STATE& _state) = 0;
            
            virtual void update(STATE& _state, int64_t _vid, int64_t _old_value, int64_t _new_value) = 0;
            
        };
        
        
    }
}


#endif
