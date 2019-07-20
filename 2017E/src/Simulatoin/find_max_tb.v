module find_max_tb();

reg clk,rst;

reg ena,wea;
reg[6:0] addr;
reg[6:0] addr_r;
reg[6:0] addr_r2;
reg[7:0] dout_r;

initial begin
    rst <= 1;
    clk <= 1;
    ena <= 0;
    wea <= 0;
    addr <= 0;
    # 20 
    rst <= 0;
    finisha <= 1;
    # 20
    finisha <= 0;



    // ena <= 1;    !!!: 在这里如果使用延时,驱动本来应该用时钟驱动的信号,可能会丢拍子

    // # 1
    // ena <= 1
end 

always begin
    # 5 
    clk <= ~clk;
end

// ------------------------------
wire[7:0] temp_max_data;
wire[6:0] temp_max_addr;

wire[7:0] dout;

find_max FIND_MAX_0 (
    .clk_in(clk),
    .rst(rst),
    .ena(ena),
    .data_in(dout),
    .addr_in(addr_r2),

    .temp_max_data(temp_max_data),
    .temp_max_addr(temp_max_addr),
    .max_data_valid(max_data_valid),
    .max_addr_valid(max_addr_valid),
    .finishb(finishb),
    .no_max_flag(no_max_flag)
);



reg finished;

always @(posedge clk) begin
    if (rst) begin
        addr <= 0;
        addr_r <= 0;
        addr_r2 <= 0;
        finished <= 0;
    end
    else begin
        if (!finished) begin
            ena <= 1;   // Not RST, Starting Reading (FOR RAM)
            dout_r <= dout;   // Save The Last element 
            addr_r <= addr;
            addr_r2 <= addr_r;
            addr <= addr + 1;
            if (addr_r2 > 0) begin
                ena <= 1;
            end
            if (addr_r2 == 127) begin
                ena <= 0;
                finished <= 1;
            end
        end

        else begin
            ena <= 0;
        end
        
        
    end
end

// ------Using RAM ------
/*
blk_mem_gen_0 RAM_0 (
  .clka(clk),    // input wire clka
  .ena(ena),      // input wire ena
  .wea(wea),      // input wire [0 : 0] wea
  .addra(addr),  // input wire [6 : 0] addr
  .dina(dina),    // input wire [7 : 0] dina
  .douta(dout)  // output wire [7 : 0] dout
);
*/

// --------Using PINGPONGRAM --------
// Cause In RAM We Use addr as addr
// But In PINGPONGRAM We should fill It into Port B To Read
// So We Make Drop addr
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
    .readyb(m)
);

endmodule
