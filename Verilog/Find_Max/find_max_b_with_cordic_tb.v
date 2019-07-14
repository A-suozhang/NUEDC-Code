module find_max_b_tb();

parameter DATA_WIDTH = 8;
parameter ADDR_WIDTH = 6;

reg clk,rst;

initial begin
    rst <= 1;
    clk <= 0;
    Data_in[0] = 8'b00_101100;
    Data_in[1] = 8'b00_101101;
    Data_in[2] = 8'b00_101110;
    Data_in[3] = 8'b00_101111;
    Data_in[4] = 8'b00_101111;
    Data_in[5] = 8'b00_101101;
    Data_in[6] = 8'b00_101110;
    Data_in[7] = 8'b00_101101;
    Data_in[8] = 8'b00_101101;
    Data_in[9] = 8'b00_101100;

    # 20
    rst <= 0;
end

always begin
    # 5
    clk <= ~clk;
end

reg[DATA_WIDTH-1:0] Data_in[0:9];


reg[DATA_WIDTH-1:0] s_axis_tdata;
reg[ADDR_WIDTH-1:0] s_axis_taddr;
reg s_axis_tvalid, s_axis_tlast;

always@(posedge clk) begin
    if (rst) begin
        s_axis_tvalid <= 0;
        s_axis_taddr <= 0;
        s_axis_tdata <= 0;
        s_axis_tlast <= 0;
    end
    else begin
        s_axis_taddr <= s_axis_taddr + 1;
        s_axis_tdata <= Data_in[s_axis_taddr];
        if (s_axis_taddr < 9) begin
            s_axis_tvalid <= 1;
        end
        else if(s_axis_taddr == 9)begin
            s_axis_tlast <= 1;
            if (s_axis_tlast) begin
                s_axis_tlast <= 0;
                s_axis_tvalid <= 0;
            end
        end    
        else begin
            s_axis_tlast <= 0;
            s_axis_tvalid <= 0;
        end    
    end
end


// ------- The Cordic Module -----------
wire[DATA_WIDTH*2-1:0] s_axis_cartesian_tdata;
assign s_axis_cartesian_tdata = {0, s_axis_tdata};
wire[DATA_WIDTH*2-1:0] m_axis_dout_tdata;
wire[DATA_WIDTH-1:0] m_axis_dout_tdata_amp;
assign m_axis_dout_tdata_amp = m_axis_dout_tdata[DATA_WIDTH-1:0];
cordic_0 CORDIC_0 (
  .aclk(clk),                                        // input wire aclk
  .s_axis_cartesian_tvalid(s_axis_tvalid),  // input wire s_axis_cartesian_tvalid
  .s_axis_cartesian_tlast(s_axis_tlast),    // input wire s_axis_cartesian_tlast
  .s_axis_cartesian_tdata(s_axis_cartesian_tdata),    // input wire [15 : 0] s_axis_cartesian_tdata
  .m_axis_dout_tvalid(m_axis_dout_tvalid),            // output wire m_axis_dout_tvalid
  .m_axis_dout_tlast(m_axis_dout_tlast),              // output wire m_axis_dout_tlast
  .m_axis_dout_tdata(m_axis_dout_tdata)              // output wire [15 : 0] m_axis_dout_tdata
);




// ------- The Find Max Module -----------

wire s_axis_tready;
wire m_axis_tvalid;
wire[DATA_WIDTH-1:0] m_axis_tdata_0;
wire[DATA_WIDTH-1:0] m_axis_tdata_1;
wire[DATA_WIDTH-1:0] m_axis_tdata_2;
wire[ADDR_WIDTH-1:0] m_axis_taddr;

find_max_b #(.DATA_WIDTH(8),.ADDR_WIDTH(6)) FIND_MAX_B_0(
    .clk_in(clk),
    .rst(rst),

    .s_axis_tvalid(m_axis_dout_tvalid),
    .s_axis_tlast(m_axis_dout_tlast),
    .s_axis_tdata(m_axis_dout_tdata_amp),  // The Amp
    // .s_axis_taddr(s_axis_taddr),
    .s_axis_tready(s_axis_tready),

    .m_axis_tready(1'b01),
    .m_axis_tvalid(m_axis_tvalid),
    .m_axis_tdata_0(m_axis_tdata_0),
    .m_axis_tdata_1(m_axis_tdata_1),
    .m_axis_tdata_2(m_axis_tdata_2),
    .m_axis_taddr(m_axis_taddr)
);
endmodule

