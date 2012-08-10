//
//  main.cpp
//  storageman
//
//  Created by Ce Zhang on 7/10/12.
//  Copyright (c) 2012 University of Wisconsin-Madison. All rights reserved.
//

#include <iostream>

struct tests{
    int aaa;
    float bbb;
    bool ccc;
};

#include "IntsBlock.h"
using namespace mia::sm;


void test_MemBlock(){
    
    IntsBlock block;
    block.append<int>(5);
    block.append<double>(3.1415926);
    
    tests s;
    s.aaa = 10;
    s.bbb = 10.1;
    s.ccc = true;
    
    
    block.append<tests>(s);
    block.append<float>(1.11);
    
    std::cout << block.get<int>(0) << std::endl;
    std::cout << block.get<double>(1) << std::endl; 
    
    tests ss;
    ss = block.get<tests>(1+sizeof(double)/sizeof(int));
    std::cout << ss.aaa << std::endl;
    ss.aaa = 20;
    
    tests sss;
    sss = block.get<tests>(1+sizeof(double)/sizeof(int));
    std::cout << sss.aaa << std::endl;
    std::cout << sss.bbb << std::endl;
    std::cout << sss.ccc << std::endl;
    
    std::cout << block.get<float>(1+sizeof(double)/sizeof(int)
                                   +sizeof(tests)/sizeof(int))
            << std::endl;
    
}


#include "SlottedPage.h"
void test_SlottedPage(){
    
    SlottedPage<int> page;
    
    IntsBlock block;
    block.append<int>(5);
    block.append<double>(3.1415926);
    tests s;
    s.aaa = 10;
    s.bbb = 10.1;
    s.ccc = true;
    block.append<tests>(s);
    block.append<float>(1.11);

    int nslot = page.push(block);
    std::cout << "slot id = " << nslot << std::endl;

    block.content[0] = 6;
    nslot = page.push(block);
    std::cout << "slot id = " << nslot << std::endl;
    
    IntsBlock b1 = page.get(0);
    std::cout << "### b1 ###" << std::endl;
    std::cout << "size = " << b1.size << std::endl;
    std::cout << b1.get<int>(0) << std::endl;
    std::cout << b1.get<double>(1) << std::endl; 
    tests ss;
    ss = b1.get<tests>(1+sizeof(double)/sizeof(int));
    std::cout << ss.aaa << std::endl;
    std::cout << ss.bbb << std::endl;
    std::cout << ss.ccc << std::endl;
    std::cout << b1.get<float>(1+sizeof(double)/sizeof(int)
                                  +sizeof(tests)/sizeof(int))
        << std::endl;
    
    IntsBlock b2 = page.get(1);
    std::cout << "### b2 ###" << std::endl;
    std::cout << "size = " << b2.size << std::endl;
    std::cout << b2.get<int>(0) << std::endl;
    std::cout << b2.get<double>(1) << std::endl; 
    tests sss;
    sss = b2.get<tests>(1+sizeof(double)/sizeof(int));
    std::cout << sss.aaa << std::endl;
    std::cout << sss.bbb << std::endl;
    std::cout << sss.ccc << std::endl;
    std::cout << b2.get<float>(1+sizeof(double)/sizeof(int)
                               +sizeof(tests)/sizeof(int))
    << std::endl;
    
    page.inplace_update(0, b2);
    b1 = page.get(0);
    std::cout << "### b1 (should = b2) ###" << std::endl;
    std::cout << "size = " << b1.size << std::endl;
    std::cout << b1.get<int>(0) << std::endl;
    std::cout << b1.get<double>(1) << std::endl; 
    tests ssss;
    ssss = b1.get<tests>(1+sizeof(double)/sizeof(int));
    std::cout << ssss.aaa << std::endl;
    std::cout << ssss.bbb << std::endl;
    std::cout << ssss.ccc << std::endl;
    std::cout << b1.get<float>(1+sizeof(double)/sizeof(int)
                               +sizeof(tests)/sizeof(int))
    << std::endl;

    
}

