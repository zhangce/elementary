//
//  Common.h
//  sman
//
//  Created by Ce Zhang on 8/19/12.
//  Copyright (c) 2012 HazyResearch. All rights reserved.
//

#ifndef sman_Common_h
#define sman_Common_h


#define PageSIZE     40960        // in byte
#define SIZEOF(X)    sizeof(X)

#define PageNINT (PageSIZE/4)       // in #int
#define SIZEOFINT (sizeof(int))

#define MemBlockSIZE 4096                   // in Byte
#define MemBlockNINT (MemBlockSIZE/4)       // in #int
#define SIZEOFINT (sizeof(int))

#define MMAP_INIT_NELEMENT 1

int shared_tmp_fid = 0;
static std::string getNextTmpFileName(){
    char tmp[1000];
    sprintf(tmp, "/tmp/tmp-%d", shared_tmp_fid ++);
    return std::string(tmp);
}

#endif
