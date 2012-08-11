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

#include "utils/Timer.h"

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
        int lower;
        int upper;
        int step;
        mia::elly::mat::Materialization_lazy * mat;
        bool isShuffle;
        int * nchange;
        int * naccept;
        
        bool tally;
        bool train;
        
        double stepSize;
        
        double temparature;
        
        int thread_id;
        std::vector<double> * vector_pool;
    };
    
    void* mapper_sample(void* sampleTask){
        
        //return NULL;
        
        bool lock = false;
        bool is_log_system = false;
        bool has_linear_upper_bound = true;
        
        SampleTask* task = (SampleTask*) sampleTask;
        
        mia::elly::SampleInput sampleInput;
        
        for(int vid=task->lower;vid<task->upper;vid+=task->step){

            task->mat->retrieve(vid, sampleInput, task->train, lock);
            
            sampleInput.stepSize = task->stepSize;
            
            //sampleInput.print();
            
            int rs;
            if(task->isShuffle == true){
                rs = mia::elly::alg::Shuffle(sampleInput);
            }else{
                rs = mia::elly::alg::GibbsSampling(sampleInput, task->thread_id, task->vector_pool, is_log_system, has_linear_upper_bound);
            }
            
            if(rs != sampleInput.vvalue){
                (*task->nchange) ++;
            }

            if(task->isShuffle == true){
                
                if(rs != sampleInput.vvalue){
                    (*task->naccept) ++;
                }
                
                task->mat->update(sampleInput, rs, false, lock);
            }else{
                
                if(task->temparature == -1){
                    
                    task->mat->update(sampleInput, rs, task->tally, lock);
                    
                }else{
                    
                    double log_ratio = 0;
                    if(is_log_system){
                        log_ratio = sampleInput.log_improve_ratio;
                    }else{
                        log_ratio = log(sampleInput.log_improve_ratio);
                    }
                    
                    if(log_ratio > 0){
                        if(rs != sampleInput.vvalue){
                            (*task->naccept) ++;
                        }
                        task->mat->update(sampleInput, rs, false, lock);
                    }else{
                    
                        double p_accept = exp(
                            log_ratio/task->temparature
                                              );
                        
                        double random = drand48();
                        if(random < p_accept){
                            if(rs != sampleInput.vvalue){
                                (*task->naccept) ++;
                            }
                            task->mat->update(sampleInput, rs, false, lock);
                        }else{
                            task->mat->update(sampleInput, sampleInput.vvalue, false, lock);
                        }
                    
                    }
                    
                }
                
            }
            
        }
        
        return NULL;
        
    }
    
        
    class Elly{
    public:
        
        int nepoch;
        
        std::vector<double> * vector_pool;
        
        void generate_tasks_and_map(mia::elly::mat::Materialization_lazy * mat, double temparature = -1, bool tally = false, bool train = false, double stepSize = -1){
            
            mia::elly::utils::Timer timer;
            
            int nchange = 0;
            int naccept = 0;
            
            int nthreads = config->sys_nthreads;
            int nvariables = mat->getNVariable();
            
            if(temparature == -1){
             
                if(stepSize == -1){
                    mia::elly::utils::log() << ">> Gibbs Sampling EPOCH #" << nepoch << ". [# Threads = " << nthreads << "]" << std::endl;
                }else{
                    mia::elly::utils::log() << ">> Weight Learning EPOCH #" << nepoch << ". [# Threads = " << nthreads << "] [Step = " << stepSize << "]" << std::endl;
                    
                }
            }else{
                mia::elly::utils::log() << ">> Simulated Annealing EPOCH #" << nepoch << ". [# Threads = " << nthreads << "] [Temparature = " << temparature << "]" << std::endl;
            }
            
            bool isShuffle = (nepoch == 0);
            
            mia::elly::utils::log() << "  | isShuffle = " << isShuffle << std::endl;
            
            pthread_t* threads = new pthread_t[nthreads];
            
            for(int nv=0; nv<nthreads; nv++){
                                    
                SampleTask * task = new SampleTask;
                task->lower = nv;
                task->upper = nvariables;
                task->step = nthreads;
                task->mat = mat;
                task->isShuffle = isShuffle;
                task->nchange = &nchange;
                task->naccept = &naccept;
                
                task->tally = tally;
                task->train = train;
                
                task->stepSize = stepSize;
                
                task->temparature = temparature;
                
                task->thread_id = nv;
                task->vector_pool = vector_pool;
                    
                pthread_create(&threads[nv], 0, mapper_sample, task);

            }
            
            for (int i = 0; i < nthreads; i++) {
            
                pthread_join(threads[i], 0);
            }
                    
            mia::elly::utils::log() << "  # change = " << nchange << std::endl;
            
            if(temparature != -1){
                mia::elly::utils::log() << "  # accept = " << naccept << std::endl;
            }
            
            mia::elly::utils::log() << "  elapsed = " << timer.elapsed() << " seconds." << std::endl;
            
        }
       
        mia::elly::utils::Config * config;
        
        Elly(mia::elly::utils::Config * _config){
            config = _config;
            vector_pool = new std::vector<double>[config->sys_nthreads*3];
        }
        
        void map(){
            
        }
        
        double sa_temparature(int _nepoch){
                // todo: this is so ad hoc
            return 1.0 - 1.0*_nepoch/config->rt_nepoch;
        }
        
        void run(){
            
            
            mia::elly::utils::FactorFileParser fp(config->rt_input, config);
            fp.parse();
            
            mia::elly::mat::Materialization_lazy mat(&fp);
            mat.materialize();
            
            
            if(config->rt_mode.compare("map") == 0){
                
                mia::elly::utils::log() << ">> Running MAP inference..." << std::endl;
                
                for(nepoch = 0; nepoch < config->rt_nepoch; nepoch ++){
                    generate_tasks_and_map(&mat, sa_temparature(nepoch));
                }
                
                char outputfile[1000];
                sprintf(outputfile, "%s-map.txt", config->rt_output.c_str());
                
                mia::elly::utils::log() << ">> Dumping result to " << outputfile << std::endl;
                
                std::ofstream fout(outputfile);
                for(int v=0;v<mat.parserrs->va.nvariable;v++){
                    fout << v << "\t" <<mat.parserrs->va.lookup(v) << std::endl;
                }
                fout.close();
                
                if(config->io_ismln == true){
                    std::string cmd = "python " + config->io_mln + " " + config->rt_output + "-map.txt" + " " + config->rt_output;
                    mia::elly::utils::log() << ">> Run " << cmd << std::endl;
                    system(cmd.c_str());
                }
                
            }
            
            if(config->rt_mode.compare("sample") == 0){
                
                mia::elly::utils::log() << ">> Running Gibbs sampling to get one sample..." << std::endl;
                
                for(nepoch = 0; nepoch < config->rt_nepoch; nepoch ++){
                    generate_tasks_and_map(&mat);
                }
                
                char outputfile[1000];
                sprintf(outputfile, "%s-sample.txt", config->rt_output.c_str());
                
                mia::elly::utils::log() << ">> Dumping result to " << outputfile << std::endl;
                
                std::ofstream fout(outputfile);
                for(int v=0;v<mat.parserrs->va.nvariable;v++){
                    fout << v << "\t" <<mat.parserrs->va.lookup(v) << std::endl;
                }
                fout.close();
                
                if(config->io_ismln == true){
                    std::string cmd = "python " + config->io_mln + " " + config->rt_output + "-sample.txt" + " " + config->rt_output;
                    mia::elly::utils::log() << ">> Run " << cmd << std::endl;
                    system(cmd.c_str());
                }
                
            }

            if(config->rt_mode.compare("marginal") == 0){
                
                mia::elly::utils::log() << ">> Running marginal inference..." << std::endl;
                
                for(nepoch = 0; nepoch < config->rt_nepoch; nepoch ++){
                    
                    generate_tasks_and_map(&mat, -1, true);
                
                }
                
                
                char outputfile[1000];
                sprintf(outputfile, "%s-marginal.txt", config->rt_output.c_str());
                
                mia::elly::utils::log() << ">> Dumping result to " << outputfile << std::endl;
                
                std::ofstream fout(outputfile);
                for(int v=0;v<mat.parserrs->va.nvariable;v++){
                    int sum = 0;
                    
                    mia::sm::IntsBlock block = mat.parserrs->vt.lookup(v);
                    for(int i=0;i<block.size;i++){
                        sum += block.content[i];
                    }
                    
                    for(int i=0;i<block.size;i++){
                    
                        if(block.content[i] == 0){
                            continue;
                        }
                        
                        fout << v << "\t" << i << "\t" << (1.0*block.content[i]/sum) << std::endl;
                    
                    }
                    
                }
                fout.close();
                
                if(config->io_ismln == true){
                    std::string cmd = "python " + config->io_mln + " " + config->rt_output + "-marginal.txt" + " " + config->rt_output;
                    mia::elly::utils::log() << ">> Run " << cmd << std::endl;
                    system(cmd.c_str());
                }
                 
                        
            }

            if(config->rt_mode.compare("learn") == 0){
                                
                mia::elly::utils::log() << ">> Learning weight..." << std::endl;
                
                double initstep = config->rt_learn_initstep;
                double decay = config->rt_learn_decay;
                
                double step = initstep;
                
                for(nepoch = 0; nepoch < config->rt_nepoch; nepoch ++){
                    
                    generate_tasks_and_map(&mat, -1, false, true, step);
                    
                    step = step * decay;
                    
                }
                
                mia::elly::utils::log() << ">> Dumping new weights..." << std::endl;
                for(int i=0;i<mat.parserrs->crs.size();i++){
                   
                    if(mat.parserrs->crs[i]->mapfilename.compare("") != 0){
                        elly::utils::log() << "  | Factor [" << mat.parserrs->crs[i]->factor_name << "]: " <<
                        mat.parserrs->crs[i]->mapfilename
                        << std::endl;
                        mat.parserrs->crs[i]->dump_weights();
                    }
                }
                
                if(config->io_ismln == true){
                    std::string cmd = "python " + config->io_mln + " " + "~~" + " " + config->rt_output;
                    mia::elly::utils::log() << ">> Run " << cmd << std::endl;
                    system(cmd.c_str());
                }
            }
            
            if(config->rt_mode.compare("ds") == 0){
                
                
                
                mia::elly::utils::log() << ">> Distant Supervision..." << std::endl;
                
                config->rt_mode = "learn";
                
                mia::elly::utils::log() << ">> Learning weight..." << std::endl;
                
                double initstep = config->rt_learn_initstep;
                double decay = config->rt_learn_decay;
                
                double step = initstep;
                
                for(nepoch = 0; nepoch < config->rt_nepoch; nepoch ++){
                    
                    generate_tasks_and_map(&mat, -1, false, true, step);
                    
                    step = step * decay;
                    
                }
                
                mia::elly::utils::log() << ">> Dumping new weights..." << std::endl;
                for(int i=0;i<mat.parserrs->crs.size();i++){
                    
                    if(mat.parserrs->crs[i]->mapfilename.compare("") != 0){
                        elly::utils::log() << "  | Factor [" << mat.parserrs->crs[i]->factor_name << "]: " <<
                        mat.parserrs->crs[i]->mapfilename
                        << std::endl;
                        mat.parserrs->crs[i]->dump_weights();
                    }
                }
                
                config->rt_mode = "marginal";
                
                mia::elly::utils::log() << ">> Running marginal inference..." << std::endl;
                
                for(nepoch = 0; nepoch < config->rt_nepoch; nepoch ++){
                    
                    generate_tasks_and_map(&mat, -1, true);
                    
                }
                
                
                char outputfile[1000];
                sprintf(outputfile, "%s-marginal.txt", config->rt_output.c_str());
                
                mia::elly::utils::log() << ">> Dumping result to " << outputfile << std::endl;
                
                std::ofstream fout(outputfile);
                for(int v=0;v<mat.parserrs->va.nvariable;v++){
                    int sum = 0;
                    
                    mia::sm::IntsBlock block = mat.parserrs->vt.lookup(v);
                    for(int i=0;i<block.size;i++){
                        sum += block.content[i];
                    }
                    
                    for(int i=0;i<block.size;i++){
                        
                        if(block.content[i] == 0){
                            continue;
                        }
                        
                        fout << v << "\t" << i << "\t" << (1.0*block.content[i]/sum) << std::endl;
                        
                    }
                    
                }
                fout.close();

                config->rt_mode = "ds";
                
                if(config->io_ismln == true){
                    std::string cmd = "python " + config->io_mln + " " + config->rt_output + "-marginal.txt" + " " + config->rt_output;
                    mia::elly::utils::log() << ">> Run " << cmd << std::endl;
                    system(cmd.c_str());
                }
                
                
                
                
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

    mia::elly::utils::Timer timer;
    
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
        
    mia::elly::Elly elly_instance(&config);
    elly_instance.run();
    
    mia::elly::utils::log() << ">> Elly exited. [" << timer.elapsed() << " seconds]" << std::endl;
    
    return 0;
}












