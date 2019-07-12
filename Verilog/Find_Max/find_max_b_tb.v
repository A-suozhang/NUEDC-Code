module find_max_b_tb();

parameter DATA_WIDTH = 8;
parameter ADDR_WIDTH = 6;

reg clk,rst;

initial begin
    rst <= 1;
    clk <= 0;
    Data_in[0] = 1;
    Data_in[1] = 2;
    Data_in[2] = 3;
    Data_in[3] = 4;
    Data_in[4] = 5;
    Data_in[5] = 4;
    Data_in[6] = 3;
    Data_in[7] = 2;
    Data_in[8] = 1;
    Data_in[9] = 2;
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



// -------------------------

wire s_axis_tready;
wire m_axis_tvalid;
wire[DATA_WIDTH-1:0] m_axis_tdata_0;
wire[DATA_WIDTH-1:0] m_axis_tdata_1;
wire[DATA_WIDTH-1:0] m_axis_tdata_2;
wire[ADDR_WIDTH-1:0] m_axis_taddr;

find_max_b #(.DATA_WIDTH(8),.ADDR_WIDTH(6)) FIND_MAX_B_0(
    .clk_in(clk),
    .rst(rst),

    .s_axis_tvalid(s_axis_tvalid),
    .s_axis_tlast(s_axis_tlast),
    .s_axis_tdata(s_axis_tdata),
    .s_axis_taddr(s_axis_taddr),
    .s_axis_tready(s_axis_tready),

    .m_axis_tready(1'b01),
    .m_axis_tvalid(m_axis_tvalid),
    .m_axis_tdata_0(m_axis_tdata_0),
    .m_axis_tdata_1(m_axis_tdata_1),
    .m_axis_tdata_2(m_axis_tdata_2),
    .m_axis_taddr(m_axis_taddr)
);
endmodule

