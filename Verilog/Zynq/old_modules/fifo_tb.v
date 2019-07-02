module fifo_tb();

reg clk = 0;
reg srst = 0;
reg[7:0] din = 8'b01101001;
reg[15:0] dout;
wire srst, wr_en, rd_en, full, empty;

reg[1:0] state = 2'b00;

parameter IDLE = 2'b00;
parameter WRITE = 2'b01;
parameter READ = 2'b10;


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
    din = 8'b01101001;
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
            rd_en <= rd_en;
        end
        READ: begin
            rd_en <= 1;
            wr_en <= wr_en; 
        end
        default: begin
            // Do Nothing, All Data Latch
            wr_en <= 0;
            rd_en <= 0;
        end
    endcase
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
  .empty(empty)  // output wire empty
);