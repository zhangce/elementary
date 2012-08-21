//
//  KV_int_IntsBlock_-_DIRECT_-_DENSEKEY.h
//  sman
//
//  Created by Ce Zhang on 8/19/12.
//  Copyright (c) 2012 HazyResearch. All rights reserved.
//

#ifndef sman_KV_int_IntsBlock___DIRECT___DENSEKEY_h
#define sman_KV_int_IntsBlock___DIRECT___DENSEKEY_h

#include <vector>

#include "../KV.h"
#include "../dstruct/IntsBlock.h"
#include "../dstruct/SlottedPage.h"

#include "../direct/KV_int_-_MM_DIRECT_-_DENSEKEY.h"
#include "../direct/KV_int_-_MMAP_DIRECT_-_DENSEKEY.h"

namespace mia{
    
    /**
     * \brief Namespace for storage manager.
     **/
    namespace sm{
    
        /**
         * \brief Key-value store for dense int key, variable length value, in MMAP.
         **/
        template<KV_Replacement REPLACEMENT>
        class KV<int, IntsBlock, MMAP, DIRECT, REPLACEMENT, DENSE_KEY>{
            
        private:
            
            KV<int, mia::sm::IntPair, MMAP, DIRECT, REPLACEMENT, DENSE_KEY> kv_key2page;
            KV<int, mia::sm::SlottedPage, MMAP, DIRECT, REPLACEMENT, DENSE_KEY> kv_page2content;
            
            int currentPage;
            
            mia::sm::SlottedPage emptyPage;
            
            //friend class Iterator;
            
        public:
                        
            KV(){
                std::cout << "Use Specialization KV_int_IntsBlock_-_DIRECT_-_DENSEKEY ." << std::endl;
                currentPage = 0;
                 
                kv_page2content.load(currentPage, emptyPage);
                
            }
            
            //Iterator get_iterator(){
            //    return Iterator(this);
            //}
            
            KV_Status get(int const & key, mia::sm::IntsBlock & value){
                
                mia::sm::IntPair pageid;
                int rs = kv_key2page.get(key, pageid);
                
                if(rs != SUCCESS){
                    return GET_ERROR_KEY_NOT_FOUND;
                }
                
                mia::sm::SlottedPage page;
                rs = kv_page2content.get(pageid.first, page);
                
                if(rs != SUCCESS){
                    return GET_ERROR_KEY_NOT_FOUND;
                }
                
                value = page.get(pageid.second);
                
                return SUCCESS;
            }
            
            KV_Status set(int const & key, mia::sm::IntsBlock const & value){
                
                mia::sm::IntPair pageid;
                int rs = kv_key2page.get(key, pageid);
                
                if(rs != SUCCESS){
                    return GET_ERROR_KEY_NOT_FOUND;
                }

                mia::sm::SlottedPage page;
                rs = kv_page2content.get(pageid.first, page);
                
                if(rs != SUCCESS){
                    return GET_ERROR_KEY_NOT_FOUND;
                }
                
                page.inplace_update(pageid.second, value);  //todo: inplace only now
                kv_page2content.set(pageid.first, page);
                
                return SUCCESS;
            }
            
            KV_Status load(int const & key, mia::sm::IntsBlock const & value){
                
                mia::sm::SlottedPage page;
                kv_page2content.get(currentPage, page);
                
                int slot = page.push(value);
                
                //std::cout << slot << " @ " << currentPage << std::endl;
                
                if(slot != -1){
                    kv_page2content.set(currentPage, page);
                }else{
                    mia::sm::SlottedPage page;
                    slot = page.push(value);
                    currentPage ++;
                    kv_page2content.load(currentPage, emptyPage);
                }
                
                mia::sm::IntPair pageid;
                pageid.first = currentPage;
                pageid.second = slot;
                
                kv_key2page.load(key, pageid);
                
                return SUCCESS;
            }
            
        };
    
        
    }
}



#endif
