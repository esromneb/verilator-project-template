


int test0(int argc, char** argv, char** env) {

    STANDARD_TB_START();

    HiggsHelper<top_t>* t = new HiggsHelper<top_t>(top,&main_time,tfp);
    preReset(top);
    t->reset(40);
    postReset(top);


    int total_runtime = 300; // in ns

    // cout << *t->main_time << "\n";

    const uint64_t time_a = *t->main_time; // what time is it after the reset?

    // because tick() is actually 2 ns, we += 2
    // this means you can never check ns against an odd number
    for(uint64_t ns = time_a; ns < total_runtime; ns+=2) {

        ///
        /// Example where we trigger after t_min
        /// 
        uint64_t d = 6;    // duration of trigger
        uint64_t ta1 = 100;
        uint64_t tb1 = 150;


        if( ns == ta1 ) {
            top->input_a = 1;
        }

        if( ns == ta1+d ) {
            top->input_a = 0;
        }

        if( ns == tb1 ) {
            top->input_b = 1;
        }

        if( ns == tb1+6 ) {
            top->input_b = 0;
        }


        ///
        /// Example where we trigger before t_min
        /// 

        uint64_t ta2 = 200;
        uint64_t tb2 = 210;


        if( ns == ta2 ) {
            top->input_a = 1;
        }

        if( ns == ta2+d ) {
            top->input_a = 0;
        }

        if( ns == tb2 ) {
            top->input_b = 1;
        }

        if( ns == tb2+6 ) {
            top->input_b = 0;
        }


        t->tick(1); // 1 tick is actually 2 ns
    }


    // Final model cleanup
    top->final();

    // Close trace if opened
    if (tfp) { tfp->close(); }




    // Destroy model
    delete top; top = NULL;
    exit(0);
}
