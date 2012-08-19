//
//  main.cpp
//  sman
//
//  Created by Ce Zhang on 8/18/12.
//  Copyright (c) 2012 HazyResearch. All rights reserved.
//

#include <iostream>
#include <string>

#include "KV.h"

#include "direct/KV_int_-_MMAP_DIRECT_-_DENSEKEY.h"
#include "direct/KV_int_-_MM_DIRECT_-_DENSEKEY.h"
#include "direct/KV_-_-_MM_DIRECT_-_-.h"
#include "direct/KV_-_-_MMAP_DIRECT_-_-.h"

#include "complex/KV_int_IntsBlock_-_-_-_DENSEKEY.h"

struct tests{
    int aaa;
    float bbb;
    bool ccc;
};

void test_KV_int_IntsBlock___DENSEKEY(){
    
    mia::sm::KV<int, mia::sm::IntsBlock, mia::sm::MMAP, mia::sm::DIRECT, mia::sm::NIL, mia::sm::DENSE_KEY> kv;
    
    mia::sm::IntsBlock block;
    block.append<int>(5);
    block.append<int>(1);
    tests s;
    s.aaa = 10;
    s.bbb = 10.1;
    s.ccc = true;
    block.append<tests>(s);
    block.append<float>(1.11);
    
    for(int i=0;i<100000;i++){
                
        block.content[1] = 100000-i;
                
        kv.load(i, block);
        
        //std::cout << i << "," << block.content[1] << ","
        //<< (i + block.content[1])<< std::endl;
        
    }
    
    for(int i=0;i<100000;i++){
        
        kv.get(i, block);

        std::cout << i << "," << block.content[1] << ","
        << (i + block.content[1])<< std::endl;
    }
    
}


void test_KV_int__MMAP_DIRECT__DENSEKEY(){
    mia::sm::KV<int, int, mia::sm::MMAP, mia::sm::DIRECT, mia::sm::NIL, mia::sm::DENSE_KEY> kv;
    
    int value;
    kv.load(1, 1);
    kv.load(2, 2);
    kv.load(10, 3);
    
    kv.get(1, value);
    std::cout << value << std::endl;
    
    kv.get(2, value);
    std::cout << value << std::endl;
    
    kv.get(10, value);
    std::cout << value << std::endl;
    
    kv.set(1, 3);
    kv.set(2, 2);
    kv.set(10, 1);
    
    kv.get(1, value);
    std::cout << value << std::endl;
    
    kv.get(2, value);
    std::cout << value << std::endl;
    
    kv.get(10, value);
    std::cout << value << std::endl;
    
    
    mia::sm::KV<int, int, mia::sm::MMAP, mia::sm::DIRECT, mia::sm::NIL, mia::sm::DENSE_KEY>::Iterator it = kv.get_iterator();
    
    int k, v;
    while(it.hasNext()){
        it.next(k, v);
        std::cout << "(k,v) = " << k << ", " << v << std::endl;
    }
    
    std::cout << std::endl;
    
}


void test_KV_int__MM_DIRECT__DENSEKEY(){
    mia::sm::KV<int, int, mia::sm::MM, mia::sm::DIRECT, mia::sm::NIL, mia::sm::DENSE_KEY> kv;
    
    int value;
    kv.load(1, 1);
    kv.load(2, 2);
    kv.load(10, 3);
    
    kv.get(1, value);
    std::cout << value << std::endl;
    
    kv.get(2, value);
    std::cout << value << std::endl;
    
    kv.get(10, value);
    std::cout << value << std::endl;
    
    kv.set(1, 3);
    kv.set(2, 2);
    kv.set(10, 1);
    
    kv.get(1, value);
    std::cout << value << std::endl;
    
    kv.get(2, value);
    std::cout << value << std::endl;
    
    kv.get(10, value);
    std::cout << value << std::endl;
    
  
    mia::sm::KV<int, int, mia::sm::MM, mia::sm::DIRECT, mia::sm::NIL, mia::sm::DENSE_KEY>::Iterator it = kv.get_iterator();
    
    int k, v;
    while(it.hasNext()){
        it.next(k, v);
        std::cout << "(k,v) = " << k << ", " << v << std::endl;
    }
    
    std::cout << std::endl;
    
}

void test_KV___MMAP_DIRECT__(){

    mia::sm::KV<std::string, int, mia::sm::MMAP, mia::sm::DIRECT, mia::sm::NIL, mia::sm::NO_HINT> kv;
    
}

void test_KV___MM_DIRECT__(){
    
    mia::sm::KV<std::string, int, mia::sm::MM, mia::sm::DIRECT, mia::sm::NIL, mia::sm::NO_HINT> kv;
    
    // exepcte: 1 2 3 3 2 1
    
    int value;
    kv.load("1", 1);
    kv.load("2", 2);
    kv.load("3", 3);
    
    kv.get("1", value);
    std::cout << value << std::endl;

    kv.get("2", value);
    std::cout << value << std::endl;
    
    kv.get("3", value);
    std::cout << value << std::endl;
    
    kv.set("1", 3);
    kv.set("2", 2);
    kv.set("3", 1);
    
    kv.get("1", value);
    std::cout << value << std::endl;
    
    kv.get("2", value);
    std::cout << value << std::endl;
    
    kv.get("3", value);
    std::cout << value << std::endl;
    
    
    mia::sm::KV<std::string, int, mia::sm::MM, mia::sm::DIRECT, mia::sm::NIL, mia::sm::NO_HINT>::Iterator it = kv.get_iterator();
    
    
    std::string k;
    int v;
    while(it.hasNext()){
        it.next(k, v);
        std::cout << "(k,v) = " << k << ", " << v << std::endl;
    }
    
    std::cout << std::endl;
    
}



int main(int argc, const char * argv[])
{

    test_KV_int_IntsBlock___DENSEKEY();
    
    test_KV___MM_DIRECT__();
    
    test_KV_int__MM_DIRECT__DENSEKEY();
    
    test_KV_int__MMAP_DIRECT__DENSEKEY();
    
    //test_KV___MMAP_DIRECT__();
    
    
    // insert code here...
    std::cout << "Hello, World!\n";
    return 0;
}

