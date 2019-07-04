module fifo_tb();

reg clk = 0;
reg srst = 0;
reg[7:0] din = 8'b00000000;
reg[7:0] din_r = 8'b00000000;
reg[7:0] din_rr = 8'b00000000;
reg[7:0] din_rrr = 8'b00000000;
wire[15:0] dout;
reg wr_en, rd_en;

reg[1:0] state = 2'b00;
reg[7:0] cnt = 8'h00;
wire[9:0] rd_data_count;
wire[8:0] wr_data_count;

parameter IDLE = 2'b00;
parameter WRITE = 2'b01;
parameter READ = 2'b10;
parameter WR = 2'b11;


always begin
  # 100
  clk <= ~clk;
end

initial begin
  srst = 1;
  #50
  srst = 0;
end

always@(posedge clk) begin
  if (srst) begin
    wr_en = 0;
    rd_en = 0;
    din = 8'b000000000;
  end
  else begin
    case (state)
        IDLE: begin
          // Do Nothing
            wr_en <= 0;
            rd_en <= 0;
        end 
        WRITE: begin
            wr_en <= 1;
            rd_en <= 0;
        end
        READ: begin
            rd_en <= 1;
            wr_en <= 0; 
        end
        WR: begin
            rd_en <= 1;
            wr_en <= 1;
        end
        default: begin
            // Do Nothing, All Data Latch
            wr_en <= 0;
            rd_en <= 0;
        end
    endcase
  end
end

always@(posedge clk) begin
  if (cnt < 10) begin
    state <= 0;
    din_r <= din;
    din_rr <= din_r;
    din_rrr <= din_rr;
  end
  else if (cnt < 110) begin
    state <= 1;
    din <= din +1;
    din_r <= din;
    din_rr <= din_r;
  end
  else if (cnt < 200) begin
    state <= 2;
    din_r <= din;
  end
  else begin
    din = 8'h00;
    din_r <= din;
  end
end

always@(posedge clk) begin
  if (cnt > 300) begin
    cnt <= 0;
  end  
  else begin
    cnt <= cnt+1;
  end
end




fifo_generator_1 fifo0 (
  .clk(clk),      // input wire clk
  .srst(srst),    // input wire srst
  .din(din),      // input wire [7 : 0] din
  .wr_en(wr_en),  // input wire wr_en
  .rd_en(rd_en),  // input wire rd_en
  .dout(dout),    // output wire [15 : 0] dout
  .full(full),    // output wire full
  .empty(empty),  // output wire empty
  .rd_data_count(rd_data_count),  // output wire [9 : 0] rd_data_count
  .wr_data_count(wr_data_count)  // output wire [10 : 0] wr_data_count
);

endmodule