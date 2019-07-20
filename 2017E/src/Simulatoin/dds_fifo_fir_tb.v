
module dds_fifo_fir_tb();

reg clk;
reg rst;

// Generating A 100MHz CLK to dirve module
always begin
    # 5
    clk <= ~clk;
end

initial begin
    clk <= 0;
    rst <= 1;
    # 50 
    rst <= 0;
end




// -----------MAIN LOGIC ---------------------
// -------FIFO 2 AXI4 -----------------------


// Write DDS(Self Made module) Output to FIFO
// If Former Module TVALID
//  WR_EN -> HIGH
wire m_axis_data_tvalid_dds;
wire read_empty, write_full;
assign m_axis_data_tvalid_dds = m_axis_data_tvalid_dds0 & m_axis_data_tvalid_dds1;  // If Both DDS data valid
assign wr_en = m_axis_data_tvalid_dds & !full;
assign write_full = m_axis_data_tdata_dds & full;
wire signed [15:0] m_axis_data_tdata_dds;
wire signed [7:0] m_axis_data_tdata_dds0, m_axis_data_tdata_dds1;
assign m_axis_data_tdata_dds = m_axis_data_tdata_dds0 + m_axis_data_tdata_dds1; // dds = dds_0 + dds_1
// wire[15:0] din;
// assign din = m_axis_data_tdata_dds;
assign rd_en = s_axis_data_tready & !empty;
assign read_empty = s_axis_data_tready & empty;


// DOUT will Appear After The cycle, May Need To Latch
wire signed [15:0] dout;

wire signed [31:0] m_axis_data_tdata;
fir_compiler_0 FIR_0 (
  .aclk(clk),                              // input wire aclks
  .s_axis_data_tvalid(!empty),  // input wire s_axis_data_tvalid
  .s_axis_data_tready(s_axis_data_tready),  // output wire s_axis_data_tready
  .s_axis_data_tdata(dout),    // input wire [15 : 0] s_axis_data_tdata
  .m_axis_data_tvalid(m_axis_data_tvalid),  // output wire m_axis_data_tvalid
  .m_axis_data_tdata(m_axis_data_tdata)    // output wire [31 : 0] m_axis_data_tdata
);


wire[9:0] data_count;


fifo_generator_0 FIFO_0 (
  .clk(clk),                // input wire clk
  .srst(rst),              // input wire srst
  .din(m_axis_data_tdata_dds),                // input wire [15 : 0] din
  .wr_en(wr_en),            // input wire wr_en
  .rd_en(rd_en),            // input wire rd_en
  .dout(dout),              // output wire [15 : 0] dout
  .full(full),              // output wire full
  .empty(empty),            // output wire empty
  .data_count(data_count)  // output wire [9 : 0] data_count
);

wire[15:0] m_axis_phase_tdata_dds0;
dds_compiler_0 DDS_0 (
  .aclk(clk),                                // input wire aclk
  .m_axis_data_tvalid(m_axis_data_tvalid_dds0),    // output wire m_axis_data_tvalid
  .m_axis_data_tdata(m_axis_data_tdata_dds0),      // output wire [7 : 0] m_axis_data_tdata
  .m_axis_phase_tvalid(m_axis_phase_tvalid_dds0),  // output wire m_axis_phase_tvalid
  .m_axis_phase_tdata(m_axis_phase_tdata_dds0)    // output wire [15 : 0] m_axis_phase_tdata
);

wire[15:0] m_axis_phase_tdata_dds1;
dds_compiler_1 DDS_1 (
  .aclk(clk),                                // input wire aclk
  .m_axis_data_tvalid(m_axis_data_tvalid_dds1),    // output wire m_axis_data_tvalid
  .m_axis_data_tdata(m_axis_data_tdata_dds1),      // output wire [7 : 0] m_axis_data_tdata
  .m_axis_phase_tvalid(m_axis_phase_tvalid_dds1),  // output wire m_axis_phase_tvalid
  .m_axis_phase_tdata(m_axis_phase_tdata_dds1)    // output wire [15 : 0] m_axis_phase_tdata
);

endmodule