module fifo_reader_tb();
// first Write 10 Data 2 FIFO
// Then Start fifo_read, read The 10 Value
reg clk,rst;
reg[3:0] cnt;
wire[15:0] data_in,din;
assign data_in = cnt+2;
assign din = data_in;

parameter DATA_WIDTH = 16;

always begin
    # 5
    clk <= ~clk;
end

initial begin
    rst <= 1;
    clk <= 0;
    # 20
    rst <= 0;
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
                if (cnt  == 10) begin
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



// -----------------The FIFO Reader ----------------
wire[DATA_WIDTH-1:0] dout;
wire[DATA_WIDTH-1:0] s_axis_tdata;

fifo_reader #(10,16) FIFO_READER_0(
    .clk(clk),
    .rst(rst),
    .dout(dout),
    .rd_en(rd_en),
    .empty(empty),
    .s_axis_tready(s_axis_tready),
    .s_axis_tvalid(s_axis_tvalid),
    .s_axis_tlast(s_axis_tlast),
    .s_axis_tdata(s_axis_tdata)
);




// -------------- The FIFO ----------------
wire[9:0] data_count;
reg wr_en;
fifo_generator_0 your_instance_name (
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

endmodule