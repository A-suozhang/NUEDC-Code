module E_top
(
    input clk_in,
    input rst_in,
    // ADC
    output adc_clkP_out_P,
    output adc_clkM_out_N,
    input adc_clkPout_in_P,
    input adc_clkMout_in_N,
    output adc_ctrl1_out,
    output adc_ctrl2_out,
    output adc_ctrl3_out,
    output adc_reset_out,
    output adc_sclk_out,
    output adc_sdata_out,
    input adc_sdout_in,
    output adc_sen_out,
    input [6:0] adc_dataA_in_P,
    input [6:0] adc_dataA_in_N,
    input [6:0] adc_dataB_in_P,
    input [6:0] adc_dataB_in_N,
    
    //DAC
    // output[AD_WIDTH-1:0] dac_dataA_out,
    // output[AD_WIDTH-1:0] dac_dataB_out,
    // output dac_clkA_out,
    // output dac_clkB_out,
    // output dac_gset_out,
    // output dac_mode_out,
    // output dac_sleep_out,


    input SW3,
    output led_on,
    input RX,
    output TX
    );




parameter  AD_WIDTH = 14;
parameter  SAMPLE_RATE = 7;
parameter DATA_NUM = 2048;
parameter DATA_WIDTH = 24;
parameter ADDR_WIDTH = $clog2(DATA_NUM);;

assign led_on = ~SW3;

// ------- The CLK_WIZARD & RST_PULSE -------------
clk_wiz_0 clk_wiz_test(
    .clk_out1(clk),
    .reset(!rst_in),
    .locked(clklocked),
    .clk_in1(clk_in)
);

rstpulse rstpulse(
    .clk_in(clk_in),
    .clk_sys(clk),
    .rst_in(rst_in),
    .rst(rst)
);
  

// ----------- The ADC DRIVER ----------------
(*mark_debug="true"*)wire signed[AD_WIDTH-1:0] adcdatA;
(*mark_debug="true"*)wire signed[AD_WIDTH-1:0] adcdatB;    
wire clkadc;
ADC_DRIVER adcdriver_inst(
    .clk(clk),
    .clkadc(clkadc),
    .rst(rst),
    .clkadclocked(clkadclocked),
    .adc_clkP_out_P(adc_clkP_out_P),
    .adc_clkM_out_N(adc_clkM_out_N),
    .adc_clkPout_in_P(adc_clkPout_in_P),
    .adc_clkMout_in_N(adc_clkMout_in_N),
    .adc_ctrl1_out(adc_ctrl1_out),
    .adc_ctrl2_out(adc_ctrl2_out),
    .adc_ctrl3_out(adc_ctrl3_out),
    .adc_reset_out(adc_reset_out),
    .adc_sclk_out(adc_sclk_out),
    .adc_sdata_out(adc_sdata_out),
    .adc_sdout_in(adc_sdout_in),
    .adc_sen_out(adc_sen_out),
    .adc_dataA_in_P(adc_dataA_in_P),
    .adc_dataA_in_N(adc_dataA_in_N),
    .adc_dataB_in_P(adc_dataB_in_P),
    .adc_dataB_in_N(adc_dataB_in_N),
    .adcdatA(adcdatA),
    .adcdatB(adcdatB)
);

// ------------- THE ADC DOWNSAMPLE --------------------
(*mark_debug="true"*)wire signed[AD_WIDTH-1:0] dsoutdata0;  // Only Take The HIGH 14 Bit
DOWNSAMP #(
    .SAMPLE_RATE(SAMPLE_RATE),
    .DATA_WIDTH(AD_WIDTH)
)
downsamp_0
(
    .clk                             ( clkadc    ),
    .rst                             ( rst       ),
    .ena                             ( 1         ),
    .dataIn                          ( adcdatA ),
    .s_dsoutdata                     ( dsoutdata0 ),
    .out_en                          ( out_en0    ),

    .outbusy                         (inwr_rst_busy0)
); 

(*mark_debug="true"*)wire signed[AD_WIDTH-1:0] dsoutdata1;
DOWNSAMP #(
    .SAMPLE_RATE(SAMPLE_RATE),
    .DATA_WIDTH(AD_WIDTH)
)
downsamp_1
(
    .clk                             ( clkadc    ),
    .rst                             ( rst       ),
    .ena                             ( 1         ),
    .dataIn                          ( adcdatB ),
    .s_dsoutdata                     ( dsoutdata1 ),
    .out_en                          ( out_en1    ),

    .outbusy                         (inwr_rst_busy1)
); 

