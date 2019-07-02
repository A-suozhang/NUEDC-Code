`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2018/11/12 15:15:02
// Design Name: 
// Module Name: CLK_DIVIDER
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


module CLK_DIVIDER(
    input clkin,
    output clkout
    );
    reg [2:0]cnt=3'd0;
    reg clkout_r=1'b0;
    assign clkout=clkout_r;
    always@(posedge clkin)begin
        cnt<=cnt+3'd1;
        if(cnt==3'd0)begin
            clkout_r<=~clkout_r;
        end
    end
endmodule
