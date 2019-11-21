
module timer_thing
  (
      input wire clk,
      input wire reset,
      input wire input_a,
      input wire input_b,
      output logic output_c
      );


    localparam T_MIN = 20;
    localparam T_MAX = 40;


    logic latch_a;

    always_ff @(posedge clk or negedge reset) begin
        if( reset ) begin
            output_c <= 0;
            latch_a <= 0;
        end else begin

            if( latch_a ) begin
                if( counter >= T_MAX ) begin
                    latch_a <= 0;
                    output_c <= 0;
                end else if (input_b && counter <= T_MIN) begin
                    // do nothing
                end else if (input_b && counter >= T_MIN) begin
                    latch_a <= 0;
                    output_c <= 0;
                end
            end

            if( input_a ) begin
                output_c <= 1;
                latch_a <= 1;
            end

        end
    end

    logic [31:0]  counter;

    always_ff @(posedge clk or negedge reset) begin
        if( reset ) begin
            counter <= 32'h0;
        end else begin
            if( latch_a ) begin
                counter <= counter+1;
            end else begin
                counter <= 32'h0;
            end
        end
    end


endmodule
