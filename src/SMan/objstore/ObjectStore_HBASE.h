//
//  ObjectStore_HBASE.h
//  elly
//
//  Created by Ce Zhang on 9/12/12.
//  Copyright (c) 2012 University of Wisconsin-Madison. All rights reserved.
//

#ifndef SMan_ObjectStore_HBASE_h
#define SMan_ObjectStore_HBASE_h

#include <time.h>

#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <poll.h>

#include <iostream>

#include <boost/lexical_cast.hpp>
#include <protocol/TBinaryProtocol.h>
#include <transport/TSocket.h>
#include <transport/TTransportUtils.h>

#include "../gen-cpp/Hbase.h"

namespace hazy{
  namespace sman{
    
    template<class VALUE, PropertyType PROPERTY>
    class ObjStore<VALUE, STORAGE_HBASE, PROPERTY>{
      
    private:
      
      pthread_mutex_t* mutex;
      
      boost::shared_ptr<apache::thrift::transport::TTransport> socket;
      boost::shared_ptr<apache::thrift::transport::TTransport> transport;
      boost::shared_ptr<apache::thrift::protocol::TProtocol> protocol;
      apache::hadoop::hbase::thrift::HbaseClient * client;
      
      std::map<apache::hadoop::hbase::thrift::Text, apache::hadoop::hbase::thrift::Text> empty_attrs;
      
      std::string t;
      
      std::vector<std::string> columnNames;
      
      apache::hadoop::hbase::thrift::Mutation mutation;
      
    public:
      
      ~ObjStore(){
        
      }
      
      ObjStore() {
        
        hbaseid ++;
        
        std::cout << "~~~~~~~~" << hbaseid << std::endl;
        
        char tablename[1000];
        sprintf(tablename, "elly_testtable_%d_%d", hbaseid, time(NULL) );
        
        std::cout << "INFO: " << "Use specification STORAGE_HBASE." << std::endl;
        
        columnNames.push_back("key:");
        
        mutation.column = "key:key";
        
        mutex = new pthread_mutex_t;
        pthread_mutex_init(mutex, NULL);
        
        socket = boost::shared_ptr<apache::thrift::transport::TTransport>(new apache::thrift::transport::TSocket("hazy-01.chtc.wisc.edu", 9090));
        transport = boost::shared_ptr<apache::thrift::transport::TTransport>(new apache::thrift::transport::TBufferedTransport(socket));
        protocol = boost::shared_ptr<apache::thrift::protocol::TProtocol>(new apache::thrift::protocol::TBinaryProtocol(transport));
        
        client = new apache::hadoop::hbase::thrift::HbaseClient(protocol);
        
        transport->open();
        
        t = std::string(tablename);
        std::vector<apache::hadoop::hbase::thrift::ColumnDescriptor> columns;
        columns.push_back(apache::hadoop::hbase::thrift::ColumnDescriptor());
        columns.back().name = "key:";
        columns.back().maxVersions = 1;
        
        std::cout << "droping table: " << t << std::endl;
        try {
          client->disableTable(t);
          client->deleteTable(t);
        }catch (const apache::hadoop::hbase::thrift::IOError &ioe){
          std::cerr << "WARN: " << ioe.message << std::endl;
        }
        
        std::cout << "creating table: " << t << std::endl;
        try{
          client->createTable(t, columns);
          //client->enableTable(t);
        } catch (const apache::hadoop::hbase::thrift::AlreadyExists &ae) {
          std::cerr << "WARN: " << ae.message << std::endl;
        }
        
      }
      
      StatusType get(int64_t key, VALUE & value){
        
        VALUE bbb = value;
        
        int64_t k = key;
        std::string kk((char*)&k, sizeof(int64_t));
        
        pthread_mutex_lock(mutex);
        int scanner = client->scannerOpenWithStop(t, kk, kk, columnNames, empty_attrs);
        pthread_mutex_unlock(mutex);
        
        
        while (true) {
          std::vector<apache::hadoop::hbase::thrift::TRowResult> value;
          
          pthread_mutex_lock(mutex);
          client->scannerGet(value, scanner);
          pthread_mutex_unlock(mutex);
          
          if (value.size() == 0)
            break;
          for (size_t i = 0; i < value.size(); i++) {
            
            for (std::map<std::string, apache::hadoop::hbase::thrift::TCell>::const_iterator it = value[i].columns.begin();
                 it != value[i].columns.end(); ++it) {
              
              it->second.value.copy((char*) &bbb, sizeof(VALUE));
              
              break;
              
            }
            
          }
          break;
        }
        
        value = bbb;
        
        
        return STATUS_SUCCESS;
      }
      
      StatusType set(int64_t key, const VALUE & value){
        
        std::vector<apache::hadoop::hbase::thrift::Mutation> mutations;
        mutations.push_back(apache::hadoop::hbase::thrift::Mutation());
        mutations.back().column = "key:key";
        
        VALUE v = value;
        std::string bb((char*)&v, sizeof(VALUE));
        mutations.back().value = bb;
        
        int64_t k = key;
        std::string kk((char*)&k, sizeof(int64_t));
        
        pthread_mutex_lock(mutex);
        client->mutateRow(t, kk, mutations, empty_attrs);
        pthread_mutex_unlock(mutex);
        
        return STATUS_SUCCESS;
      }
      
      StatusType load(int64_t key, const VALUE & value){
        
        
        std::vector<apache::hadoop::hbase::thrift::Mutation> mutations;
        mutations.push_back(apache::hadoop::hbase::thrift::Mutation());
        mutations.back().column = "key:key";
        
        VALUE v = value;
        std::string bb((char*)&v, sizeof(VALUE));
        mutations.back().value = bb;
        
        int64_t k = key;
        std::string kk((char*)&k, sizeof(int64_t));
        
        pthread_mutex_lock(mutex);
        client->mutateRow(t, kk, mutations, empty_attrs);
        pthread_mutex_unlock(mutex);
        
        return STATUS_SUCCESS;
      }
      
      StatusType apply(int64_t key, void (*func)(VALUE &)){
        
        VALUE v;
        
        this->get(key, v);
        
        func(v);
        
        this->set(key, v);
        
        return STATUS_SUCCESS;
      }
      
    };
    
    
  }
}




#endif
