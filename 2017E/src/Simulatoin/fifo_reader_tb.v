module fifo_reader_tb();
// first Write 10 Data 2 FIFO
// Then Start fifo_read, read The 10 Value
reg clk,rst;
reg[8:0] cnt;       // cnt from 1 to Data Num


reg signed[13:0] data_in[0:127]; 
wire[DATA_WIDTH-1:0] din;
assign din = data_in[cnt-1];

parameter DATA_WIDTH = 14;
parameter DATA_NUM = 128;

always begin
    # 5
    clk <= ~clk;
end

initial begin
    rst <= 1;
    clk <= 0;
    # 20
    rst <= 0;
    $readmemh("C:/Users/A_suozhang/Code/EE/Matlab/sin_wave.txt",data_in,0,127);
end

reg[1:0] state;
reg s_axis_tready;
reg write_done;

always@(posedge clk) begin
    if (rst) begin
        state <= 0;
        cnt <= 0;
        s_axis_tready <= 0;
        write_done <= 0;
        wr_en <= 0;
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
                wr_en <= 1;
                cnt <= cnt + 1;
                if (cnt  == DATA_NUM) begin
                    state <= 2;
                    wr_en <= 0;
                    write_done <= 1;
                end
            end
            2'b10: begin    // Read From FIFO
                s_axis_tready <= 1;
                state <= 0;
            end
        endcase
    end
end

// -------------- The FIFO ----------------
wire[DATA_WIDTH-1:0] dout;
wire[9:0] data_count;
reg wr_en;
fifo_generator_0 FIFO_0 (
  .clk(clk),                // input wire clk
  .srst(rst),              // input wire srst
  .din(din),                // input wire [15 : 0] din
  .wr_en(wr_en),            // input wire wr_en
  .rd_en(rd_en),            // input wire rd_en
  .dout(dout),              // output wire [15 : 0] dout
  .full(full),              // output wire full
  .empty(empty),            // output wire empty
  .data_count(data_count)  // output wire [9 : 0] data_count
);


// -----------------The FIFO Reader ----------------
wire[DATA_WIDTH-1:0] s_axis_tdata;


fifo_reader #(DATA_NUM,DATA_WIDTH) FIFO_READER_0(
    .clk(clk),
    .rst(rst),
    .dout(dout),
    .rd_en(rd_en),
    .empty(empty),
    .s_axis_tready(s_axis_tready & s_axis_data_tready_o),
    .s_axis_tvalid(s_axis_tvalid),
    .s_axis_tlast(s_axis_tlast),
    .s_axis_tdata(s_axis_tdata)
);

// ---------------- FFT ---------------------
wire m_axis_data_tready;
assign m_axis_data_tready = 1;

wire signed[23:0] fft_re, fft_im;
wire[47:0] m_axis_data_tdata;
assign fft_re = m_axis_data_tdata[47:24];
assign fft_im = m_axis_data_tdata[23:0];
wire[63:0] fft_amp;
assign fft_amp = fft_re*fft_re + fft_im*fft_im;


FFT_WRAPPER FFT_WRAPPER_0(
    .clk(clk),
    .rst(rst),
    .rst_done(rst_done),
    .s_axis_data_tdata({0,0,s_axis_tdata}),          // High 16 Is Re
    .s_axis_data_tvalid(s_axis_tvalid),
    .s_axis_data_tready_o(s_axis_data_tready_o),
    .s_axis_data_tlast(s_axis_tlast),

     .m_axis_data_tdata(m_axis_data_tdata),
     .m_axis_data_tvalid(m_axis_data_tvalid),
     .m_axis_data_tready(m_axis_data_tready),
     .m_axis_data_tlast(m_axis_data_tlast),

     .event_frame_started(event_frame_started),
     .event_tlast_unexpected(event_tlast_unexpected),
     .event_tlast_missing(event_tlast_missing),
     .event_status_channel_halt(event_status_channel_halt),
     .event_data_in_channel_halt(event_data_in_channel_halt),
     .event_data_out_channel_halt(event_data_out_channel_halt)

);



endmodule