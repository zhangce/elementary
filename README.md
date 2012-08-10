===============
Mia::Elementary
===============

Mia::Elementary is an execution engine for scalable statistial 
inference and learning.

Mia::Elementary is developed by the Hazy group (http://hazy.cs.wisc.edu/hazy/)
from University of Wisconsin-Madison under Professor Christopher Ré 
(http://pages.cs.wisc.edu/~chrisre/).


  Table of Content
--------------------
  1. Introduction
  2. Installation
  3. Examples

   
### 1. Introduction

### 2. Installation

After downloading and uncompressing Mia::Elementary, the folder mia has the following structure:

    mia  
     |-- bin  
     |-- dep  
     |-- src  
     |-- CMakeLists.txt  
     |-- README.md  

#### 2.1 Prerequisite

##### 2.1.1 Operating Systems

Mia::Elementary is tested in the following operating systems:

  - MacOSX (>10.6);
  - Redhat Linux Enterprise 5.0;
  - Windows 7 Professional.

##### 2.1.2 Dependencies

Mia::Elementary is written in C++ and has been tested using the following compilers:

  - GCC 4.4.6 under Linux
  - GCC 4.2.1 under MacOSX
  - Visual Studio Express under Windows

To use the MLN component in Mia::Elementary, Java (>1.6) and Python (>2.6) is required.

Mia::Elementary relies on the following C++ libraries:

  - Boost::program_options (http://www.boost.org/doc/libs/1_50_0/doc/html/program_options.html)

We included a installation script for boost at mia/dep/boost. To install boost, type in under folder mia

    $ cd dep/boost  
    $ sh install.sh  

Mia::Elementary needs CMake (>2.6) to build. (http://www.cmake.org/)

2.2 Installation
   
To install Mia::Elementary, go to the folder mia, and type in

    $ cmake .  

You may need to set environment variable BOOST_ROOT.

The following lines should prompt:

    -- Use build optimizations: Release  
    -- The C compiler identification is GNU  
    -- The CXX compiler identification is GNU  
    -- Check for working C compiler: /usr/bin/gcc  
    -- Check for working C compiler: /usr/bin/gcc -- works  
    -- Detecting C compiler ABI info  
    -- Detecting C compiler ABI info - done  
    -- Check for working CXX compiler: /usr/bin/c++  
    -- Check for working CXX compiler: /usr/bin/c++ -- works  
    -- Detecting CXX compiler ABI info  
    -- Detecting CXX compiler ABI info - done  
    -- Boost version: 1.49.0  
    -- Found the following Boost libraries:  
    --   program_options  
    -- Configuring done  
    -- Generating done  
    -- Build files have been written to: /raid/czhang/Codes/mia  

A file named ``Makefile'' should appear in the folder mia now.
To build Mia::Elementary, type in

    $ make  
  
A binary file named ``elementary'' should appear in the folder mia/bin.
To test this binary, type in

    $ ./bin/elementary  

and the following lines should prompt asking for more command line options:

    ##### Elly 0.1 #####
    >> Parsing command line options... 
    ERROR: std::exception
    Allowed options:
    --help                  Produce help message  
    --config arg            Configuration file. command line options have higher   
                            priority than config file.  
    --ui.verbose arg        Verbose level for STDOUT and STDERR. {trace, debug,   
                            info}. [DEFAULT=info]  
    --ui.logfile arg        Path to log file if need one.  
    --ui.logverbose arg     Verbose level for log file. {trace, debug, info}.   
                            [DEFAULT=debug]  
    --rt.input arg          Where does input factor graph located in.   
                            {file://DIR_PATH}.  
    --rt.output arg         Where does output located in. {file://FILE_PATH}.  
    --rt.workdir arg        Directory that contains temporary files.  
    --rt.mode arg           Mode that Elly runs in. {map, marginal, sample, learn}.   
                            [DEFAULT=marginal]  
    --rt.thin arg           Collect one sample every #rt.thin iterations.   
                            [DEFAULT=1]  
    --rt.burnin arg         Ignore the first #rt.burnin collected samples.   
                            [DEFAULT=1]  
    --rt.nepoch arg         Number of iterations (epochs) to run. [DEFAULT=10]  
    --rt.learn_initstep arg Initial step size for SGD learning. [DEFAULT=0.01]  
    --rt.learn_decay arg    Decay factor for SGD learning step size after each   
                            epoch. [DEFAULT=1.00]  
    --sys.threads arg       Number of threads to use. [DEFAULT=1]  

### 3. Examples

#### 3.1 LDA

You can type in 

  > $ ./bin/elementary --config=examples/elly/config_test.cfg  

to execute LDA. Because it only uses 1 thread, wait 2 minutes for it to finish.

The result file will be in examples/elly/RS/lda-sample.txt. To see the human understandable
version, type in

  > $ python examples/elly/LDA/view.py examples/elly/RS/lda-sample.txt examples/elly/LDA/int_word.tsv  

to see the result of each topic

  > TOPIC #0     
  > new(327)    today(168)    san(168)    york(165)    texas(152)    city(142)    south(133)    
  > news(123)    network(121)    california(118)    high(118)    summer(114)    national(112)      
  > states(112)    southern(108)    ohio(107)    rain(105)    fair(101)    north(96)    
  >
  > TOPIC #1     
  > united(386)    states(267)    iraq(246)    war(231)    american(227)    israel(213)    
  > military(189)    kuwait(182)    iraqi(181)    iran(157)    nations(155)    gulf(154)    
  > troops(151)    saudi(150)    foreign(150)    officials(150)    two(147)    israeli(145)    official(136)  









