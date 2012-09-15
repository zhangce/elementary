//
//  test_parallel.h
//  SMan
//
//  Created by Ce Zhang on 9/9/12.
//  Copyright (c) 2012 HazyResearch. All rights reserved.
//

#ifndef SMan_test_parallel_h
#define SMan_test_parallel_h



#include "../common/Common.h"
#include "../objstore/ObjStore.h"

#include "../objstore/ObjectStore_MM.h"
#include "../objstore/ObjectStore_FILE.h"

#include "../objstore/BufferedObjStore.h"


inline void func_pp2(double & value){
  ++value;
}

template<hazy::sman::StorageType STORAGE, hazy::sman::PropertyType PROPERTY>
struct read_task{
  int nkey;
  int threadid;
  
  hazy::sman::BufferedObjStore<double, STORAGE, PROPERTY>* os;
};

template<hazy::sman::StorageType STORAGE, hazy::sman::PropertyType PROPERTY>
void * task_read(void * _task){
  
  read_task<STORAGE, PROPERTY> * task = (read_task<STORAGE, PROPERTY>*) _task;
  
  std::cout << "task " << task->threadid << std::endl;
  
  double value;
  for (int i=task->nkey-1;i>=0; --i){
    
    //task->os->set(i, 3.14*i);
    
    task->os->get(i, value);
    
    //if(3.14*i != value){
    //  std::cout << i << " " << (3.14*i) << " " << value << std::endl;
    //}
    
    //if(3.14*i != value){
    //  std::cout << i << " " << (3.14*i) << " " << value << std::endl;
    //}
    
    assert(3.14*i == value);
    value = 5;
    
    //task->os->apply(i, func_pp2);
    
    task->os->get(i, value);
    
    //if(3.14*i + 1 != value){
    //  std::cout << i << " " << (3.14*i) << " " << value << std::endl;
    //}
    
    assert(3.14*i  == value);
  }

  return NULL;
}

template<hazy::sman::StorageType STORAGE, hazy::sman::PropertyType PROPERTY>
void test_parallel_read(int nkey=10000, int nbuffer=10, int nthread=4){
  
  hazy::utils::Timer t;
  double load, set, get, apply;
  
  hazy::sman::BufferedObjStore<double, STORAGE, PROPERTY> os(nbuffer);
  
  t.restart();
  for (int i=0;i<nkey; ++i){
    os.load(i, i);
  }
  load = t.elapsed();
  
  t.restart();
  for (int i=0;i<nkey; ++i){
    os.set(i, i * 3.14);
  }
  set = t.elapsed();
  
  t.restart();
  pthread_t * threads = new pthread_t[nthread];
  read_task<STORAGE, PROPERTY> * tasks = new read_task<STORAGE, PROPERTY>[nthread];
  for(int i=0;i<nthread;i++){
    tasks[i].nkey = nkey;
    tasks[i].threadid = i;
    tasks[i].os = &os;
    
    pthread_create(&threads[i], NULL, task_read<STORAGE, PROPERTY>, (void*) &tasks[i]);
    
  }
  
  void * status;
  for(int i=0;i<nthread;i++){
    pthread_join(threads[i], &status);
  }
  get = t.elapsed();
  
  t.restart();
  //for (int i=nkey-1;i>=0; --i){
  //  os.apply(i, func_pp);
  //}
  apply = t.elapsed();
  
  //double value;
  //for (int i=nkey-1;i>=0; --i){
  //  os.get(i, value);
  //  //std::cout << (3.14*i) << " " << value << std::endl;
  //  //std::cout << (nthread) << std::endl;
  //  if(3.14*i + (nthread) != value){
  //    std::cout << (3.14*i) << " " << value << std::endl;
  //    std::cout << (nthread) << std::endl;
  //  }
  //  assert(3.14*i + (nthread) == value);
  //}
  
  std::cout << "PTEST: " << "ObjStore_" << STORAGE << " @ " << nkey << " KEYS uses " << std::endl
  << load << ",\t" << set << ",\t" << get << ",\t" << apply << " \tseconds."
  << std::endl ;
  
  std::cout << "PTEST: " << "get: " << os.hitget << "/" << os.nget << ", "
  << "set: " << os.hitset << "/" << os.nset << ", "
  << "load: " << os.hitload << "/" << os.nload << ", "
  << "flush: " << os.nflush << std::endl << std::endl;
  
  
}


#endif
