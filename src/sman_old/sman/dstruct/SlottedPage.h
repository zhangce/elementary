//
//  SlottedPage.h
//  sman
//
//  Created by Ce Zhang on 8/19/12.
//  Copyright (c) 2012 HazyResearch. All rights reserved.
//

#ifndef sman_SlottedPage_h
#define sman_SlottedPage_h

#include "IntsBlock.h"

namespace mia{
    namespace sm{
    
        /**
         * \brief Slotted page, with each record a mia::sm::IntsBlock.
         **/
        class SlottedPage{
            
        public:
            
            /**
             * Content as an int array of one page.
             *
             * layout of each page:
             *     PAGESIZENINT-1: int, current nint offset can be used for data
             *     PAGESIZENINT-2: int, current nint offset can be used for slot
             *     CurrentSlotOffset ~ PAGESIZENINT-3: int, nint for record
             * each record:
             *     int --- size of the recort (nin
             *     int,int,... --- content
             *
             **/
            int content[PageNINT];
            
            SlottedPage(){
                setCurrentDataOffset(0);
                setCurrentSlotOffset(PageNINT-3);
            }
            
            int getCurrentDataOffset(){
                return content[PageNINT-1];
            }
            
            void setCurrentDataOffset(int _int){
                content[PageNINT-1] = _int;
            }
            
            int getCurrentSlotOffset(){
                return content[PageNINT-2];
            }
            
            void setCurrentSlotOffset(int _int){
                content[PageNINT-2] = _int;
            }
            
            // return
            //    true: suc. put in
            //    false: failed. current page full.
            // not thread safe
            int push(IntsBlock obj){
                
                if( getCurrentDataOffset() + 1 + obj.size >
                   getCurrentSlotOffset()){
                    return -1;
                }
                
                
                content[getCurrentSlotOffset()] = getCurrentDataOffset();
                setCurrentSlotOffset(getCurrentSlotOffset() - 1);
                
                content[getCurrentDataOffset()] = obj.size;
                for(int i=0;i<obj.size;i++){
                    content[getCurrentDataOffset()+1+i] = obj.content[i];
                }
                
                setCurrentDataOffset(getCurrentDataOffset() + 1 + obj.size);
                
                return getNSlot() - 1;
                
            }
            
            int getNSlot(){
                return PageNINT - 3 - getCurrentSlotOffset() ;
            }
            
            int getDataPos(int nslot){  // nslot starts from 0
                return content[PageNINT - 3 - nslot] ;
            }
            
            
            IntsBlock get(int nslot){
                IntsBlock obj;
                obj.size = content[getDataPos(nslot)];
                for(int i=0;i<obj.size;i++){
                    obj.content[i] = content[getDataPos(nslot) + 1 + i];
                }
                return obj;
            }
            
            void inplace_update(int nslot, IntsBlock obj){
                assert(content[getDataPos(nslot)] == obj.size);
                for(int i=0;i<obj.size;i++){
                    content[getDataPos(nslot) + 1 + i] = obj.content[i];
                }
            }
            
        };
    
        
    }
}

#endif
