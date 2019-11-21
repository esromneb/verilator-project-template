#define VERILATE_TESTBENCH

#include <stdlib.h>
#include <iostream>
#include <vector>
#include <assert.h>
#include <verilated.h>
#include <sys/stat.h>
#include "Vtimer_thing.h"
#include "Vtimer_thing__Syms.h"
#include <verilated_vcd_c.h>

// must come before higgs_helper
typedef Vtimer_thing top_t;

#include "higgs_helper.hpp"

typedef HiggsHelper<top_t> helper_t;


VerilatedVcdC* tfp = NULL;
// Construct the Verilated model, from Vtop.h generated from Verilating "top.v"
// Or use a const unique_ptr, or the VL_UNIQUE_PTR wrapper
top_t* top = new top_t;
// Current simulation time (64-bit unsigned)
uint64_t main_time = 0;
// Called by $time in Verilog
double sc_time_stamp () {
  return main_time; // Note does conversion to real, to match SystemC
}

#include "test_0.cpp"

int main(int argc, char** argv, char** env) {
    uint32_t test_select = 0;

    if(const char* env_p = std::getenv("TEST_SELECT")) {
        unsigned int env_test_select = atoi(env_p);
        std::cout << "Environment variable TEST_SELECT was set to: "
             << env_test_select << "\n";
        test_select = env_test_select;
    }


    unsigned int fixed_seed = 0;
    setup_random(fixed_seed);

    switch(test_select) {
        case 0:
            test0(argc, argv, env);
            break;
        // case 1:
        //     test1(argc, argv, env);
        //     break;
        // case 2:
        //     test2(argc, argv, env);
        //     break;
        // case 3:
        //     test3(argc, argv, env);
        //     break;
        // case 4:
        //     test4(argc, argv, env);
        //     break;
        // case 5:
        //     test5(argc, argv, env);
        //     break;
        default:
            std::cout << "Invalid test selected\n";
            exit(1);
            break;
    }

}
