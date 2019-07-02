module freq_measure_top(Sys_clk,rst_n,sig_in_a,sig_in_b,led_on,Signal_Tx);

input wire Sys_clk;
input wire rst_n;

input wire sig_in_a;
input wire sig_in_b;

output reg led_on;
output wire Signal_Tx;

reg[31:0] delay_time;
reg[31:0] duty;


// output wire clk_out1;
// output wire clk_out2;

wire clk_devided0;
wire locked;
reg start;
wire busy,finished;
// wire[31:0] final_clk_cnt;
wire[31:0] final_sig_cnt1,final_sig_cnt2,final_sig_cnt3; //The output Of the module must be wire
wire[31:0] final_time_cnt;

wire rst,rst2;



//Wait For a While and then start working
reg[15:0] wait_for_start;

always @(posedge clk_out1 or negedge rst_n) begin 
	if(~rst_n) begin
		led_on <= 0;
		start <= 0;
		wait_for_start <= 0;
	end 

	else if(wait_for_start == 16'h1111) begin
		start <= 1;
		wait_for_start <= 0;
		led_on <= 1;
	end

	else begin
		start <= start;
		wait_for_start <= wait_for_start + 1;
		led_on <= led_on;
	end
end

reg sig_in_a_r;
reg sig_in_a_r2;
 (* mark_debug = "TRUE" *)  reg sig_in_a_rr;
reg sig_in_a_rr2;
reg sig_in_b_r;
 (* mark_debug = "TRUE" *) reg sig_in_b_rr;

always @(posedge clk_out1 or negedge rst_n) begin 
	if(~rst_n) begin
		sig_in_a_r <= 0;
		sig_in_a_rr <= 0;
	end else begin
		sig_in_a_r <= sig_in_a;
		sig_in_a_rr <= sig_in_a_r;
	end
end

always @(posedge clk_out1 or negedge rst_n) begin 
	if(~rst_n) begin
		sig_in_b_r <= 0;
		sig_in_b_rr <= 0;
	end 
	else begin
		sig_in_b_r <= sig_in_b;
		sig_in_b_rr <= sig_in_b_r;
	end
end



//rst is the signal after process
//used for other modules whose clk is not sys_clk
rstpulse rst_inst0(
        .clk_in(Sys_clk),
        .clk_sys(clk_out1),
        .rst_in(rst_n),
        .rst(rst)
    );

// rstpulse rst_inst1(
//         .clk_in(Sys_clk),
//         .clk_sys(clk_out3),
//         .rst_in(rst_n),
//         .rst(rst2)
//     );


//Generate the clk,
clk_wiz_0 clk_wiz_test 
(
// Clock out ports
.clk_out1(clk_out1),     // output clk_out1
.clk_out2(clk_out2),     // output clk_out2
.clk_out3(clk_out3),
// Status and control signals
.reset(!rst_n), // input reset
.locked(locked),       // output locked
// Clock in ports
.clk_in1(Sys_clk));      // input clk_in1


// // Clock DIivide
// wire clk_divided_1,clk_divided_2;

// CLK_DIVIDER clk_divider0(
// 	.clkin(clk_out1),
// 	.clkout(clk_divided_1)
// 	);

// CLK_DIVIDER clk_divider1(
// 	.clkin(clk_out2),
// 	.clkout(clk_divided_2)
// 	);


// duty_measure duty_measure0(
// 	.clk_in(clk_out3),
// 	.rst_n(!rst2),
// 	.sig_in(sig_in_a_rr2),
// 	.final_duty_cnt(final_duty_cnt),
// 	.finish(duty_finished)
// 	);

freq_measure freq_measure_0(
	.clk_in(clk_out1),
	.sig_in(sig_in_a_rr),
	.sig_in_2(sig_in_b_rr),
	.rst_n(!rst),
	.start(start),
	.busy(busy),
	.finished(finished),
	.final_sig_cnt1(final_sig_cnt1),
	.final_sig_cnt2(final_sig_cnt2),
	.final_sig_cnt3(final_sig_cnt3),
	// .final_clk_cnt(final_clk_cnt),
	.final_time_cnt(final_time_cnt)
	);


Uart_For_Freq_Measure Uart0(
	.Sys_CLK(Sys_clk),
	.Sys_RST(!rst),
	.Signal_Tx(Signal_Tx),
	.final_time_cnt(final_time_cnt),
	.final_sig_cnt1(final_sig_cnt1),
	.final_sig_cnt2(final_sig_cnt2),
	.final_sig_cnt3(final_sig_cnt3)
	);



endmodule // freq_measure_top
