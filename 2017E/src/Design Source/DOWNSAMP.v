`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2019/7/12 10:01:06
// Design Name: 
// Module Name: DOWNSAMP_TRUNC
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

module DOWNSAMP#(
    parameter SAMPLE_RATE = 4, // sampling per 2^n data
    parameter DATA_WIDTH = 14
)
(
    input clk,
    input rst,
    input ena, 
    input signed[DATA_WIDTH-1:0] dataIn,
    output [DATA_WIDTH-1:0] us_dsoutdata,
    output signed[DATA_WIDTH-1:0] s_dsoutdata,
    output out_en,
    
    input outbusy
);
    assign out_en = (ds_counter == 0)&&(!outbusy);
    reg[SAMPLE_RATE-1:0] ds_counter;
    reg[DATA_WIDTH+SAMPLE_RATE-1:0] dsoutdata;

    // sign convert
    wire hbit; 
    wire[DATA_WIDTH-1:0] us_dataIn;
    assign hbit = (dataIn[DATA_WIDTH-1] == 1) ? 1'b0 : 1'b1;
    assign us_dataIn = { hbit, dataIn[DATA_WIDTH-2:0]};

    wire out_hbit;
    assign out_hbit = ( us_dsoutdata[DATA_WIDTH-1] == 1) ? 1'b0 : 1'b1;
    assign s_dsoutdata = { out_hbit, us_dsoutdata[DATA_WIDTH-2:0] };



    always @(posedge clk) begin
        if(rst)
            ds_counter <= 0;
        else if(ena)
            ds_counter <= ds_counter + 1; 
    end

    always @(posedge clk) begin
        if(rst)
            dsoutdata <= 0;
        else if(!ena)
            dsoutdata <= 0;
        else if(ds_counter == 0)
            dsoutdata <= us_dataIn;
        else 
            dsoutdata <= dsoutdata + us_dataIn;
    end
    assign us_dsoutdata = dsoutdata >> SAMPLE_RATE;

endmodule