module mul_tb();

reg clk,rst;
reg signed[7:0] data_a;
reg signed[7:0] data_b;

always begin
    #5
    clk <= ~clk;
end

initial begin
    clk <= 0;
    rst <= 1;
    #20
    rst <= 0;
    data_a <= 1;
    data_b <= -1;
    #10
    data_a <= 2;
    data_b <= -2;
end

wire signed[15:0] out;
mult_gen_0 MULT_0 (
  .CLK(clk),  // input wire CLK
  .A(data_a),      // input wire [7 : 0] A
  .B(data_b),      // input wire [7 : 0] B
  .P(out)      // output wire [15 : 0] P
);




endmodule
