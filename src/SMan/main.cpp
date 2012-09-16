
//
//  main.cpp
//  SMan
//
//  Created by Ce Zhang on 9/7/12.
//  Copyright (c) 2012 HazyResearch. All rights reserved.
//

#include "common/Common.h"
#include "objstore/ObjStore.h"

#include "objstore/ObjectStore_MM.h"
#include "objstore/ObjectStore_FILE.h"
#include "objstore/ObjectStore_JHASH.h"

#include "objstore/BufferedObjStore.h"

inline void func_pp(double & value){
  ++value;
}

void test_ObjStore_MM_NAIVE(int nkey = 100000){
  
  hazy::utils::Timer t;
  double load, set, get, apply;
  
  double* mm = new double[nkey];
  
  t.restart();
  for (int i=0;i<nkey; ++i){
    mm[i] = i;
  }
  load = t.elapsed();
  
  t.restart();
  for (int i=0;i<nkey; ++i){
    mm[i] = 3.14 * i;
  }
  set = t.elapsed();
  
  t.restart();
  double value;
  for (int i=nkey-1;i>=0; --i){
    value = mm[i];
    assert(3.14*i == value);
    value = 5;
    value = mm[i];
    assert(3.14*i == value);
  }
  get = t.elapsed();
  
  t.restart();
  for (int i=nkey-1;i>=0; --i){
    mm[i] ++;
  }
  apply = t.elapsed();
  
  std::cout << "TEST: " << "ObjStore_MM_NAIVE @ " << nkey << " KEYS uses " << std::endl
            << load << ",\t" << set << ",\t" << get << ",\t" << apply << " \tseconds."
            << std::endl << std::endl;
}


template<hazy::sman::StorageType STORAGE>
void test_ObjStore(int nkey = 100000){
  
  hazy::utils::Timer t;
  double load, set, get, apply;
  
  hazy::sman::ObjStore<double, STORAGE, hazy::sman::PROPERTY_NIL> os;
  
  t.restart();
  for (int i=0;i<nkey; ++i){
    os.load(i, i);
  }
  load = t.elapsed();
  std::cout << "loaded" << std::endl;
  
  t.restart();
  for (int i=0;i<nkey; ++i){
    os.set(i, i * 3.14);
  }
  set = t.elapsed();
  std::cout << "set" << std::endl;
  
  t.restart();
  double value;
  for (int i=nkey-1;i>=0; --i){
    os.get(i, value);
    assert(3.14*i == value);
    value = 5;
    os.get(i, value);
    assert(3.14*i == value);
  }
  get = t.elapsed();
  std::cout << "get" << std::endl;
  
  t.restart();
  for (int i=nkey-1;i>=0; --i){
    os.apply(i, func_pp);
  }
  apply = t.elapsed();
  
  std::cout << "apply" << std::endl;

  for (int i=nkey-1;i>=0; --i){
    os.get(i, value);
    assert(3.14*i + 1 == value);
  }
  
  std::cout << "TEST: " << "ObjStore_" << STORAGE << " @ " << nkey << " KEYS uses " << std::endl
            << load << ",\t" << set << ",\t" << get << ",\t" << apply << " \tseconds."
            << std::endl << std::endl;
}

