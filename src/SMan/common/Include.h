//
//  Include.h
//  elly
//
//  Created by Ce Zhang on 9/10/12.
//  Copyright (c) 2012 University of Wisconsin-Madison. All rights reserved.
//

#ifndef elly_Include_h
#define elly_Include_h

#include "../common/Common.h"
#include "../objstore/ObjStore.h"

#include "../objstore/ObjectStore_MM.h"
#include "../objstore/ObjectStore_FILE.h"
#include "../objstore/ObjectStore_JHASH.h"
#include "../objstore/ObjectStore_HBASE.h"

#include "../objstore/BufferedObjStore.h"

#include "../kvstore/PagedBufferedObjectStore.h"


#ifndef COMMON_PAGESIZE
std::cout << "MACRO: COMMON_PAGESIZE: " << (COMMON_PAGESIZE) << std::endl;
//std::cout << "MACRO: COMMON_PAGESIZE2: " << (COMMON_PAGESIZE2) << std::endl;
#else
#define COMMON_PAGESIZE 40960
std::cout << "MACRO: COMMON_PAGESIZE: " << (COMMON_PAGESIZE) << std::endl;
#endif

#endif
