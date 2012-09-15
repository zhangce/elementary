//
//  ObjectStore_PG.h
//  SMan
//
//  Created by Ce Zhang on 9/10/12.
//  Copyright (c) 2012 HazyResearch. All rights reserved.
//

#ifndef SMan_ObjectStore_PG_h
#define SMan_ObjectStore_PG_h

/*
#include "../common/Common.h"

namespace hazy{
  namespace sman{
        
    template<class VALUE, PropertyType PROPERTY>
    class ObjStore<VALUE, STORAGE_JHASH, PROPERTY>{
      
    private:
      
      //JavaVM *jvm;
      JNIEnv *env;
      JavaVMInitArgs vm_args;
      JavaVMOption* options;
      //jclass cls;
      //jclass globid;
      
      //jmethodID id_get;
      //jmethodID id_set;
      //jmethodID id_load;
      //jmethodID id_init;
      //jmethodID id_reg;
      
      jint connectorid;
      
    public:
      
      ~ObjStore(){
        
      }
      
      ObjStore(){
        
        if(PROPERTY == JAVAHASH_MM){
          std::cout << "INFO: " << "Use specification STORAGE_JHASH @ HashMapConnector." << std::endl;
        }else if(PROPERTY == JAVAHASH_ACCU){
          std::cout << "INFO: " << "Use specification STORAGE_JHASH @ AccumuloConnector." << std::endl;
        }
        
        if(jvm == NULL){
          
          //std::cout << "INIT JAVA" << std::endl;
          
          options = new JavaVMOption[1];
        
          options[0].optionString =
              "-Djava.class.path=/Users/czhang/Desktop/StorageManager/SMan/java/bin:/Users/czhang/Desktop/Codes/AccumuloConnector/lib/postgresql-8.4-701.jdbc4.jar:/Users/czhang/Desktop/StorageManager/SMan/java/lib/accumulo-core-1.4.1-javadoc.jar:/Users/czhang/Desktop/StorageManager/SMan/java/lib/accumulo-core-1.4.1-sources.jar:/Users/czhang/Desktop/StorageManager/SMan/java/lib/accumulo-core-1.4.1.jar:/Users/czhang/Desktop/StorageManager/SMan/java/lib/accumulo-server-1.4.1-javadoc.jar:/Users/czhang/Desktop/StorageManager/SMan/java/lib/accumulo-server-1.4.1-sources.jar:/Users/czhang/Desktop/StorageManager/SMan/java/lib/accumulo-server-1.4.1.jar:/Users/czhang/Desktop/StorageManager/SMan/java/lib/accumulo-start-1.4.1-javadoc.jar:/Users/czhang/Desktop/StorageManager/SMan/java/lib/accumulo-start-1.4.1-sources.jar:/Users/czhang/Desktop/StorageManager/SMan/java/lib/accumulo-start-1.4.1.jar:/Users/czhang/Desktop/StorageManager/SMan/java/lib/cloudtrace-1.4.1-javadoc.jar:/Users/czhang/Desktop/StorageManager/SMan/java/lib/cloudtrace-1.4.1-sources.jar:/Users/czhang/Desktop/StorageManager/SMan/java/lib/cloudtrace-1.4.1.jar:/Users/czhang/Desktop/StorageManager/SMan/java/lib/commons-collections-3.2.jar:/Users/czhang/Desktop/StorageManager/SMan/java/lib/commons-configuration-1.5.jar:/Users/czhang/Desktop/StorageManager/SMan/java/lib/commons-io-1.4.jar:/Users/czhang/Desktop/StorageManager/SMan/java/lib/commons-jci-core-1.0.jar:/Users/czhang/Desktop/StorageManager/SMan/java/lib/commons-jci-fam-1.0.jar:/Users/czhang/Desktop/StorageManager/SMan/java/lib/commons-lang-2.4.jar:/Users/czhang/Desktop/StorageManager/SMan/java/lib/commons-logging-1.0.4.jar:/Users/czhang/Desktop/StorageManager/SMan/java/lib/commons-logging-api-1.0.4.jar:/Users/czhang/Desktop/StorageManager/SMan/java/lib/examples-simple-1.4.1-javadoc.jar:/Users/czhang/Desktop/StorageManager/SMan/java/lib/examples-simple-1.4.1-sources.jar:/Users/czhang/Desktop/StorageManager/SMan/java/lib/examples-simple-1.4.1.jar:/Users/czhang/Desktop/StorageManager/SMan/java/lib/hadoop-ant-1.0.3.jar:/Users/czhang/Desktop/StorageManager/SMan/java/lib/hadoop-client-1.0.3.jar:/Users/czhang/Desktop/StorageManager/SMan/java/lib/hadoop-core-1.0.3.jar:/Users/czhang/Desktop/StorageManager/SMan/java/lib/hadoop-examples-1.0.3.jar:/Users/czhang/Desktop/StorageManager/SMan/java/lib/hadoop-minicluster-1.0.3.jar:/Users/czhang/Desktop/StorageManager/SMan/java/lib/hadoop-test-1.0.3.jar:/Users/czhang/Desktop/StorageManager/SMan/java/lib/hadoop-tools-1.0.3.jar:/Users/czhang/Desktop/StorageManager/SMan/java/lib/jline-0.9.94.jar:/Users/czhang/Desktop/StorageManager/SMan/java/lib/libthrift-0.6.1.jar:/Users/czhang/Desktop/StorageManager/SMan/java/lib/log4j-1.2.16.jar:/Users/czhang/Desktop/StorageManager/SMan/java/lib/postgresql-8.4-701.jdbc4.jar:/Users/czhang/Desktop/StorageManager/SMan/java/lib/slf4j-api-1.6.1.jar:/Users/czhang/Desktop/StorageManager/SMan/java/lib/slf4j-log4j12-1.6.1.jar:/Users/czhang/Desktop/StorageManager/SMan/java/lib/wikisearch-ingest-1.4.1-javadoc.jar:/Users/czhang/Desktop/StorageManager/SMan/java/lib/wikisearch-query-1.4.1-javadoc.jar:/Users/czhang/Desktop/StorageManager/SMan/java/lib/zookeeper-3.4.3.jar";
          vm_args.version = JNI_VERSION_1_6;
          vm_args.nOptions = 1;
          vm_args.options = options;
          vm_args.ignoreUnrecognized = false;
        
          if(JNI_CreateJavaVM(&jvm, (void**)&env, &vm_args) <0){
            std::cout << "<try to reuse>" << std::endl;
            jsize b;
            JNI_GetCreatedJavaVMs(&jvm, 1, &b);
            std::cout << "</try to reuse>" << std::endl;
          }
          delete options;
        
          cls = env->FindClass("JavaConnector");
        
          id_reg = env->GetStaticMethodID(cls, "push_new", "(Ljava/lang/String;)I");
          id_init = env->GetStaticMethodID(cls, "init", "(ILjava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");
          id_get = env->GetStaticMethodID(cls, "get", "(II)Ljava/lang/Object;");
          id_set = env->GetStaticMethodID(cls, "set", "(II[B)V");
          id_load = env->GetStaticMethodID(cls, "load", "(II[B)V");
          
        }
        
        jvm->AttachCurrentThread((void**)&env, NULL);
        if(PROPERTY == JAVAHASH_MM){
          connectorid = env->CallStaticIntMethod(cls, id_reg, env->NewStringUTF("HashMapConnector"));
        }else if(PROPERTY == JAVAHASH_ACCU){
          connectorid = env->CallStaticIntMethod(cls, id_reg, env->NewStringUTF("AccumuloConnector"));
        }
        
        std::cout << "## registered id = " << connectorid << std::endl;
        
        jobjectArray ret;
        ret = (jobjectArray)env->NewObjectArray(4,
                                                env->FindClass("java/lang/String"),
                                                env->NewStringUTF(""));
        
        char aa[1000];
        sprintf(aa, "i2/testtable%d", connectorid);
        std::cout << "table -- " << aa << std::endl;
        
        env->CallStaticVoidMethod(cls, id_init, connectorid,
                                  env->NewStringUTF("localhost:2181"),
                                  env->NewStringUTF("root"),
                                  env->NewStringUTF("bB19871121"),
                                  env->NewStringUTF(aa));
      }
      
      StatusType get(int64_t key, VALUE & value){
        
        JNIEnv *env2;
        jvm->AttachCurrentThread((void**)&env2, NULL);
        
        jbyteArray barr = (jbyteArray) env2->CallObjectMethod(cls, id_get, connectorid, key);
        
        VALUE tmp;
        
        env2->GetByteArrayRegion(barr, 0, sizeof(VALUE), (jbyte*) &tmp);
        
        value = tmp;
        
        env2->DeleteLocalRef(barr);
        
        return STATUS_SUCCESS;
      }
      
      StatusType set(int64_t key, const VALUE & value){
        
        JNIEnv *env2;
        jvm->AttachCurrentThread((void**)&env2, NULL);
        
        jbyteArray serialized = env2->NewByteArray(sizeof(VALUE));
        
        env2->SetByteArrayRegion(serialized, 0, sizeof(VALUE), (jbyte*) &value);
        
        env2->CallStaticVoidMethod(cls, id_set, connectorid, key, serialized);
        
        env2->DeleteLocalRef(serialized);
        
        
        return STATUS_SUCCESS;
      }
      
      StatusType load(int64_t key, const VALUE & value){
        
        JNIEnv *env2;
        jvm->AttachCurrentThread((void**)&env2, NULL);
        
        jbyteArray serialized = env2->NewByteArray(sizeof(VALUE));
        
        env2->SetByteArrayRegion(serialized, 0, sizeof(VALUE), (jbyte*) &value);
        
        //jsize alen = env->GetArrayLength(serialized);
        
        env2->CallStaticVoidMethod(cls, id_load, connectorid, key, serialized);
        
        env2->DeleteLocalRef(serialized);
        
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
*/

#endif
