module find_max_reader(
    input wire clk_in,
    input wire rst,

    input wire readyb,
    input wire[7:0] dout,

    output wire  finishb,
    output reg[6:0] addr,
    
    output wire[7:0] temp_max_data,
    output wire[6:0] temp_max_addr,
    output wire max_data_valid,
    output wire max_addr_valid,
    output wire no_max_flag
);

reg readyb_r;
reg ena;
reg[6:0] addr_r, addr_r2;
wire posedge_readyb;
assign posedge_readyb = !readyb_r & readyb;


always @(posedge clk_in) begin
    if (rst) begin
        readyb_r <= 0;
        ena <= 0;
        addr <= 0;
        addr_r <= 0;
        addr_r2 <= 0;

    end 
    else begin
        readyb_r <= readyb;
        addr_r <= addr;
        addr_r2 <= addr_r;
        // Rise Ena HIGH only On Poseedge Readyb(Means That The B Port could Read)
        if (posedge_readyb) begin
            ena <= 1;
            addr <= addr + 1;
        end

        if (ena) begin
            addr <= addr + 1;
        end

        if (addr_r2 == 127) begin   // Based On The RAM Size
            addr <= 0;
            ena <= 0;
        end
        
        
    end   
end



// --------- The Find MAx Module ------------
find_max FIND_MAX_0 (
    .clk_in(clk_in),
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

endmodule