#include "UniformPage.h"
void test_UniformPage(){
    
    UniformPage<IntsBlock> page;
    
    IntsBlock block;
    block.append<int>(5);
    block.append<double>(3.1415926);
    tests s;
    s.aaa = 10;
    s.bbb = 10.1;
    s.ccc = true;
    block.append<tests>(s);
    block.append<float>(1.11);
    
    int nslot = page.push(block);
    std::cout << "slot id = " << nslot << std::endl;
    
    block.content[0] = 6;
    nslot = page.push(block);
    std::cout << "slot id = " << nslot << std::endl;
    
    IntsBlock b1 = page.get(0);
    std::cout << "### b1 ###" << std::endl;
    std::cout << "size = " << b1.size << std::endl;
    std::cout << b1.get<int>(0) << std::endl;
    std::cout << b1.get<double>(1) << std::endl; 
    tests ss;
    ss = b1.get<tests>(1+sizeof(double)/sizeof(int));
    std::cout << ss.aaa << std::endl;
    std::cout << ss.bbb << std::endl;
    std::cout << ss.ccc << std::endl;
    std::cout << b1.get<float>(1+sizeof(double)/sizeof(int)
                               +sizeof(tests)/sizeof(int))
    << std::endl;
    
    IntsBlock b2 = page.get(1);
    std::cout << "### b2 ###" << std::endl;
    std::cout << "size = " << b2.size << std::endl;
    std::cout << b2.get<int>(0) << std::endl;
    std::cout << b2.get<double>(1) << std::endl; 
    tests sss;
    sss = b2.get<tests>(1+sizeof(double)/sizeof(int));
    std::cout << sss.aaa << std::endl;
    std::cout << sss.bbb << std::endl;
    std::cout << sss.ccc << std::endl;
    std::cout << b2.get<float>(1+sizeof(double)/sizeof(int)
                               +sizeof(tests)/sizeof(int))
    << std::endl;
    
    page.inplace_update(0, b2);
    b1 = page.get(0);
    std::cout << "### b1 (should = b2) ###" << std::endl;
    std::cout << "size = " << b1.size << std::endl;
    std::cout << b1.get<int>(0) << std::endl;
    std::cout << b1.get<double>(1) << std::endl; 
    tests ssss;
    ssss = b1.get<tests>(1+sizeof(double)/sizeof(int));
    std::cout << ssss.aaa << std::endl;
    std::cout << ssss.bbb << std::endl;
    std::cout << ssss.ccc << std::endl;
    std::cout << b1.get<float>(1+sizeof(double)/sizeof(int)
                               +sizeof(tests)/sizeof(int))
    << std::endl;
    
    
}


#include "Buffer_mm.h"
void test_buffer_mm(){
    
    IntsBlock block;
    block.append<int>(5);
    block.append<int>(1);
    tests s;
    s.aaa = 10;
    s.bbb = 10.1;
    s.ccc = true;
    block.append<tests>(s);
    block.append<float>(1.11);

    Buffer_mm<SlottedPage, IntsBlock> buf;
    
    for(int i=0;i<10000000;i++){
        int pageid, slotid;
        buf.push(block, pageid, slotid);
        //std::cout << "pageid = " << pageid << ", slotid = " << slotid << std::endl;
        block.content[1] = i;
        buf.set(pageid, slotid, block);
        IntsBlock b = buf.get(pageid, slotid);
        //std::cout << b.content[1] << std::endl;
        assert(b.content[1] == i);
    }
    
}

struct testss{
    int a;
    int b;
    tests c;
    float d;
};

#include "Buffer_mmap.h"

#include "KeyValue_fl.h"
#include "KeyValue_vl.h"
void test_kv(){
    
    //KeyValue_vl<Buffer_mm> kv;
    //KeyValue_fl<Buffer_mm, testss> kv;
    
    KeyValue_vl<Buffer_mmap> kv;
    //KeyValue_fl<Buffer_mmap, testss> kv;
    
    IntsBlock block;
    block.append<int>(5);
    block.append<int>(1);
    tests s;
    s.aaa = 10;
    s.bbb = 10.1;
    s.ccc = true;
    block.append<tests>(s);
    block.append<float>(1.11);
    
    testss sblock;
    sblock.a = 5;
    sblock.b = 1;
    sblock.c.aaa = 10;
    sblock.c.bbb = 10.1;
    sblock.c.ccc = true;
    sblock.d = 1.11;
    
    for(int i=0;i<1000000;i++){
        
        //sblock.b = 1000000-i;
        //kv.set(i, sblock);
        
        block.content[1] = 1000000-i;
        kv.set(i, block);
        
    }
    
    for(int i=0;i<1000000;i++){
        
        IntsBlock block = kv.get(i);
        //testss sblock = kv.get(i);
        
        //std::cout << i << "," << block.content[1] << ","
        //<< (i + block.content[1])<< std::endl;
        
        //std::cout << i << "," << sblock.b << ","
        //    << (i + sblock.b)
        //<< std::endl;
    }
    
}


int main(int argc, const char * argv[])
{

    //test_MemBlock();
    //test_SlottedPage();
    //test_buffer_mm();
    //test_UniformPage();
    
    test_kv();
    
    // insert code here...
    std::cout << "Hello, World!\n";
    return 0;
}

