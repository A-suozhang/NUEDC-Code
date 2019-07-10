module cordic_tb( );

reg aclk = 0;

always begin
    # 10
    aclk <= ~aclk;
end




// -----------------------------------------------------------------------------------------
// ------------------------------ CORDIC ARCTAN MODE ---------------------------------------
// -----------------------------------------------------------------------------------------

/*

reg s_axis_cartesian_tvalid;
reg[7:0] s_axis_cartesian_tdata_y,s_axis_cartesian_tdata_x;
wire[15:0] s_axis_cartesian_tdata;
assign s_axis_cartesian_tdata = {s_axis_cartesian_tdata_y, s_axis_cartesian_tdata_x};

initial begin
    s_axis_cartesian_tvalid <= 0;
    s_axis_cartesian_tdata_x <= 0;
    s_axis_cartesian_tdata_y <= 0;
    # 50
    s_axis_cartesian_tvalid <= 1;
    s_axis_cartesian_tdata_x <= 8'b00_100000;
    s_axis_cartesian_tdata_y <= 8'b00_100000;
    # 20
    s_axis_cartesian_tvalid <= 1;
    s_axis_cartesian_tdata_x <= 8'b00_110000;
    s_axis_cartesian_tdata_y <= 8'b00_100000;
    # 20
    s_axis_cartesian_tvalid <= 1;
    s_axis_cartesian_tdata_x <= 8'b00_111000;
    s_axis_cartesian_tdata_y <= 8'b00_100000;
    # 20
    s_axis_cartesian_tvalid <= 1;
    s_axis_cartesian_tdata_x <= 8'b00_111100;
    s_axis_cartesian_tdata_y <= 8'b00_100000;
    # 20
    s_axis_cartesian_tvalid <= 0;
end



wire[7:0] m_axis_dout_tdata;
wire m_axis_dout_tvalid;

cordic_0 CORDIC_0 (
  .aclk(aclk),                                        // input wire aclk
  .s_axis_cartesian_tvalid(s_axis_cartesian_tvalid),  // input wire s_axis_cartesian_tvalid
  .s_axis_cartesian_tdata(s_axis_cartesian_tdata),    // input wire [15 : 0] s_axis_cartesian_tdata
  .m_axis_dout_tvalid(m_axis_dout_tvalid),            // output wire m_axis_dout_tvalid
  .m_axis_dout_tdata(m_axis_dout_tdata)              // output wire [7 : 0] m_axis_dout_tdata
);
*/


// -----------------------------------------------------------------------------------------
// ------------------------ CORDIC VECTOR ROTATION MODE ------------------------------------
// -----------------------------------------------------------------------------------------

