module dds_config_tb();

reg clk, rst;
reg s_axis_config_tvalid;
reg[31:0] s_axis_config_tdata;

always  begin
    #5
    clk <= ~clk;
end

initial begin
    rst <= 1;
    clk <= 0;
    # 20
    rst <= 0;
    # 10 
    s_axis_config_tvalid = 1;
    // phase width[see it in IP core, NOT THE SAME WITH the output width]
    // f0 = config*f_clk/(2^(phase_width)
    s_axis_config_tdata = 32'd2684354;      // 2684354 = 2^(28 - phase_width)*(f_0/f_clk)
    # 20
    s_axis_config_tvalid = 0;

end


wire signed[7:0] m_axis_data_tdata;
wire signed[31:0] m_axis_phase_tdata;
dds_compiler_2 DDS_CONFIG_0 (
  .aclk(clk),                                  // input wire aclk
  .s_axis_config_tvalid(s_axis_config_tvalid),  // input wire s_axis_config_tvalid
  .s_axis_config_tdata(s_axis_config_tdata),    // input wire [31 : 0] s_axis_config_tdata
  .m_axis_data_tvalid(m_axis_data_tvalid),      // output wire m_axis_data_tvalid
  .m_axis_data_tdata(m_axis_data_tdata),        // output wire [7 : 0] m_axis_data_tdata
  .m_axis_phase_tvalid(m_axis_phase_tvalid),    // output wire m_axis_phase_tvalid
  .m_axis_phase_tdata(m_axis_phase_tdata)      // output wire [31 : 0] m_axis_phase_tdata
);





endmodule