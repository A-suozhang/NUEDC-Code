module PingPongRAM_tb();

reg clk;
reg rst;

always begin
    # 10
    clk <= ~clk;
end

initial begin
    clk <= 0;
    rst <= 1;
    # 50
    rst <= 0;
    # 30
    finisha <= 1;
    # 20
    finisha <= 0;
    #20
    finisha <= 1;
    finishb <= 1;
end


reg[6:0] addra;
reg[7:0] dina;
reg wea; 
reg finisha, finishb;

reg[6:0] addrb;
wire[7:0] doutb; 

PINGPONG_RAM PINGPONG_RAM0(
    .clka(clk),
    .rsta(rst),
    .addra(addra),
    .wea(wea),
    .dina(dina),
    .finisha(finisha),
    .readya(readya),

    .clkb(clk),
    .rstb(rst),
    .addrb(addrb),
    .doutb(doutb),
    .finishb(finishb),
    .readyb(readyb)
);

endmodule