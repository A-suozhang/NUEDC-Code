module data_transmit_tb();

reg clk,rst;
reg[24:0] data;
reg[11:0] addr;
reg data_valid;


always begin
    #5 
    clk <= ~clk;
end

initial begin
    clk <= 0;
    rst <= 1;
    #20
    rst <= 0;
    # 10
    data <= 25'd1200;
    addr <= 12'd255;
    # 10 
    data_valid <= 1;
end

Data_Transmit DATA_TRANSMIT_0(
    .clk(clk),
    .rst(rst),

    .data_valid(data_valid),
    .data(data),
    .addr(addr),

    .RX(RX),
    .TX(TX)
);

endmodule
