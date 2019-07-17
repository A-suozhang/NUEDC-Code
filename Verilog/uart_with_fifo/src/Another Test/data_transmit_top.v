module data_transmit_top(
    input wire Sys_clk,
    input wire rst_n,
    input wire SW3,
    input wire RX,
    output wire TX,
    output wire led_on
    );

assign led_on = ~SW3;   // Using Led To Denote


// -------------The CLK WIZARD
clk_wiz_0 clk_wiz_test 
(
// Clock out ports
.clk_out1(clk),     // output clk_out1
// Status and control signals
.reset(!rst_n), // input reset
.locked(locked),       // output locked
// Clock in ports
.clk_in1(Sys_clk));      // input clk_in1



// --------------The RESET PULSE
wire rst;
//rst is the signal after process
//used for other modules whose clk is not sys_clk
rstpulse rst_inst0(
    .clk_in(Sys_clk),
    .clk_sys(clk),
    .rst_in(rst_n),
    .rst(rst)
);


Data_Transmit DATA_TRANSMIT_0(
    .clk(clk),
    .rst(rst),
    .RX(RX),
    .TX(TX)
);





endmodule
