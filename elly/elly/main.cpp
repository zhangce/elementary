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


namespace elly{
    
    class Elly{
    public:
        
        elly::utils::Config config;
        
        Elly(elly::utils::Config &_config){
            config = _config;
        }
        
        void run(){
            
            if(config.rt_mode.compare("map") == 0){
                
            }

            if(config.rt_mode.compare("marginal") == 0){
                elly::inf::Inf_marginal inf(config);
                inf.infer();
            }

            if(config.rt_mode.compare("mle") == 0){
                
            }

            if(config.rt_mode.compare("learn") == 0){
                
            }

            
        }
        
        ~Elly(){}
    };
    
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


#include "../../storageman/storageman/Buffer_mm.h"
#include "../../storageman/storageman/KeyValue_fl.h"
#include "../../storageman/storageman/KeyValue_vl.h"

int main(int argc, const char * argv[])
{

    elly::utils::Config config;
    
    elly::utils::log() << "##### Elly " << 
            config.version_number << 
            " #####" << std::endl;
    
    int rs_parse_options = elly::utils::parse_options(config, argc, argv);

    if (rs_parse_options != 0){
        return 0;
    }

    elly::utils::FactorFileParser fp(config.rt_input, config);
    fp.parse();
    

    
    elly::Elly elly_instance(config);
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