// ------------- The Input ADC FIFO -------------
wire[11:0] wr_data_count0;
(*mark_debug="true"*)wire[11:0] rd_data_count0;
wire signed[13:0] dout0;

adc_fifo adc_fifo_0 (
  .rst(rst),                      // input wire rst
  .wr_clk(clkadc),                // input wire wr_clk
  .rd_clk(clk),                // input wire rd_clk
  // Only Make The HIGH 16 Bit For FIFO
  .din(dsoutdata0),  // input wire [13 : 0] din
  .wr_en(out_en0),                  // input wire wr_en
  .rd_en(rd_en0),                  // input wire rd_en
  .dout(dout0),                    // output wire [13 : 0] dout
  .almost_full(full0),      // output wire almost_full
  .almost_empty(empty0),    // output wire almost_empty
  .rd_data_count(rd_data_count0),  // output wire [10 : 0] rd_data_count
  .wr_data_count(wr_data_count0),  // output wire [10 : 0] wr_data_count
  .wr_rst_busy(wr_rst_busy0),      // output wire wr_rst_busy
  .rd_rst_busy(rd_rst_busy0)      // output wire rd_rst_busy
);


wire[11:0] wr_data_count1;
wire[11:0] rd_data_count1;
wire signed[13:0] dout1;

adc_fifo adc_fifo_1 (
  .rst(rst),                      // input wire rst
  .wr_clk(clkadc),                // input wire wr_clk
  .rd_clk(clk),                // input wire rd_clk
  .din(dsoutdata1),                      // input wire [13 : 0] din
  .wr_en(out_en1),                  // input wire wr_en
  .rd_en(rd_en1),                  // input wire rd_en
  .dout(dout1),                    // output wire [13 : 0] dout
  .almost_full(full1),      // output wire almost_full
  .almost_empty(empty1),    // output wire almost_empty
  .rd_data_count(rd_data_count1),  // output wire [11 : 0] rd_data_count
  .wr_data_count(wr_data_count1),  // output wire [11 : 0] wr_data_count
  .wr_rst_busy(wr_rst_busy1),      // output wire wr_rst_busy
  .rd_rst_busy(rd_rst_busy1)      // output wire rd_rst_busy
);

// -----------------The FIFO Reader 0----------------
(*mark_debug="true"*)wire signed[13:0] s_axis_tdata0;
wire s_axis_tready0;
(*mark_debug="true"*)assign s_axis_tready0 = (rd_data_count0 >= DATA_NUM & !rd_rst_busy0)?1:0; 


fifo_reader #(DATA_NUM,14) FIFO_READER_0(
    .clk(clk),
    .rst(rst),          // If The FFT Procedure Is Done, RST The FIFO, To Avoid FFT Data contains both Old and New
    .dout(dout0),
    .rd_en(rd_en0),
    .empty(empty0),
    .s_axis_tready(s_axis_tready0 & s_axis_data_tready_o0),
    .s_axis_tvalid(s_axis_tvalid0),
    .m_axis_data_tlast(m_axis_data_tlast0),
    .s_axis_tlast(s_axis_tlast0),
    .s_axis_tdata(s_axis_tdata0)
);
 
// -----------------The FIFO Reader 1----------------
(*mark_debug="true"*)wire signed[13:0] s_axis_tdata1;
wire s_axis_tready1;
assign s_axis_tready1 = (rd_data_count1 >= DATA_NUM & !rd_rst_busy1)?1:0; 

fifo_reader #(DATA_NUM,14) FIFO_READER_1(
    .clk(clk),
    .rst(rst),        // If The FFT Procedure Is Done, RST The FIFO, To Avoid FFT Data contains both Old and New
    .dout(dout1),
    .rd_en(rd_en1),
    .empty(empty1),
    .s_axis_tready(s_axis_tready1 & s_axis_data_tready_o1),
    .s_axis_tvalid(s_axis_tvalid1),
    .m_axis_data_tlast(m_axis_data_tlast1),
    .s_axis_tlast(s_axis_tlast1),
    .s_axis_tdata(s_axis_tdata1)
);

// ---------------- FFT 0 ---------------------
wire m_axis_data_tready0;
assign m_axis_data_tready0 = 1;

