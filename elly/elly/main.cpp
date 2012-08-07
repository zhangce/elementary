//
//  main.cpp
//  elly
//
//  Created by Ce Zhang on 7/4/12.
//  Copyright (c) 2012 University of Wisconsin-Madison. All rights reserved.
//

#include "utils/Common.h"

#include "utils/OptionParser.h"

#include "inf/Inf_marginal.h"

#include "utils/FactorFileParser.h"

#include "../../storageman/storageman/Buffer_mm.h"
#include "../../storageman/storageman/KeyValue_fl.h"
#include "../../storageman/storageman/KeyValue_vl.h"

#include "mat/Materialization_lazy.h"
#include "factors/factor_inits.h"

#include "factors/factor_register.h"

#include "alg/GibbsSampler.h"

#include <pthread.h>


namespace mia{
namespace elly{
    
    class SampleTask{
    public:
        int vid;
        mia::elly::mat::Materialization_lazy * mat;
        bool isShuffle;
        int * nchange;
    };
    
    void* mapper_sample(void* sampleTask){
        
        //return NULL;
        
        SampleTask* task = (SampleTask*) sampleTask;
        
        mia::elly::SampleInput sampleInput;
        task->mat->retrieve(task->vid, sampleInput);
        
        //sampleInput.print();
                
        int rs;
        if(task->isShuffle == false){
            rs = mia::elly::alg::GibbsSampling(sampleInput);
        }else{
            rs = mia::elly::alg::Shuffle(sampleInput);
        }
        
        if(rs != sampleInput.vvalue){
            (*task->nchange) ++;
        }
        
        task->mat->update(sampleInput, rs);
        
        return NULL;
        
    }
    
        
    class Elly{
    public:
        
        
        int nepoch;
        
        void generate_tasks_and_map(mia::elly::mat::Materialization_lazy * mat){
            
            mia::elly::utils::Timer timer;
            
            int nchange = 0;
            
            int nthreads = config.sys_nthreads;
            int nvariables = mat->getNVariable();
            
            mia::elly::utils::log() << ">> EPOCH #" << nepoch << ". Parallelizing inference using " << nthreads << " threads..." << std::endl;
            
            int nchunk = 0;
            
            bool isShuffle = (nepoch == 0);
            
            mia::elly::utils::log() << "  | isShuffle = " << isShuffle << std::endl;
            
            pthread_t* threads = new pthread_t[nthreads];
            
            for(int nv=0; nv<nvariables; nv+=nthreads){
                
                if(nchunk % 100000 == 0){
                    mia::elly::utils::log() << "  | Chunk #" << nchunk << ": VID in { ";
                }
                
                int nexecutor = 0;
                for(int nv2=nv; nv2<nvariables && nv2-nv<nthreads; nv2++){
                    
                    if(nchunk % 100000 == 0){
                        mia::elly::utils::log() << nv2 << " ";
                    }
                    
                    SampleTask task;
                    task.vid = nv2;
                    task.mat = mat;
                    task.isShuffle = isShuffle;
                    task.nchange = &nchange;
                    
                    ////////// STUPID: FIX ME ///////////
                    //mapper_sample(&task);
                    //pthread_create(&threads[nexecutor], 0, mapper_sample, &task);
                    mapper_sample(&task);
                    
                    nexecutor ++;
                    
                }
                
                if(nchunk % 100000 == 0){
                    mia::elly::utils::log() << "}" << std::endl;
                }
                
                //for (int i = 0; i < nexecutor; i++) {
                //
                //    pthread_join(threads[i], 0);
                //}
                
                nchunk ++;
            }
                    
            mia::elly::utils::log() << "  # change = " << nchange << std::endl;
            
            mia::elly::utils::log() << "  elapsed = " << timer.elapsed() << " seconds." << std::endl;
            
        }
       
        mia::elly::utils::Config config;
        
        Elly(mia::elly::utils::Config &_config){
            config = _config;
        }
        
        void map(){
            
        }
        
