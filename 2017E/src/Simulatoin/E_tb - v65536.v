module E_tb();

// first Write 10 Data 2 FIFO
// Then Start fifo_read, read The 10 Value
reg clk,rst;
reg[16:0] cnt;       // cnt from 1 to Data Num


reg signed[13:0] data_in0[0:DATA_NUM]; 
reg signed[13:0] data_in1[0:DATA_NUM]; 
wire[13:0] din0;
wire[13:0] din1;
assign din0 = data_in0[cnt-1];
assign din1 = data_in1[cnt-1];

parameter DATA_WIDTH = 24;
parameter DATA_NUM = 65536;
parameter ADDR_WIDTH = 16;

always begin
    # 5
    clk <= ~clk;
end

initial begin
    rst <= 1;
    clk <= 0;
    # 20
    rst <= 0;
    $readmemh("C:/Users/A_suozhang/Code/EE/Matlab/sin_wave0.txt",data_in0,0,DATA_NUM);
    $readmemh("C:/Users/A_suozhang/Code/EE/Matlab/sin_wave1.txt",data_in1,0,DATA_NUM);
    
end

reg[1:0] state;
reg s_axis_tready0;
reg write_done;

always@(posedge clk) begin
    if (rst) begin
        state <= 0;
        cnt <= 0;
        s_axis_tready0 <= 0;
        write_done <= 0;
        wr_en0 <= 0;
    end
    else begin
        case(state)
            2'b00: begin    // IDLE
                if (!write_done) begin
                    state <= 1;
                end
                else begin
                    state <= 0;
                end
                
            end
            2'b01: begin    // Write Into FIFO
                wr_en0 <= 1;
                cnt <= cnt + 1;
                if (cnt  == DATA_NUM) begin
                    state <= 2;
                    wr_en0 <= 0;
                    write_done <= 1;
                end
            end
            2'b10: begin    // Read From FIFO
                s_axis_tready0 <= 1;
                state <= 0;
            end
        endcase
    end
end

// -------------- The FIFO 0 ----------------
wire[13:0] dout0;
wire[16:0] data_count0;
reg wr_en0;
fifo_generator_0 FIFO_0 (
  .clk(clk),                // input wire clk
  .srst(rst),              // input wire srst
  .din(din0),                // input wire [15 : 0] din0
  .wr_en(wr_en0),            // input wire wr_en0
  .rd_en(rd_en0),            // input wire rd_en0
  .dout(dout0),              // output wire [15 : 0] dout
  .full(full0),              // output wire full0
  .empty(empty0),            // output wire empty0
  .data_count(data_count0)  // output wire [9 : 0] data_count0
);

// -------------- The FIFO 1 ----------------
wire[13:0] dout1;
wire[16:0] data_count1;
fifo_generator_0 FIFO_1 (
  .clk(clk),                // input wire clk
  .srst(rst),              // input wire srst
  .din(din1),                // input wire [15 : 0] din0
  .wr_en(wr_en0),            // input wire wr_en0
  .rd_en(rd_en0),            // input wire rd_en0
  .dout(dout1),              // output wire [15 : 0] dout
  .full(full1),              // output wire full0
  .empty(empty1),            // output wire empty0
  .data_count(data_count1)  // output wire [9 : 0] data_count0
);


// -----------------The FIFO Reader 0----------------
wire[13:0] s_axis_tdata0;


fifo_reader #(DATA_NUM,14) FIFO_READER_0(
    .clk(clk),
    .rst(rst),
    .dout(dout0),
    .rd_en(rd_en0),
    .empty(empty0),
    .s_axis_tready(s_axis_tready0 & s_axis_data_tready_o0),
    .s_axis_tvalid(s_axis_tvalid0),
    .s_axis_tlast(s_axis_tlast0),
    .s_axis_tdata(s_axis_tdata0)
);

// -----------------The FIFO Reader 1----------------
wire[13:0] s_axis_tdata1;


fifo_reader #(DATA_NUM,14) FIFO_READER_1(
    .clk(clk),
    .rst(rst),
    .dout(dout1),
    .rd_en(rd_en0),
    .empty(empty1),
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
wire[63:0] fft_amp0;
assign fft_amp0 = fft_re0*fft_re0 + fft_im0*fft_im0;
wire[31:0] s_axis_tdata_fft0;
assign s_axis_tdata_fft0 = {2'b0,s_axis_tdata0,16'b0};


FFT_WRAPPER FFT_WRAPPER_0(
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
assign fft_re1 = m_axis_data_tdata1[63:49];
assign fft_im1 = m_axis_data_tdata1[31:7];
wire[63:0] fft_amp1;
assign fft_amp1 = fft_re1*fft_re1 + fft_im1*fft_im1;
wire[31:0] s_axis_tdata_fft1;
assign s_axis_tdata_fft1 = {2'b0,s_axis_tdata1,16'b0};


FFT_WRAPPER FFT_WRAPPER_1(
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

// ------------------ The Calibration Module -------------------
// Need To Do Some Math Here
// The Output Freq & The Output Amp Are Calculated





// --------------------- The DDS Module -----------------------
reg s_axis_config_tvalid_dds;
reg[31:0] s_axis_config_tdata_dds;
wire signed[7:0] m_axis_data_tdata_dds;
wire signed[31:0] m_axis_phase_tdata_dds;


// If The Calibration & Transition is Done, Give a Signal 
// Make The Freq & Amp



//  DDS_Config Word // 2684354 = 2^(28 - phase_width)*(f_0/f_clk)
dds_compiler_2 DDS_CONFIG_0 (
  .aclk(clk),                                  // input wire aclk
  .s_axis_config_tvalid(s_axis_config_tvalid_dds),  // input wire s_axis_config_tvalid
  .s_axis_config_tdata(s_axis_config_tdata_dds),    // input wire [31 : 0] s_axis_config_tdata
  .m_axis_data_tvalid(m_axis_data_tvalid_dds),      // output wire m_axis_data_tvalid
  .m_axis_data_tdata(m_axis_data_tdata_dds),        // output wire [7 : 0] m_axis_data_tdata
  .m_axis_phase_tvalid(m_axis_phase_tvalid_dds),    // output wire m_axis_phase_tvalid
  .m_axis_phase_tdata(m_axis_phase_tdata_dds)      // output wire [31 : 0] m_axis_phase_tdata
);

// The Amp Config Word(Unsigned - 16 bit)    amp = amp_config_word/2^16;
mult_gen_1 AMP_MULT_0 (
  .CLK(clk),  // input wire CLK
  .A(m_axis_data_tdata_dds),      // input wire [7 : 0] A
  .B(B),      // input wire [15 : 0] B
  .P(P)      // output wire [23 : 0] P
);





endmodule

