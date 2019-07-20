`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2018/11/12 15:01:06
// Design Name: 
// Module Name: ADC_DRIVER
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module ADC_DRIVER(
        input clk,
        output clkadc,
        input rst,
        output clkadclocked,
        
        output adc_clkP_out_P,
        output adc_clkM_out_N,
        input adc_clkPout_in_P,
        input adc_clkMout_in_N,
        output adc_ctrl1_out,
        output adc_ctrl2_out,
        output adc_ctrl3_out,
        output adc_reset_out,
        output adc_sclk_out,
        output adc_sdata_out,
        input adc_sdout_in,
        output adc_sen_out,
        input [6:0] adc_dataA_in_P,
        input [6:0] adc_dataA_in_N,
        input [6:0] adc_dataB_in_P,
        input [6:0] adc_dataB_in_N,
        
        output reg signed [13:0]adcdatA,
        output reg signed [13:0]adcdatB
    );
    
    OBUFDS #(
        .IOSTANDARD("DEFAULT") // Specify the output I/O standard
    ) OBUFDS_adcclk (
        .O(adc_clkP_out_P),     // Diff_p output (connect directly to top-level port)
        .OB(adc_clkM_out_N),   // Diff_n output (connect directly to top-level port)
        .I(clk)      // Buffer input 
    );
    
    assign adc_reset_out=1'b1;
    assign adc_ctrl1_out=1'b0;
    assign adc_ctrl2_out=1'b0;
    assign adc_ctrl3_out=1'b0;
    assign adc_sen_out=1'b1;
    assign adc_sclk_out=1'b0;
    
    clk_wiz_adc clkadc_i (
        // Clock out ports
        .clk_out1(clkadc),     // output clk_out1
        // Status and control signals
        .reset(rst), // input reset
        .locked(clkadclocked),       // output locked
        // Clock in ports
        .clk_in1_n(adc_clkMout_in_N),
        .clk_in1_p(adc_clkPout_in_P)
    );
    
    
    wire [6:0]adcdatAbuf;
    wire signed [13:0]adcdatAddr;
    
    wire [6:0]adcdatBbuf;
    wire signed [13:0]adcdatBddr;
    
    genvar i;
    generate
    for(i=0;i<=6;i=i+1)begin
        IBUFDS #(
            .DIFF_TERM("FALSE"),       // Differential Termination
            .IOSTANDARD("DEFAULT")     // Specify the input I/O standard
        ) IBUFDS_inst (
            .O(adcdatAbuf[i]),  // Buffer output
            .I(adc_dataA_in_P[i]),  // Diff_p buffer input (connect directly to top-level port)
            .IB(adc_dataA_in_N[i]) // Diff_n buffer input (connect directly to top-level port)
        );
    end
    endgenerate
    genvar k;
    generate
    for(k=0;k<=6;k=k+1)begin
        IBUFDS #(
            .DIFF_TERM("FALSE"),       // Differential Termination
            .IOSTANDARD("DEFAULT")     // Specify the input I/O standard
        ) IBUFDS_inst (
            .O(adcdatBbuf[k]),  // Buffer output
            .I(adc_dataB_in_P[k]),  // Diff_p buffer input (connect directly to top-level port)
            .IB(adc_dataB_in_N[k]) // Diff_n buffer input (connect directly to top-level port)
        );
    end
    endgenerate
    
    genvar j;
    generate
    for(j=0;j<=6;j=j+1)begin
        IDDR #(
            .DDR_CLK_EDGE("OPPOSITE_EDGE"), // "OPPOSITE_EDGE", "SAME_EDGE" 
                                          //    or "SAME_EDGE_PIPELINED" 
            .INIT_Q1(1'b0), // Initial value of Q1: 1'b0 or 1'b1
            .INIT_Q2(1'b0), // Initial value of Q2: 1'b0 or 1'b1
            .SRTYPE("SYNC") // Set/Reset type: "SYNC" or "ASYNC" 
        ) IDDR_inst (
            .Q1(adcdatAddr[2*j]), // 1-bit output for positive edge of clock 
            .Q2(adcdatAddr[2*j+1]), // 1-bit output for negative edge of clock
            .C(clkadc),   // 1-bit clock input
            .CE(1'b1), // 1-bit clock enable input
            .D(adcdatAbuf[j]),   // 1-bit DDR data input
            .R(1'b0),   // 1-bit reset
            .S(1'b0)    // 1-bit set
        );
    end
    endgenerate
    genvar l;
    generate
    for(l=0;l<=6;l=l+1)begin
        IDDR #(
            .DDR_CLK_EDGE("OPPOSITE_EDGE"), // "OPPOSITE_EDGE", "SAME_EDGE" 
                                          //    or "SAME_EDGE_PIPELINED" 
            .INIT_Q1(1'b0), // Initial value of Q1: 1'b0 or 1'b1
            .INIT_Q2(1'b0), // Initial value of Q2: 1'b0 or 1'b1
            .SRTYPE("SYNC") // Set/Reset type: "SYNC" or "ASYNC" 
        ) IDDR_inst (
            .Q1(adcdatBddr[2*l]), // 1-bit output for positive edge of clock 
            .Q2(adcdatBddr[2*l+1]), // 1-bit output for negative edge of clock
            .C(clkadc),   // 1-bit clock input
            .CE(1'b1), // 1-bit clock enable input
            .D(adcdatBbuf[l]),   // 1-bit DDR data input
            .R(1'b0),   // 1-bit reset
            .S(1'b0)    // 1-bit set
        );
    end
    endgenerate
    
    always@(posedge clkadc)begin
        adcdatA<=adcdatAddr;
        adcdatB<=adcdatBddr;
    end
endmodule