/*
reg s_axis_cartesian_tvalid, s_axis_phase_tvalid;
wire[15:0] s_axis_cartesian_tdata;
reg[7:0] s_axis_cartesian_tdata_y;
reg[7:0] s_axis_cartesian_tdata_x;
assign s_axis_cartesian_tdata = {s_axis_cartesian_tdata_y, s_axis_cartesian_tdata_x};
reg[7:0] s_axis_phase_tdata; 

initial begin
    s_axis_phase_tvalid <= 0;
    s_axis_phase_tdata <= 0;
    s_axis_cartesian_tvalid <= 0;
    s_axis_cartesian_tdata_y <= 0;
    s_axis_cartesian_tdata_x <= 0;
    # 50 
    s_axis_phase_tvalid <= 1;
    s_axis_cartesian_tvalid <= 1;
    s_axis_cartesian_tdata_x <= 8'b00_100000;   // x0 = 0.5
    s_axis_cartesian_tdata_x <= 8'b00_010000;   // x0 = 0.25
    //s_axis_phase_tdata <= 8'b000_11001;         // pi/4
    // s_axis_phase_tdata <= 8'b111_00111;        // -pi/4
    s_axis_phase_tdata <= 8'b000_10000;           // pi/6
    # 20
    s_axis_phase_tdata <= 8'b010_01011;           // 3/4pi
    # 20
    s_axis_cartesian_tvalid <= 0;
    s_axis_phase_tvalid <= 0;

end

reg[7:0] test;
always@(posedge aclk) begin
    if (m_axis_dout_tvalid) begin
        test <= m_axis_dout_tdata_x;
    end
end

wire[15:0] m_axis_dout_tdata;
wire[7:0] m_axis_dout_tdata_y;
wire[7:0] m_axis_dout_tdata_x;

// assign m_axis_dout_tdata = {m_axis_dout_tdata_y, m_axis_dout_tdata_x};
assign m_axis_dout_tdata_y = m_axis_dout_tdata[15:8];
assign m_axis_dout_tdata_x = m_axis_dout_tdata[7:0];


cordic_0 CORDIC_0 (
  .aclk(aclk),                                        // input wire aclk
  .s_axis_phase_tvalid(s_axis_phase_tvalid),          // input wire s_axis_phase_tvalid
  .s_axis_phase_tdata(s_axis_phase_tdata),            // input wire [7 : 0] s_axis_phase_tdata
  .s_axis_cartesian_tvalid(s_axis_cartesian_tvalid),  // input wire s_axis_cartesian_tvalid
  .s_axis_cartesian_tdata(s_axis_cartesian_tdata),    // input wire [15 : 0] s_axis_cartesian_tdata
  .m_axis_dout_tvalid(m_axis_dout_tvalid),            // output wire m_axis_dout_tvalid
  .m_axis_dout_tdata(m_axis_dout_tdata)              // output wire [15 : 0] m_axis_dout_tdata
);

*/

// -----------------------------------------------------------------------------------------
// ------------------------ CORDIC VECTOR TRANSITION MODE ------------------------------------
// -----------------------------------------------------------------------------------------
wire[15:0] s_axis_cartesian_tdata;
reg[7:0] s_axis_cartesian_tdata_y;
reg[7:0] s_axis_cartesian_tdata_x;
reg s_axis_cartesian_tvalid;
assign s_axis_cartesian_tdata = {s_axis_cartesian_tdata_y, s_axis_cartesian_tdata_x};

initial begin
    s_axis_cartesian_tvalid <= 0;
    s_axis_cartesian_tdata_y <= 0;
    s_axis_cartesian_tdata_x <= 0;
    # 50 
    s_axis_cartesian_tvalid <= 1;
    s_axis_cartesian_tdata_y <= 8'b00_101101;   // x0 = sqrt(2)/2
    s_axis_cartesian_tdata_x <= 8'b00_101101;   // y0 = sqrt(2)/2
    # 20
    s_axis_cartesian_tdata_y <= 8'b00_010000;   // x0 = 0.25
    s_axis_cartesian_tdata_x <= 8'b00_000000;   // y0 = 0
    # 20
    s_axis_cartesian_tvalid <= 0;

end

wire[15:0] m_axis_dout_tdata;
wire[7:0] m_axis_dout_tdata_amp;
wire[7:0] m_axis_dout_tdata_phase;
assign m_axis_dout_tdata_phase = m_axis_dout_tdata[15:8];
assign m_axis_dout_tdata_amp = m_axis_dout_tdata[7:0];

cordic_0 CORDIC_0 (
  .aclk(aclk),                                        // input wire aclk
  .s_axis_cartesian_tvalid(s_axis_cartesian_tvalid),  // input wire s_axis_cartesian_tvalid
  .s_axis_cartesian_tdata(s_axis_cartesian_tdata),    // input wire [15 : 0] s_axis_cartesian_tdata
  .m_axis_dout_tvalid(m_axis_dout_tvalid),            // output wire m_axis_dout_tvalid
  .m_axis_dout_tdata(m_axis_dout_tdata)              // output wire [15 : 0] m_axis_dout_tdata
); 



endmodule