wire signed[23:0] fft_re0, fft_im0;
wire[63:0] m_axis_data_tdata0;
assign fft_re0 = m_axis_data_tdata0[63:39]; // Only Take High 24 Of 32 Bit 
assign fft_im0 = m_axis_data_tdata0[31:7];
// wire[63:0] fft_amp0;
// assign fft_amp0 = fft_re0*fft_re0 + fft_im0*fft_im0;
wire[31:0] s_axis_tdata_fft0;
assign s_axis_tdata_fft0 = {2'b0,s_axis_tdata0,16'b0};


FFT_WRAPPER#(32,64) FFT_WRAPPER_0(
    .clk(clk),
    .rst(rst),
    .rst_done(rst_done0),
    .s_axis_data_tdata(s_axis_tdata_fft0),          // High 16 Is Re
    .s_axis_data_tvalid(s_axis_tvalid0),
    .s_axis_data_tready_o(s_axis_data_tready_o0),
    .s_axis_data_tlast(s_axis_tlast0),

     .m_axis_data_tdata(m_axis_data_tdata0),
     .m_axis_data_tvalid(m_axis_data_tvalid0),
     .m_axis_data_tready(m_axis_data_tready0),
     .m_axis_data_tlast(m_axis_data_tlast0),

     .event_frame_started(event_frame_started0),
     .event_tlast_unexpected(event_tlast_unexpected0),
     .event_tlast_missing(event_tlast_missing0),
     .event_status_channel_halt(event_status_channel_halt0),
     .event_data_in_channel_halt(event_data_in0_channel_halt0),
     .event_data_out_channel_halt(event_data_out_channel_halt0)

);

// ---------------- FFT 1 ---------------------
wire m_axis_data_tready1;
assign m_axis_data_tready1 = 1;

wire signed[23:0] fft_re1, fft_im1;
wire[63:0] m_axis_data_tdata1;
assign fft_re1 = m_axis_data_tdata1[63:39];
assign fft_im1 = m_axis_data_tdata1[31:7];
// wire[63:0] fft_amp1;
// assign fft_amp1 = fft_re1*fft_re1 + fft_im1*fft_im1;
wire[31:0] s_axis_tdata_fft1;
assign s_axis_tdata_fft1 = {2'b0,s_axis_tdata1,16'b0};


FFT_WRAPPER#(32,64) FFT_WRAPPER_1(
    .clk(clk),
    .rst(rst),
    .rst_done(rst_done1),
    .s_axis_data_tdata(s_axis_tdata_fft1),          // High 16 Is Re
    .s_axis_data_tvalid(s_axis_tvalid1),
    .s_axis_data_tready_o(s_axis_data_tready_o1),
    .s_axis_data_tlast(s_axis_tlast1),

     .m_axis_data_tdata(m_axis_data_tdata1),
     .m_axis_data_tvalid(m_axis_data_tvalid1),
     .m_axis_data_tready(m_axis_data_tready1),
     .m_axis_data_tlast(m_axis_data_tlast1),

     .event_frame_started(event_frame_started1),
     .event_tlast_unexpected(event_tlast_unexpected1),
     .event_tlast_missing(event_tlast_missing1),
     .event_status_channel_halt(event_status_channel_halt1),
     .event_data_in_channel_halt(event_data_in0_channel_halt1),
     .event_data_out_channel_halt(event_data_out_channel_halt1)

);


// ------------ The FFT 2 Cordic Data Format Transform -------------
// The Cordic Core requires the input frcational be in [-1,1], and has 2 integer bit 00(first sign bit)
// So The 01_xxxx or 10_xxxx is illegal For Cordic Input (But This Could Happen With Common signed reg Data)
// Actually Since The Input for Our Module(FFT) Is 14 bit Length, The Output Won't Be That Long(24 bit)

// ------- The Square 2 Amplotude Module 0-----------
wire[DATA_WIDTH*2-1:0] s_axis_cartesian_tdata0;  // Itsef Not Signed, But The Elements In It Are Signed
assign s_axis_cartesian_tdata0 = {fft_im0, fft_re0};
wire[DATA_WIDTH*2-1:0] m_axis_dout_tdata0;
wire signed[DATA_WIDTH-1:0] m_axis_dout_tdata_amp0;
(*mark_debug="true"*)wire signed[DATA_WIDTH-11:0] m_axis_dout_tdata_amp0_debug;
assign m_axis_dout_tdata_amp0 = m_axis_dout_tdata0[DATA_WIDTH-1:0];
assign m_axis_dout_tdata_amp0_debug = m_axis_dout_tdata0[DATA_WIDTH-1:10];
 
