module generate_tb();

reg clk,rst;
reg[7:0] a,b;
wire[7:0] out;
parameter config_param = 0;

always begin
    #5 clk <= ~clk;
end

initial begin
    clk <= 0;
    rst <= 1;
    # 30 
    rst <= 0;
    a <= 8'b00011000;
    b <= 8'b11100111;
end

// ------------- Testing GENERATE IF STATEMENT ---------------
generate
    if(config_param) 
        And And0(a,b,out);
    else
        Or Or0(a,b,out);
endgenerate

endmodule
