module find_max_reader_tb();

reg clk,rst,wea;

always begin
    # 5 
    clk <= ~clk;
end

initial begin
    rst <= 1;
    clk <= 1;
    finisha <= 0;
    # 100 
    rst <= 0;
    # 100
    finisha <= 1;
    # 10
    finisha <= 0;

end 

wire[7:0] dout;
wire[6:0] addr;
wire[7:0] temp_max_data;
wire[6:0] temp_max_addr;

find_max_reader FIND_MAX_READER0(

    .clk_in(clk),
    .rst(rst),
    .readyb(readyb),
    .dout(dout),

    .finishb(finishb),
    .addr(addr),
    .temp_max_data(temp_max_data),
    .temp_max_addr(temp_max_addr),
    .max_data_valid(max_data_valid),
    .max_addr_valid(max_addr_valid),
    .no_max_flag(no_max_flag)

);

wire[7:0] dina;
wire[6:0] addra;
reg finisha;
PINGPONG_RAM PINGPONG_RAM_0(
    .clka(clk),
    .rsta(rst),
    .addra(addra),
    .wea(wea),
    .dina(dina),
    .finisha(finisha),
    .readya(readya),
    .clkb(clk),
    .rstb(rst),
    .addrb(addr),
    .doutb(dout),
    .finishb(finishb),
    .readyb(readyb)
);


endmodule