cordic_0 CORDIC_00 (
  .aclk(clk),                                        // input wire aclk
  .s_axis_cartesian_tvalid(m_axis_data_tvalid0),  // input wire s_axis_cartesian_tvalid
  .s_axis_cartesian_tlast(m_axis_data_tlast0),    // input wire s_axis_cartesian_tlast
  .s_axis_cartesian_tdata(s_axis_cartesian_tdata0),    // input wire [47 : 0] s_axis_cartesian_tdata
  .m_axis_dout_tvalid(m_axis_dout_tvalid0),            // output wire m_axis_dout_tvalid
  .m_axis_dout_tlast(m_axis_dout_tlast0),              // output wire m_axis_dout_tlast
  .m_axis_dout_tdata(m_axis_dout_tdata0)              // output wire [47 : 0] m_axis_dout_tdata
);

// ------- The Square 2 Amplotude Module 1-----------
wire[DATA_WIDTH*2-1:0] s_axis_cartesian_tdata1;  // Itsef Not Signed, But The Elements In It Are Signed
assign s_axis_cartesian_tdata1 = {fft_im1, fft_re1};
wire[DATA_WIDTH*2-1:0] m_axis_dout_tdata1;
wire signed[DATA_WIDTH-1:0] m_axis_dout_tdata_amp1;
(*mark_debug="true"*)wire signed[DATA_WIDTH-11:0] m_axis_dout_tdata_amp1_debug;
assign m_axis_dout_tdata_amp1 = m_axis_dout_tdata1[DATA_WIDTH-1:0];
assign m_axis_dout_tdata_amp1_debug = m_axis_dout_tdata1[DATA_WIDTH-1:10];

cordic_0 CORDIC_01 (
  .aclk(clk),                                        // input wire aclk
  .s_axis_cartesian_tvalid(m_axis_data_tvalid1),  // input wire s_axis_cartesian_tvalid
  .s_axis_cartesian_tlast(m_axis_data_tlast1),    // input wire s_axis_cartesian_tlast
  .s_axis_cartesian_tdata(s_axis_cartesian_tdata1),    // input wire [47 : 0] s_axis_cartesian_tdata
  .m_axis_dout_tvalid(m_axis_dout_tvalid1),            // output wire m_axis_dout_tvalid
  .m_axis_dout_tlast(m_axis_dout_tlast1),              // output wire m_axis_dout_tlast
  .m_axis_dout_tdata(m_axis_dout_tdata1)              // output wire [47 : 0] m_axis_dout_tdata
);

wire[DATA_WIDTH:0] m_axis_dout_tdata_amp_sub;
wire[DATA_WIDTH-1:0] m_axis_dout_tdata_amp;
(*mark_debug="true"*)wire[DATA_WIDTH-11:0] m_axis_dout_tdata_amp_debug;

assign m_axis_dout_tdata_amp_sub = m_axis_dout_tdata_amp0 - m_axis_dout_tdata_amp1;
// m_axis_dout_tdata_amp_sub is 1 more bit wider 
// So The Amp Equals amp_sub >> 1
assign m_axis_dout_tdata_amp = m_axis_dout_tdata_amp_sub[DATA_WIDTH:1];
assign m_axis_dout_tdata_amp_debug = m_axis_dout_tdata_amp_sub[DATA_WIDTH-1:10];

wire m_axis_dout_tlast, m_axis_dout_tvalid;
assign m_axis_dout_tvalid = m_axis_dout_tvalid0 & m_axis_dout_tvalid1;
assign m_axis_dout_tlast = m_axis_dout_tlast0 & m_axis_dout_tlast1;


// ------- The Find Max Module -----------
wire s_axis_tready0_findmax;
wire m_axis_tvalid;
wire signed [DATA_WIDTH-1:0] m_axis_tdata_0;
wire signed [DATA_WIDTH-1:0] m_axis_tdata_1;
wire signed [DATA_WIDTH-1:0] m_axis_tdata_2;
(*mark_debug="true"*)wire[ADDR_WIDTH-1:0] m_axis_taddr;

