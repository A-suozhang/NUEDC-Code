module pin_test_top(Sys_clk, rst_n);

// Simply Driving clk_wizard
// Adding Keys For 

input wire Sys_clk;
input wire rst_n;

clk_wiz_0 clk_wiz_test 
(
// Clock out ports
.clk_out1(clk_out1),     // output clk_out1
// Status and control signals
.reset(!rst_n), // input reset
.locked(locked),       // output locked
// Clock in ports
.clk_in1(Sys_clk));      // input clk_in1

// -----------------------------
wire rst;
wire locked;
//rst is the signal after process
//used for other modules whose clk is not sys_clk
rstpulse rst_inst0(
    .clk_in(Sys_clk),
    .clk_sys(clk_out1),
    .rst_in(rst_n),
    .rst(rst)
);


// --------------
wire led_on;
wire SW3;
led_test led_test0(
    .clk_in(clk_out1),
    .rst(rst),
    .switch_on(SW3),
    .led_on(led_on)
);

// -----------------
// Divide The System CLK
clk_divide clk_divide0(
    .clk_in(clk_out1),
    .rst(rst),
    .clk_out(out)
);

endmodule