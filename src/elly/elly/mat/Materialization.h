//
//  Materialization.h
//  elly
//
//  Created by Ce Zhang on 9/16/12.
//  Copyright (c) 2012 University of Wisconsin-Madison. All rights reserved.
//

#ifndef elly_Materialization_h
#define elly_Materialization_h

namespace mia{
  namespace elly{
    namespace mat{
  
      class Materialization{
        
      public:
        
        virtual void * get_parserrs() = 0;
        
        virtual void materialize() = 0;
        
        virtual int getNVariable() = 0;
        
        virtual mia::sm::IntsBlock vt_lookup(int64_t vid) = 0;
        
        virtual int va_lookup(int64_t vid) = 0;
                
        virtual int getNCRS() = 0;
        virtual mia::elly::dstruct::AbstractCorrelationRelation * getCRS(int ncrs) = 0;
        
        virtual void update(mia::elly::SampleInput & sampleInput, int newvalue, bool tally = false, bool lock = true) = 0;
        
        virtual void retrieve(int vid, mia::elly::SampleInput & rs, bool train = false, bool lock = true) = 0;
        
        
      };
      
    }
  }
}

#endif