// The Find Max Module's Input is Signed
// The Output Signed
find_max_b #(.DATA_WIDTH(DATA_WIDTH),.ADDR_WIDTH(ADDR_WIDTH)) FIND_MAX_B_0(
    .clk_in(clk),
    .rst(rst),

    .s_axis_tvalid(m_axis_dout_tvalid),
    .s_axis_tlast(m_axis_dout_tlast),
    .s_axis_tdata(m_axis_dout_tdata_amp),  // The Amp
    // .s_axis_taddr(s_axis_taddr),
    .s_axis_tready(s_axis_tready0_findmax),

    .m_axis_tready(1'b1),
    .m_axis_tvalid(m_axis_tvalid),
    .m_axis_tdata_0(m_axis_tdata_0),
    .m_axis_tdata_1(m_axis_tdata_1),
    .m_axis_tdata_2(m_axis_tdata_2),
    .m_axis_taddr(m_axis_taddr)
);

// --------- The Mult And Add Module -----------
reg signed[DATA_WIDTH-1:0] data_0;
reg signed[DATA_WIDTH-1:0] data_1;
reg signed[DATA_WIDTH-1:0] data_2;
wire signed[2*DATA_WIDTH-1:0] square_0;
wire signed[2*DATA_WIDTH-1:0] square_1;
wire signed[2*DATA_WIDTH-1:0] square_2;
wire signed[2*DATA_WIDTH+1:0] square_sum;     // 2*DATA_WIDTH-1+2
assign square_sum = square_0 + square_1 + square_2;

// ------------------ AXI4 Stream 2 Mult & ADD --------------------------
reg m_axis_tvalid_r, m_axis_tvalid_r2, m_axis_tvalid_r3, m_axis_tvalid_r4;

always@(posedge clk) begin
    if (rst) begin
        data_0 <= 0;
        data_1 <= 0;
        data_2 <= 0;
        m_axis_tvalid_r <= 0;
        m_axis_tvalid_r2 <= 0;
        m_axis_tvalid_r3 <= 0;
        m_axis_tvalid_r4 <= 0;
    end
    else begin
        if (m_axis_tvalid) begin
            data_0 <= m_axis_tdata_0;
            data_1 <= m_axis_tdata_1;
            data_2 <= m_axis_tdata_2;    
        end
        // Also Give m_axis_data to data0 last 1 clk
        // After 3 clk, The square_sum will be valid
        m_axis_tvalid_r <= m_axis_tvalid;
        m_axis_tvalid_r2 <= m_axis_tvalid_r;
        m_axis_tvalid_r3 <= m_axis_tvalid_r2;
        m_axis_tvalid_r4 <= m_axis_tvalid_r3;
    end
    
end

// The Module Has 3 clk Latency(Could Modify In IP)
mult_gen_0 MULT_0 (        
  .CLK(clk),  // input wire CLK
  .A(data_0),      // input wire [23 : 0] A
  .B(data_0),      // input wire [23 : 0] B
  .P(square_0)      // output wire [47 : 0] P
);

mult_gen_0 MULT_1 (
  .CLK(clk),  // input wire CLK
  .A(data_1),      // input wire [23 : 0] A
  .B(data_1),      // input wire [23 : 0] B
  .P(square_1)      // output wire [47 : 0] P
);

mult_gen_0 MULT_2 (
  .CLK(clk),  // input wire CLK
  .A(data_2),      // input wire [23 : 0] A
  .B(data_2),      // input wire [23 : 0] B
  .P(square_2)      // output wire [47 : 0] P
);


// ---------------------- The Root Module ------------------------------------
// The Input Data Square Sum Data Width 50, The Max Data Width 48, Right Shift 2
wire root_valid;
(*mark_debug="true"*)wire[24:0] root_result; // The Data Width From Cordic IP Core
(*mark_debug="true"*)wire[11:0] root_result_debug;
assign root_result_debug = root_result[24:13];
cordic_1 CORDIC_1 (
  .aclk(clk),                                        // input wire aclk
  .s_axis_cartesian_tvalid(m_axis_tvalid_r4),  // input wire s_axis_cartesian_tvalid
  .s_axis_cartesian_tdata(square_sum[49:2]),    // input wire [47 : 0] s_axis_cartesian_tdata
  .m_axis_dout_tvalid(root_valid),            // output wire m_axis_dout_tvalid
  .m_axis_dout_tdata(root_result)              // output wire [31 : 0] m_axis_dout_tdata
);


Data_Transmit data_transmit_0(
    .clk(clk),
    .rst(rst),
    .data_valid(root_valid),
    .data(root_result),
    .addr(m_axis_taddr),

    .RX(RX),
    .TX(TX)

);


endmodule

