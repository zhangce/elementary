//
//  FactorFileParser.h
//  elly
//
//  Created by Ce Zhang on 7/8/12.
//  Copyright (c) 2012 University of Wisconsin-Madison. All rights reserved.
//

#ifndef elly_FactorFileParser_h
#define elly_FactorFileParser_h

#include <vector>
#include "Common.h"

#include "../dstruct/StandardCorrelationRelation.h"
#include "../dstruct/VariableFactorRelation.h"
#include "../dstruct/VariableTallyRelation.h"
#include "../dstruct/VariableAssignmentRelation.h"
#include "../dstruct/AbstractCorrelationRelation.h"
#include "../dstruct/VariableTrainingRelation.h"

//#include "../../../storageman/storageman/Buffer_mmap.h"

#include <fstream>

//#include "/usr/local/include/boost/program_options.hpp"
//namespace po = boost::program_options;

#include "boost/property_tree/ptree.hpp"
#include "boost/property_tree/ini_parser.hpp"
#include "boost/foreach.hpp"

#include "../factors/factor_inits.h"
#include "../factors/factor_register.h"

#include <jni.h>


namespace mia{
namespace elly{
    namespace utils{
            
        /**
         * \brief Class that contains a set of relations and load them.
         */
        class FactorFileParser{
        public:
            std::string folder_name;
            std::string catelog;
            
            elly::utils::Config* config;
        
            std::vector<mia::elly::dstruct::AbstractCorrelationRelation* > crs;
            mia::elly::dstruct::VariableFactorRelation vf;
            mia::elly::dstruct::VariableAssignmentRelation va;
            
            mia::elly::dstruct::VariableTallyRelation vt;
            mia::elly::dstruct::VariableTrainingRelation vtrain;
            
            
            FactorFileParser(std::string _folder_name, elly::utils::Config* _config){
                
                folder_name = _folder_name;
                catelog = folder_name + "/catelog.cfg"; 
                config = _config;
            }
            
