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
    // ena <= 1;
end 

always begin
    # 5 
    clk <= ~clk;
end


// -----------------FIND MAX ----------------------

/*  temp_max must have an initial value, must use reg 
wire[7:0] temp_max_data;
assign temp_max_data = (dout >= temp_max_data)? dout:temp_max_data;
wire[6:0] temp_max_addr;
assign temp_max_addr = (dout >= temp_max_data)? addr:temp_max_addr;
*/
reg[7:0] temp_max_data;
reg[6:0] temp_max_addr;

// Start From 0, the actual cnt is multiple_max_cnt+1;
reg[7:0] multiple_max_cnt;
// Store 16 Same Max Value's ADDR
// The Otehr Max_ADDR was stored in these Regs (without the first one which was stored in temp_max_addr)
reg[6:0] multiple_max_addr[0:15]; 

reg multiple_max_overflow;

always @(posedge clk) begin
    if (rst) begin
        temp_max_data <= 0;
        temp_max_addr <= 0;
        multiple_max_cnt <= 0;
        multiple_max_overflow <= 0;
    end
    else begin
        if (ena) begin
            if (dout > temp_max_data) begin
                temp_max_data <= dout;
                multiple_max_cnt <= 0;              // If a new max enocunts, empty multiple_max
                temp_max_addr <= addr - 2;         // True addr Latch addr 2 (-2)   &&  find max procedure last 1 cycle (+1)
            end
            else if (dout == temp_max_data) begin

                // when First Encounts multiple max
                // if (multiple_max_cnt == 0) begin
                //     multiple_max_addr[0] <= temp_max_addr;  // Load Current addr into 
                //     multiple_max_cnt <= multiple_max_cnt + 1;
                // end
                // If Overflow (More than 16 same max)
                if (multiple_max_cnt == 15) begin
                    multiple_max_overflow <= 1;
                end
                else begin
                    multiple_max_cnt <= multiple_max_cnt + 1;
                    multiple_max_addr[multiple_max_cnt] <= addr - 2;
                end
                
            end
            else begin
                temp_max_data <= temp_max_data;
                temp_max_addr <= temp_max_addr;
                multiple_max_cnt <= multiple_max_cnt;
            end
        end
    end
end

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
            if (ena) begin
                addr <= addr + 1;
            end
            if (addr == 127) begin
                finished <= 1;
                ena <= 0;
            end
        end
        else begin
            ena <= 0;
        end
        
        
    end
end

wire[7:0] dout;
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
