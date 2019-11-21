#ifndef __HIGGS_HELPER__
#define __HIGGS_HELPER__

#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <vector>
#include <assert.h>
#include <cstdint>
#include <cmath>

// Include common routines
//#include <verilated.h>

#include <verilated_vcd_c.h>

// since higgs_helper is providing everything, why not grab this
using namespace std;



#ifndef RESET
#define RESET reset
#endif



void preReset(top_t* top) {
  // Initialize inputs
  top->RESET = 1;
  top->clk = 0;
}

void postReset(top_t* top) {
}


void setup_random(uint32_t fixed_seed) {

    unsigned int seed_start = std::time(0);
    
    if(const char* env_p = std::getenv("TEST_SEED")) {
        unsigned int env_seed = atoi(env_p);
        cout << "environment variable TEST_SEED was set to: " << env_seed << endl;
        fixed_seed = env_seed;
    }

    if(fixed_seed != 0) {
        seed_start = fixed_seed;
        cout << "starting with hard-coded seed " << seed_start << endl;
    } else {
        cout << "starting with random seed " << seed_start << endl;
    }

    srand(seed_start);
}



template <class T>
class HiggsHelper {

public:
    T *top;
    uint64_t* main_time;
    VerilatedVcdC* tfp = NULL;
    unsigned char *clk;
   

    HiggsHelper(T *_top, uint64_t *_main_time, VerilatedVcdC *_tfp):
    top(_top)
    ,main_time(_main_time)
    ,tfp(_tfp)
    ,clk(&(_top->clk))
    {
        this->initialize_ports();
    }


    uint64_t us(void) const {
        return *main_time / 1000;
    }

    // pass number of clock cycles to reset, must be even number and greater than 4
    void reset(unsigned count=40) {
        count += count % 2; // make even number

        // bound minimum
        if(count < 4) {
                count += 4 - count;
        }

        *clk = 0;
        top->RESET = 1;
        for(auto i = 0; i < count; i++) {

                if(i+2 >= count)
                {
                        top->RESET = 0;
                }

                (*main_time)++;
                *clk = !*clk;
                if(tfp) {tfp->dump(*main_time);}
                top->eval();
        }
        top->RESET = 0;
    }



    void handleDataNeg() {
    }

    void handleDataPos() {
    }


    void entered_next_us(uint64_t us) {
        // std::cout << "here " << us << "us" << std::endl;
        print_time(us);
    }


    void check_us() {
        constexpr int increment = (1000); // 1 us
        constexpr int convert_us = (1000);
        if(*main_time % increment == 0) {
            uint64_t us = *main_time / convert_us;
            entered_next_us(us);
        }
    }




    void print_time(uint64_t us) {
        constexpr int increment = (100); // 100 us
        if(us % increment == 0) {
            int val;
            std::cout << us << "us" << std::endl;
        }
    }



// clock from 0 to 1
void tickHigh(void) {
   (*main_time)++;
   *clk = !*clk;
   top->eval();
   if(tfp) {tfp->dump(*main_time);}
   this->handleDataNeg();
}


void tickLow(void) {
    *clk = !*clk;
    (*main_time)++;
    top->eval();
    if(tfp) {tfp->dump(*main_time);}
    this->handleDataPos();

    check_us();
}


void tick(const unsigned count = 1){
    for(unsigned i = 0; i < count; i++) {
        tickHigh();
        tickLow();
    }
}



  void initialize_ports() {
  }


}; // class HiggsHelper



///
/// In the macro TB_START_FILENAME we would like to accept both std::string and char
/// this overload is required to make that happen
///
void _tfp_write_file_overload(VerilatedVcdC* const _tfp, const char* const filename) {
    _tfp->open(filename);
}

///
/// In the macro TB_START_FILENAME we would like to accept both std::string and char
/// this overload is required to make that happen
///
void _tfp_write_file_overload(VerilatedVcdC* const _tfp, const std::string& filename) {
    _tfp->open(filename.c_str());
}



#define TB_START_PRE() \
(void)argc;(void)argv;(void)env; \
Verilated::commandArgs(argc, argv); /*Pass arguments so Verilated code can see them, e.g. $value$plusargs*/ \
Verilated::debug(0); /* Set debug level, 0 is off, 9 is highest presently used */ \
Verilated::randReset(2);

#define TB_START_FILENAME(filename) \
{\
const char* const flag = Verilated::commandArgsPlusMatch("trace"); \
if (flag && 0==strcmp(flag, "+trace")) { \
    Verilated::traceEverOn(true);  \
    cout << "Enabling waves into " << (filename) << "...\n\n"; \
    tfp = new VerilatedVcdC; \
    top->trace(tfp, 99); \
    \
    _tfp_write_file_overload(tfp, (filename)); \
} else { \
    cout << "WILL NOT WRITE .vcd WAVE FILE\n"; \
    cout << "  \"make show\" will be stale\n\n"; \
} \
}

#define STANDARD_TB_START() \
(void)argc;(void)argv;(void)env; \
Verilated::commandArgs(argc, argv); /*Pass arguments so Verilated code can see them, e.g. $value$plusargs*/ \
Verilated::debug(0); /* Set debug level, 0 is off, 9 is highest presently used */ \
Verilated::randReset(2); \
TB_START_FILENAME("wave_dump.vcd");


#endif
