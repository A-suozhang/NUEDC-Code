module find_max_b_tb();

parameter DATA_WIDTH = 24;
parameter ADDR_WIDTH = 8;
parameter NUM_TO_READ = 128;

reg clk,rst;


reg[DATA_WIDTH-1:0] Data_in_re[0:NUM_TO_READ-1];
reg[DATA_WIDTH-1:0] Data_in_im[0:NUM_TO_READ-1];


initial begin
    rst <= 1;
    clk <= 0;
    $readmemh("C:/Users/A_suozhang/Code/EE/Verilog/Read_Text/test_re.txt",Data_in_re,0,NUM_TO_READ-1);
    $readmemh("C:/Users/A_suozhang/Code/EE/Verilog/Read_Text/test_im.txt",Data_in_im,0,NUM_TO_READ-1);
    // Data_in[0] = 8'b00_101100;
    // Data_in[1] = 8'b00_101101;
    // Data_in[2] = 8'b00_101110;
    // Data_in[3] = 8'b00_101111;
    // Data_in[4] = 8'b00_101111;
    // Data_in[5] = 8'b00_101101;
    // Data_in[6] = 8'b00_101110;
    // Data_in[7] = 8'b00_101101;
    // Data_in[8] = 8'b00_101101;
    // Data_in[9] = 8'b00_101100;

    # 20
    rst <= 0;
end

always begin
    # 5
    clk <= ~clk;
end


// --------- The TB To Feed Data In Via AXI4 -----------------
reg signed [DATA_WIDTH-1:0] s_axis_tdata_re;
reg signed [DATA_WIDTH-1:0] s_axis_tdata_im;
reg[ADDR_WIDTH-1:0] s_axis_taddr;
reg s_axis_tvalid, s_axis_tlast;

always@(posedge clk) begin
    if (rst) begin
        s_axis_tvalid <= 0;
        s_axis_taddr <= 0;
        s_axis_tdata_re <= 0;
        s_axis_tdata_im <= 0;
        s_axis_tlast <= 0;
    end
    else begin
        s_axis_taddr <= s_axis_taddr + 1;
        s_axis_tdata_re <= Data_in_re[s_axis_taddr];
        s_axis_tdata_im <= Data_in_im[s_axis_taddr];
        if (s_axis_taddr < NUM_TO_READ-1) begin
            s_axis_tvalid <= 1;
        end
        else if(s_axis_taddr == NUM_TO_READ-1)begin
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
// ------------ The FFT 2 Cordic Data Format Transform -------------
// The Cordic Core requires the input frcational be in [-1,1], and has 2 integer bit 00(first sign bit)
// So The 01_xxxx or 10_xxxx is illegal For Cordic Input (But This Could Happen With Common signed reg Data)
// Actually Since The Input for Our Module(FFT) Is 14 bit Length, The Output Won't Be That Long(24 bit)


// ------- The Square 2 Amplotude Module -----------
wire[DATA_WIDTH*2-1:0] s_axis_cartesian_tdata;  // Itsef Not Signed, But The Elements In It Are Signed
assign s_axis_cartesian_tdata = {s_axis_tdata_im, s_axis_tdata_re};
wire[DATA_WIDTH*2-1:0] m_axis_dout_tdata;
wire signed[DATA_WIDTH-1:0] m_axis_dout_tdata_amp;
assign m_axis_dout_tdata_amp = m_axis_dout_tdata[DATA_WIDTH-1:0];
cordic_0 CORDIC_0 (
  .aclk(clk),                                        // input wire aclk
  .s_axis_cartesian_tvalid(s_axis_tvalid),  // input wire s_axis_cartesian_tvalid
  .s_axis_cartesian_tlast(s_axis_tlast),    // input wire s_axis_cartesian_tlast
  .s_axis_cartesian_tdata(s_axis_cartesian_tdata),    // input wire [47 : 0] s_axis_cartesian_tdata
  .m_axis_dout_tvalid(m_axis_dout_tvalid),            // output wire m_axis_dout_tvalid
  .m_axis_dout_tlast(m_axis_dout_tlast),              // output wire m_axis_dout_tlast
  .m_axis_dout_tdata(m_axis_dout_tdata)              // output wire [47 : 0] m_axis_dout_tdata
);


// ------- The Find Max Module -----------
wire s_axis_tready;
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
    .s_axis_tready(s_axis_tready),

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

