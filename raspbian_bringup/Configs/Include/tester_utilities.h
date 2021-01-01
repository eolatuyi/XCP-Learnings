/*
 * tester_utilities.h
 *
 *  Created on: Jul 25, 2020
 *      Author: gertrude
 */

#ifndef CONFIGS_INCLUDES_TESTER_UTILITIES_H_
#define CONFIGS_INCLUDES_TESTER_UTILITIES_H_

#include <stdint.h>

#define DEFINE_TESTER_UTILITIES_SUT_OBJECTS

typedef void (*test_utilities_stimulus_writer_t)(void* param,   void* val, uint8_t size);
typedef void (*test_utilities_transducer_reader_t)(void* param, void* val, uint8_t size);

struct tester_utilities_stimulus
{
    uint32_t  len;
    uint32_t* time_stamp;
    void*     params;
    void*     values;
    uint8_t*  sizes;
    
    //test_utilities_stimulus_writer_t* writers;
    
};

struct tester_utilities_expectation
{
    uint32_t  len;
    uint32_t* time_stamp;
    void*     params;
    void*     values;
    uint8_t*  sizes;
    
  //  test_utilities_transducer_reader_t* readers;
};

typedef void (*test_utilities_simulator_t)(void* cblk);
typedef void (*test_utilities_job_gen_t)(void* cblk);
typedef void (*test_utilities_job_observer_t)(void* cblk);

struct tester_utilities_stimulator_cblk
{
    uint8_t stimulus_len;
    uint8_t expectation_len;
    struct tester_utilities_stimulus*    stimulus;
    struct tester_utilities_expectation* expectation;
    
    test_utilities_simulator_t    simulator;
    
    volatile uint8_t              trigger_generator;

    void*                         gen_arg;
    
    test_utilities_job_gen_t      generator;
    test_utilities_job_observer_t transducer;
};

#define DEFINE_TESTER_UTILITIES_SIMULATION(stimulus, stimulus_len, expectation, expectation_len,gen_arg, generator, transducer)\
                                                                               \
    struct tester_utilities_stimulator_cblk tester_utilities_stimulator_cblk = \
    {                                                                          \
        stimulus_len,                                                          \
        expectation_len,                                                       \
        stimulus,                                                              \
        expectation,                                                           \
        tester_utilities_simulator_thread,                                     \
        0,                                                                     \
        gen_arg,                                                               \
        generator,                                                             \
        transducer                                                             \
    };


#define TESTER_UTILITIES_SIMULATOR_CBLK tester_utilities_stimulator_cblk  
#define DECLARE_TESTER_UTILITIES_SIMULATOR_CBLK extern struct tester_utilities_stimulator_cblk tester_utilities_stimulator_cblk;  

#define DEFINE_TESTER_UTILITIES_STIMULUS(len, time_stamp, param_names, vals)
#define DEFINE_TESTER_UTILITIES_EXPECTATION(len, time_stamp, param_names, vals)

DECLARE_TESTER_UTILITIES_SIMULATOR_CBLK

void tester_utilities_init(void);
void tester_utilities_simulator_thread(void* arg);

#endif /* CONFIGS_INCLUDES_TESTER_UTILITIES_H_ */