            void parse(){
                
                elly::utils::log() << ">> Parsing factors from " << catelog << std::endl;
                
                boost::property_tree::ptree pt;
                boost::property_tree::ini_parser::read_ini(catelog.c_str(), pt);
                
                boost::property_tree::ptree::const_iterator end = pt.end();
                
                std::string itype = "";
                
                for(boost::property_tree::ptree::const_iterator it = pt.begin();
                    it != end; ++it){
                    
                    if(it->first.compare("type") == 0 &&
                      it->second.data().compare("file") == 0){
                        std::cout << "  | Input type: In-file factor graph." << std::endl;
                        itype = "file";
                        break;
                    }
                    
                    if(it->first.compare("type") == 0 &&
                       it->second.data().compare("mln") == 0){
                        std::cout << "  | Input type: Markov Logic Network." << std::endl;
                        itype = "mln";
                        break;
                    }
                    
                    if(it->first.compare("type") == 0){
                        elly::utils::logerr() << "ERROR: not parsable type in config file.";
                        throw std::exception();
                    }
                    
                }
                
                if(itype.compare("mln") == 0){
                    
                    std::string evid = "";
                    std::string mln = "";
                    std::string query = "";
                    std::string queryfile = "";
                    
                    std::string java = "";
                    std::string path_to_tuffy = "";
                    std::string tuffy_config = "";
                    std::string other_config = "";
                    
                    for(boost::property_tree::ptree::const_iterator it = pt.begin();
                        it != end; ++it){

                        BOOST_FOREACH(boost::property_tree::ptree::value_type &v,
                                      pt.get_child(it->first)){
                            
                            
                            if(it->first.compare("mln") == 0){
                                mln = mln + " " + v.second.data();
                            }
                            
                            if(it->first.compare("evid") == 0){
                                evid = evid + " " + v.second.data();
                            }
                            
                            if(it->first.compare("queryfile") == 0){
                                queryfile = queryfile + " "+ v.second.data();
                            }
                            
                            if(it->first.compare("query") == 0){
                                query = query + " " + v.second.data();
                            }
                            
                            if(it->first.compare("option") == 0 &&
                               v.first.compare("java") == 0){
                                java = v.second.data();
                            }
                            
                            if(it->first.compare("option") == 0 &&
                               v.first.compare("tuffy") == 0){
                                path_to_tuffy = v.second.data();
                            }
                            
                            if(it->first.compare("option") == 0 &&
                               v.first.compare("iomln") == 0){
                                config->io_mln = v.second.data();
                            }
                            
                            if(it->first.compare("option") == 0 &&
                               v.first.compare("config") == 0){
                                tuffy_config = v.second.data();
                            }
                            
                            if(it->first.compare("option") == 0 &&
                               v.first.compare("other") == 0){
                                other_config = v.second.data();
                            }
                            
                        }
                        
                    }
                    
                    if(java.compare("") == 0 ||
                       path_to_tuffy.compare("") == 0 ||
                       mln.compare("") == 0 ||
                       evid.compare("") == 0 ||
                       config->io_mln.compare("") == 0){
                        elly::utils::logerr() << "ERROR: empty java/path_to_tuffy/mln/iomln" << std::endl;
                        throw std::exception();
                    }
                                        
                    elly::utils::log() << "  | MLN         = " << mln << std::endl;
                    elly::utils::log() << "  | Evid        = " << evid << std::endl;
                    elly::utils::log() << "  | Query       = " << query << std::endl;
                    elly::utils::log() << "  | QueryFile   = " << queryfile << std::endl;
                    elly::utils::log() << "  | Java        = " << java << std::endl;
                    elly::utils::log() << "  | Tuffy       = " << path_to_tuffy << std::endl;
                    elly::utils::log() << "  | TuffyConfig = " << tuffy_config << std::endl;
                    elly::utils::log() << "  | OtherConfig = " << other_config << std::endl;
                    
                    std::vector<std::string> cmds;
                    //std::string cmd = "";
                    
                    //cmd = java + " -jar " + path_to_tuffy + " -mia -i " + mln + " -e " + evid;
                    
                    cmds.push_back("-mia");
                    cmds.push_back("-i");
                    cmds.push_back(mln);
                    cmds.push_back("-e");
                    cmds.push_back(evid);
                    
                    if(query.compare("")!=0){
                        //cmd += " -q " + query;
                        cmds.push_back("-q");
                        cmds.push_back(query);
                    }
                    
                    if(queryfile.compare("")!=0){
                        //cmd += " -queryFile " + queryfile;
                        cmds.push_back("-queryFile");
                        cmds.push_back(queryfile);
                    }
                    if(tuffy_config.compare("")!=0){
                        //cmd += " -conf " + tuffy_config;
                        cmds.push_back("-conf");
                        cmds.push_back(tuffy_config);
                    }
                    //if(other_config.compare("")!=0){
                    //    cmd += other_config;
                    //}
                    
                    cmds.push_back("-o");
                    cmds.push_back(config->rt_output);
                    cmds.push_back("-verbose");
                    cmds.push_back("3");
                    
                    system((std::string("mkdir -p ") + config->rt_output).c_str());
                    
                    std::string tuffy_log = config->rt_output + "/log_tuffy.txt";
                    std::string tuffy_error = config->rt_output + "/error_tuffy.txt";
                    
                    JavaVM *jvm;       /* denotes a Java VM */
                    JNIEnv *env;       /* pointer to native method interface */
                    JavaVMInitArgs vm_args; /* JDK/JRE 6 VM initialization arguments */
                    JavaVMOption* options = new JavaVMOption[1];
                    
                    std::string classpath = "-Djava.class.path=";
                    classpath += path_to_tuffy + "/:";
                    classpath += path_to_tuffy + "/" + "commons-lang3-3.0-beta-javadoc.jar:";
                    classpath += path_to_tuffy + "/" + "JavaEWAH-0.3.2.jar:";
                    classpath += path_to_tuffy + "/" + "commons-lang3-3.0-beta.jar:";
                    classpath += path_to_tuffy + "/" + "antlr-3.2.jar:";
                    classpath += path_to_tuffy + "/" + "commons-math-2.2.jar:";
                    classpath += path_to_tuffy + "/" + "args4j-2.0.12.jar:";
                    classpath += path_to_tuffy + "/" + "jgrapht-jdk1.6.jar:";
                    classpath += path_to_tuffy + "/" + "commons-codec-1.6.jar:";
                    classpath += path_to_tuffy + "/" + "postgresql-8.4-701.jdbc4.jar";
                    
                    
                    char sstr[1000];
                    sprintf(sstr, "%s", classpath.c_str());
                    std::cout << sstr << std::endl;
                    
                    options[0].optionString = sstr;
                    
                    vm_args.version = JNI_VERSION_1_6;
                    vm_args.nOptions = 1;
                    vm_args.options = options;
                    vm_args.ignoreUnrecognized = false;
                    /* load and initialize a Java VM, return a JNI interface
                     * pointer in env */
                    JNI_CreateJavaVM(&jvm, (void**)&env, &vm_args);
                    delete options;
                    
                    
                    jobjectArray ret;
                    ret= (jobjectArray)env->NewObjectArray(cmds.size(),
                                                           env->FindClass("java/lang/String"),
                                                           env->NewStringUTF(""));
                    
                    for(int i=0;i<cmds.size();i++) {
                        env->SetObjectArrayElement(
                                                   ret,i,env->NewStringUTF(cmds[i].c_str()));
                    }
                    
                    jclass cls = env->FindClass("tuffy/main/Main");
                    
                    jmethodID mid = env->GetStaticMethodID(cls, "main_return", "([Ljava/lang/String;)I");
                    jint rs = env->CallStaticIntMethod(cls, mid, ret);
                    
                    //std::cout << "JAVA RESULT = " << rs << std::endl;
                    
                    if(rs != 99999){
                        std::cout << ">> Error in running Tuffy!" << std::endl;
                        throw std::exception();
                    }
                    
                    config->io_ismln = true;
                    
                    // remove content of files in output folder
                    //int rs = system(("rm " + config->rt_output + "/*").c_str());
                    
                    //if(rs == 0){
                    //    std::cout << "Cannot run " << "[rm " + config->rt_output + "/*" + "]... Exiting..." << std::endl;
                    //    assert(rs != 0);
                    //}
                    
                    //system(cmd.c_str());
                   
                    //itype = "file";
                    //config.rt_input = config.rt_output;
                    
                    folder_name = config->rt_output;
                    catelog = folder_name + "/catelog.cfg";
                    
                    this->parse();
                    
                    
                    //itype = "mln";
                    
                    return;
                }
                
                if(itype.compare("file") == 0){
                
                    for(boost::property_tree::ptree::const_iterator it = pt.begin();
                        it != end; ++it){
                        
                        if(it->first.compare("type") == 0){
                            continue;
                        }
                        
                        if(it->first.compare("__vf") == 0){
                            
                            BOOST_FOREACH(boost::property_tree::ptree::value_type &v,
                                          pt.get_child(it->first)){
                                
                                if(v.first.compare("type") == 0){
                                    vf.filetype = v.second.data();
                                    va.filetype = v.second.data();
                                    vt.filetype = v.second.data();
                                    vtrain.filetype = v.second.data();
                                }
                                
                                if(v.first.compare("file") == 0){
                                    vf.filename = v.second.data();
                                    va.filename = v.second.data();
                                    vt.filename = v.second.data();
                                    vtrain.filename = v.second.data();
                                }
                                
                            }
                                                        
                            continue;
                        }
                        
                        if(it->first.compare("__train") == 0){
                            
                            BOOST_FOREACH(boost::property_tree::ptree::value_type &v,
                                          pt.get_child(it->first)){
                                
                                if(v.first.compare("type") == 0){
                                    vtrain.filetype = v.second.data();
                                }
                                
                                if(v.first.compare("file") == 0){
                                    vtrain.filename = v.second.data();
                                }
                                
                            }
                            
                            continue;
                            
                        }
                    
                        //mia::elly::dstruct::StandardCorrelationRelation<mia::sm::Buffer_mm> * cr =
                        //new mia::elly::dstruct::StandardCorrelationRelation<mia::sm::Buffer_mm>;
                        
                        //mia::elly::dstruct::StandardCorrelationRelation<mia::sm::Buffer_mmap> * cr =
                        //new mia::elly::dstruct::StandardCorrelationRelation<mia::sm::Buffer_mmap>;
                        
                        int func_id = -1;
                        BOOST_FOREACH(boost::property_tree::ptree::value_type &v,
                                      pt.get_child(it->first)){
                            if(v.first.compare("func") == 0){
                                func_id = atoi(v.second.data().c_str());
                            }
                        }
                        assert(func_id != -1);

                        //mia::elly::dstruct::AbstractCorrelationRelation * cr = mia::elly::factors::get_correlation_relation<hazy::sman::STORAGE_MM>(func_id);

                        mia::elly::dstruct::AbstractCorrelationRelation * cr = mia::elly::factors::get_correlation_relation<hazy::sman::STORAGE_HBASE>(func_id);

                        
                        cr->factor_name = it->first;
                        cr->mapfilename = "";
                    
                        BOOST_FOREACH(boost::property_tree::ptree::value_type &v,
                                  pt.get_child(it->first)){
                        
                            if(v.first.compare("fid") == 0){
                                cr->factor_id = atoi(v.second.data().c_str());
                            }

                            if(v.first.compare("func") == 0){
                                cr->function_id = atoi(v.second.data().c_str());
                            }
                            
                            if(v.first.compare("type") == 0){
                                cr->filetype = v.second.data();
                            }
                        
                            if(v.first.compare("file") == 0){
                                cr->filename = v.second.data();
                            }
                            
                            if(v.first.compare("weight") == 0){
                                cr->mapfilename = v.second.data();
                            }
                        
                        }
                    
                        if(crs.size() != cr->factor_id){
                            mia::elly::utils::logerr() << " ERROR: INPUT factor must be 0,1,..." << std::endl;
                            assert(false);
                        }
                        crs.push_back((mia::elly::dstruct::AbstractCorrelationRelation *)cr);
                    
                    }
                
                    elly::utils::log() << "  | Parsed " << crs.size() << " factors: " << std::endl;
                    for(int ff = 0; ff < crs.size(); ff++){
                
                        elly::utils::log() << "    + Factor [" << crs[ff]->factor_name << "]:" << std::endl;

                        elly::utils::log() << "      - function_id = " << crs[ff]->function_id << "" << std::endl;

                        elly::utils::log() << "      - filename = " << crs[ff]->filename << "" << std::endl;
                    
                        elly::utils::log() << "      - filetype = " << crs[ff]->filetype << "" << std::endl;
                
                    }
                
                    elly::utils::log() << ">> Preparing " << crs.size() << " factors: " << std::endl;
                    for(int ff = 0; ff < crs.size(); ff++){
                    
                        elly::utils::log() << "  | Factor [" << crs[ff]->factor_name << "]:" << std::endl;
                    
                        crs[ff]->prepare();
                        crs[ff]->prepare_weights();
                    
                    }
                    
                    elly::utils::log() << ">> Initializing variable assignments to 0..." << std::endl;
                    va.prepare();
                    
                    elly::utils::log() << ">> Preparing variable-factor relation..." << std::endl;
                    vf.prepare();

                    if(config->rt_mode.compare("marginal") == 0 || config->rt_mode.compare("ds") == 0){
                        elly::utils::log() << ">> Preparing variable tally relation..." << std::endl;
                        vt.prepare();
                    }
                    
                    if(config->rt_mode.compare("learn") == 0 || config->rt_mode.compare("ds") == 0){
                        elly::utils::log() << ">> Preparing variable training relation..." << std::endl;
                        vtrain.prepare();
                    }
                    
                }
                
            }
            
            
        };
    
    }
}
}


#endif
