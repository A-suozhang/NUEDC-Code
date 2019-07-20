module Data_Process#(parameter DATA_WIDTH = 24, DATA_NUM = 2048)(
    input wire clk,
    input wire rst,
    input wire ad_fifo_ready,   // ad_fifo1 & ad_fifo2 empty && not_rst_busy
    input wire empty0,
    input wire empty1,
    input wire[13:0] dout0,      // From dout1
    input wire[13:0] dout1,
    output wire rd_en,           // Read From FIFO At The Same Time 
    output wire m_axis_data_tlast,  // FFT Process Donw, Used For RST FIFO 


    output wire root_valid,
    output wire[24:0] root_result,
    output wire[ADDR_WIDTH-1:0] m_axis_taddr

);


parameter ADDR_WIDTH = $clog2(DATA_NUM);

wire s_axis_tready0;
assign s_axis_tready0 = ad_fifo_ready; // If The Former FIFO Not Empty



// ------------ Read Data From The Former FIFO ---------------
// -----------------The FIFO Reader 0----------------
wire[13:0] s_axis_tdata0;
fifo_reader #(DATA_NUM,14) FIFO_READER_0(
    .clk(clk),
    .rst(rst),
    .dout(dout0),
    .rd_en(rd_en0),
    .empty(empty0),
    .m_axis_data_tlast(m_axis_data_tlast0),
    .s_axis_tready(s_axis_tready0 & s_axis_data_tready_o0),
    .s_axis_tvalid(s_axis_tvalid0),
    .s_axis_tlast(s_axis_tlast0),
    .s_axis_tdata(s_axis_tdata0)
);

// -----------------The FIFO Reader 1----------------
wire[13:0] s_axis_tdata1;
fifo_reader #(DATA_NUM,14) FIFO_READER_1(
    .clk(clk),
    .rst(rst),          // If The FFT Procedure Is Done, RST The FIFO, To Avoid FFT Data contains both Old and New
    .dout(dout1),
    .rd_en(rd_en0),
    .empty(empty1),
    .m_axis_data_tlast(m_axis_data_tlast1),
    .s_axis_tready(s_axis_tready0 & s_axis_data_tready_o1),
    .s_axis_tvalid(s_axis_tvalid1),
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
    .rst_done(rst_done0),
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
assign m_axis_dout_tdata_amp0 = m_axis_dout_tdata0[DATA_WIDTH-1:0];
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
assign m_axis_dout_tdata_amp1 = m_axis_dout_tdata1[DATA_WIDTH-1:0];
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
assign m_axis_dout_tdata_amp_sub = m_axis_dout_tdata_amp0 - m_axis_dout_tdata_amp1;
// m_axis_dout_tdata_amp_sub is 1 more bit wider 
// So The Amp Equals amp_sub >> 1
assign m_axis_dout_tdata_amp = m_axis_dout_tdata_amp_sub[DATA_WIDTH:1];

wire m_axis_dout_tlast, m_axis_dout_tvalid;
assign m_axis_dout_tvalid = m_axis_dout_tvalid0 & m_axis_dout_tvalid1;
assign m_axis_dout_tlast = m_axis_dout_tlast0 & m_axis_dout_tlast1;


// ------- The Find Max Module -----------
wire s_axis_tready0_findmax;
wire m_axis_tvalid;
wire signed [DATA_WIDTH-1:0] m_axis_tdata_0;
wire signed [DATA_WIDTH-1:0] m_axis_tdata_1;
wire signed [DATA_WIDTH-1:0] m_axis_tdata_2;
wire[ADDR_WIDTH-1:0] m_axis_taddr;

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
wire[24:0] root_result; // The Data Width From Cordic IP Core
cordic_1 CORDIC_1 (
  .aclk(clk),                                        // input wire aclk
  .s_axis_cartesian_tvalid(m_axis_tvalid_r4),  // input wire s_axis_cartesian_tvalid
  .s_axis_cartesian_tdata(square_sum>>2),    // input wire [47 : 0] s_axis_cartesian_tdata
  .m_axis_dout_tvalid(root_valid),            // output wire m_axis_dout_tvalid
  .m_axis_dout_tdata(root_result)              // output wire [15 : 0] m_axis_dout_tdata
);




endmodule