template<hazy::sman::StorageType STORAGE>
void test_BufferedObjStore(int nkey = 100000, int nbuffer = 10){
  
  hazy::utils::Timer t;
  double load, set, get, apply;
  
  hazy::sman::BufferedObjStore<double, STORAGE, hazy::sman::PROPERTY_NIL> os(nbuffer);
  
  t.restart();
  for (int i=0;i<nkey; ++i){
    os.load(i, i);
  }
  load = t.elapsed();
  std::cout << "loaded" << std::endl;
    
  t.restart();
  for (int i=0;i<nkey; ++i){
    os.set(i, i * 3.14);
  }
  set = t.elapsed();
  std::cout << "set" << std::endl;
    
  t.restart();
  double value;
  for (int i=nkey-1;i>=0; --i){
    os.get(i, value);
    //if(3.14*i != value){
    //  std::cout << i << " " << (3.14*i) << " " << value << std::endl;
    //}
    assert(3.14*i == value);
    value = 5;
    os.get(i, value);
    assert(3.14*i == value);
  }
  get = t.elapsed();
  std::cout << "get" << std::endl;
    
  t.restart();
  for (int i=nkey-1;i>=0; --i){
    os.apply(i, func_pp);
  }
  apply = t.elapsed();
  
  for (int i=nkey-1;i>=0; --i){
    os.get(i, value);
    assert(3.14*i + 1 == value);
  }
  
  std::cout << "TEST: " << "ObjStore_" << STORAGE << " @ " << nkey << " KEYS uses " << std::endl
  << load << ",\t" << set << ",\t" << get << ",\t" << apply << " \tseconds."
  << std::endl ;
  
  std::cout << "TEST: " << "get: " << os.hitget << "/" << os.nget << ", " 
            << "set: " << os.hitset << "/" << os.nset << ", "
            << "load: " << os.hitload << "/" << os.nload << ", "
            << "flush: " << os.nflush << std::endl << std::endl;
}

#include "test/test_parallel.h"

int main(int argc, const char * argv[])
{

  //hazy::sman::ObjStore<int, hazy::sman::STORAGE_MM> objstore;
  
  int scale = 100000;
  int nbuffer = 100;
  
  //test_ObjStore_MM_NAIVE(scale);
  
  //test_ObjStore<hazy::sman::STORAGE_JHASH>(scale);

  /*
  hazy::sman::ObjStore<double, hazy::sman::STORAGE_JHASH> objstore;
  double value;
  objstore.load(0, 0);
  objstore.load(1, 1);
  objstore.get(0, value);
  objstore.set(0, 0);
  objstore.get(1, value);
  objstore.set(1, 4.14);
  */
  
  
  //test_parallel_read<hazy::sman::STORAGE_JHASH>(scale, nbuffer, 1);
  //test_parallel_read<hazy::sman::STORAGE_JHASH>(scale, nbuffer, 2);
  //test_parallel_read<hazy::sman::STORAGE_JHASH>(scale, nbuffer, 3);
  //test_parallel_read<hazy::sman::STORAGE_JHASH>(scale, nbuffer, 4);
  test_parallel_read<hazy::sman::STORAGE_JHASH, hazy::sman::JAVAHASH_MM>(scale, nbuffer, 8);
  
  
  /*
  test_parallel_read<hazy::sman::STORAGE_JHASH>(scale, nbuffer, 1);
  test_parallel_read<hazy::sman::STORAGE_JHASH>(scale, nbuffer, 2);
  test_parallel_read<hazy::sman::STORAGE_JHASH>(scale, nbuffer, 3);
  test_parallel_read<hazy::sman::STORAGE_JHASH>(scale, nbuffer, 4);
  test_parallel_read<hazy::sman::STORAGE_JHASH>(scale, nbuffer, 8);
  */
  
  //test_parallel_read<hazy::sman::STORAGE_MM>(scale, nbuffer, 1);
  //test_parallel_read<hazy::sman::STORAGE_MM>(scale, nbuffer, 2);
  //test_parallel_read<hazy::sman::STORAGE_MM>(scale, nbuffer, 3);
  //test_parallel_read<hazy::sman::STORAGE_MM>(scale, nbuffer, 4);
  //test_parallel_read<hazy::sman::STORAGE_MM>(scale, nbuffer, 8);
  
  //test_ObjStore_MM_NAIVE(scale);
  //test_BufferedObjStore<hazy::sman::STORAGE_MM>(scale, nbuffer);
  //test_BufferedObjStore<hazy::sman::STORAGE_MM>(scale, 100000);
  
  //test_ObjStore_MM_NAIVE(scale);
  //test_ObjStore<hazy::sman::STORAGE_MM>(scale);
  //test_ObjStore<hazy::sman::STORAGE_FILE>(scale);
  
  return 0;
}

