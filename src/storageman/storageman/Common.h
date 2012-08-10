//
//  Common.h
//  storageman
//
//  Created by Ce Zhang on 7/10/12.
//  Copyright (c) 2012 University of Wisconsin-Madison. All rights reserved.
//

#ifndef storageman_Common_h
#define storageman_Common_h

#define PageSIZE     4096000        // in byte
#define SIZEOF(X)    sizeof(X)

#define PageNINT (PageSIZE/4)       // in #int
#define SIZEOFINT (sizeof(int))

#define MemBlockSIZE 4096                   // in Byte
#define MemBlockNINT (MemBlockSIZE/4)       // in #int
#define SIZEOFINT (sizeof(int))

////////

#define BufferSIZE   409600000
#define BufferNPAGE  (BufferSIZE/PageSIZE)

#define MMAP_INIT_NPAGE 1

int shared_tmp_fid = 0;
static std::string getNextTmpFileName(){
    char tmp[1000];
    sprintf(tmp, "/tmp/tmp-%d", shared_tmp_fid ++);
    return std::string(tmp);
}


#endif