        void run(){
            
            mia::elly::utils::FactorFileParser fp(config.rt_input, config);
            fp.parse();
            
            mia::elly::mat::Materialization_lazy mat(&fp);
            mat.materialize();
            
            
            if(config.rt_mode.compare("map") == 0){
                
            }

            if(config.rt_mode.compare("marginal") == 0){
            
                nepoch = 0;
                
                for(nepoch; nepoch < 100; nepoch ++){
                    
                    generate_tasks_and_map(&mat);
                
                    char outputfile[1000];
                    sprintf(outputfile, "%s-%d.txt", config.rt_output.c_str(), nepoch);
                
                    mia::elly::utils::log() << ">> Dumping result to " << outputfile << std::endl;
                    
                    std::ofstream fout(outputfile);
                    for(int v=0;v<mat.parserrs->va.nvariable;v++){
                        fout << v << "\t" <<mat.parserrs->va.lookup(v) << std::endl;
                    }
                    fout.close();
                    
                }
                        
            }

            if(config.rt_mode.compare("mle") == 0){
                
            }

            if(config.rt_mode.compare("learn") == 0){
                
            }

            
        }
        
        ~Elly(){}
    };
    
}
}

/*
void test_kv_intkey(){
    elly::kv::KV_Intkey<int> kvrel(10,10000);
    for(int64_t i=0;i<10;i++){
        int vv[1000];
        for(int64_t j=0;j<1000;j++){
            vv[j] = i+j;
        }
        kvrel.push(i, vv, 1000);
    }
    
    std::cout << "push finished!" << std::endl;
    
    for(int64_t i=0;i<10;i++){
        
        int* values;
        int nvar;
        kvrel.lookup(i, values, nvar);
        std::cout << "key " << i << ": " << nvar << " vars, " << values[0] << " " << values[1] << std::endl;
    }

}*/


int main(int argc, const char * argv[])
{

    mia::elly::utils::Config config;
    
    mia::elly::utils::log() << "##### Elly " << 
            config.version_number << 
            " #####" << std::endl;
    
    int rs_parse_options = mia::elly::utils::parse_options(config, argc, argv);

    if (rs_parse_options != 0){
        return 0;
    }
    
    mia::elly::factors::register_potentials();
    mia::elly::factors::register_updates();
    
    mia::elly::Elly elly_instance(config);
    elly_instance.run();
    
    
    
    
    
    /*
    mia::sm::KeyValue_vl<mia::sm::Buffer_mm> kv;
    mia::sm::IntsBlock block;
    block.append<int>(1);
    kv.set(0, block);
    block.content[0] = 2;
    kv.set(1, block);
    block.content[0] = 3;
    kv.set(2, block);
    
    std::cout << 0 << " = " << kv.get(0).get<int>(0) << std::endl;
    std::cout << 1 << " = " << kv.get(1).get<int>(0) << std::endl;
    std::cout << 2 << " = " << kv.get(2).get<int>(0) << std::endl;
    */
    
//    test_kv_intkey();
    
    /*
    
    elly::mem::buffer_mm bmm;
    int a=5;
    bmm.push_back<int>(a);
    a=10;
    bmm.push_back<int>(a);
    a=100;
    bmm.push_back<int>(a);
    a=1000;
    bmm.push_back<int>(200);
    
    
    for(int i=0;i<100000;i++){
        bmm.push_back<int>(i);
    }
    
    std::cout << *bmm.get<int>(0) << std::endl;
    std::cout << *bmm.get<int>(4) << std::endl;
    std::cout << *bmm.get<int>(8) << std::endl;
    std::cout << *bmm.get<int>(12) << std::endl;
    
    std::cout << *bmm.uniform_get<int>(0) << std::endl;
    std::cout << *bmm.uniform_get<int>(1) << std::endl;
    std::cout << *bmm.uniform_get<int>(2) << std::endl;
    std::cout << *bmm.uniform_get<int>(3) << std::endl;
    */
    
    
    return 0;
}












