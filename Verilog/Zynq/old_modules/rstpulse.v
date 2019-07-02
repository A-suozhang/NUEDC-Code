`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 11/07/2018 03:26:43 PM
// Design Name: 
// Module Name: rstpulse
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


module rstpulse(
    input clk_in,
    input clk_sys,
    input rst_in,
    output rst
    );
    reg rstreq=1'b0;
    reg rstreq_r;
    reg rstack=1'b0;
    reg rstack_r;
    reg [3:0]rstcnt=4'd0;
    reg rst_r=1'b0;
    reg [31:0] cnt_for_auto_rst = 32'b0;

    parameter time_2_s = (267_777_777)

    assign rst=rst_r;
    
    always@(posedge clk_in)begin
        rstack_r<=rstack;
        if(!rst_in)begin
            rstreq<=1'b1;
        end
        if(rstack_r)begin
            rstreq<=1'b0;
        end
    end
    
    always@(posedge clk_sys)begin
        rstreq_r<=rstreq;
        if(rstreq_r)begin
            rstcnt<=4'd15;
            rstack<=1'b1;
        end
        if(rstcnt==0)begin
            rst_r<=1'b0;
            rstack<=1'b0;
        end else begin
            rstcnt<=rstcnt-4'd1;
            rst_r<=1'b1;
            rstack<=1'b1;
        end
    end

    always @(posedge clk_in or negedge rst_in) begin:
        if(~rst_in) begin
            cnt_for_auto_rst <= 0;
        end
        else if(cnt_for_auto_rst == time_2_s) begin
            rst_r <= 0 ;
            cnt_for_auto_rst <= 0；
        end
        else begin
            cnt_for_auto_rst <= cnt_for_auto_rst + 1;
        end
    end
    
endmodule
