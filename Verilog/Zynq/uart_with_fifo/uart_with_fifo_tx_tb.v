module uart_with_fifo_tx_tb();

reg clk = 0;
reg rst = 1;
reg data2send_valid = 0;

always begin
  # 10
  clk <= ~clk;
end

/*
initial begin
    # 50
    rst <= 0;
end
*/

// TestSignal For uart_with_fifo_tx_regs
initial begin
  #50 
  rst <= 0;
  #50 
  data2send_valid <= 1;
  #20
  data2send_valid <= 0;
end



// Reading From uart_with_fifo_rx
reg[7:0] local_data;
reg[2:0] wait_time;
reg rd_en,wr_en;
reg state;

always@(posedge clk) begin
    if(rst) begin
      state <= 0;
      rd_en <= 0;
      local_data <= 0; 
      wait_time <= 0;
    end
    else begin
      case(state) 
          1'b0: begin
              rd_en <= 0;
              wait_time <= wait_time + 1;
              if (wait_time > 3'd5) begin
                if(!empty) begin   // empty will last 2 cycle.making state stop in idle for 1 cycle
                    state <= 1;
                end  
              end
                 
          end
          1'b1: begin
              wait_time <= 0;
              rd_en <= 1;
              state <= 0;
          end
      endcase

      if (rd_en) begin
          local_data <= dout;
          wr_en <= 1;       // Write The local Data Into The TX_FIFO
      end
      else begin
          wr_en <= 0;
          local_data <= local_data;
      end 
    end
end


// Writing FIFO From The LOCAL DATA
// UART_WITH_FIFO_TX_REGS
// Counld produce the test tx_serial_data_signal for uart_rx

uart_with_fifo_tx_regs uart_with_fifo_tx_regs_0(
    .clk_in(clk),
    .rst(rst),
    .data2send_valid(data2send_valid),
    .tx_serial_data(tx_serial_data)
);


wire[7:0] dout;
uart_with_fifo_rx uart_with_fifo_rx_0(
    .clk_in(clk),
    .rst(rst),
    .rx_serial_data(tx_serial_data),
    .rd_en(rd_en),
    .dout(dout),
    .almost_empty(almost_empty),
    .empty(empty)
);


wire[7:0] din;
assign din = dout;
wire tx_serial_data_0;
uart_with_fifo_tx uart_with_fifo_tx_0(
  .clk_in(clk),
  .rst(rst),
  .wr_en(wr_en),
  .din(din),
  .full(full),
  .almost_full(almost_full),
  .tx_serial_data(tx_serial_data_0)
);


baud_gen baud_gen0(
    .clk_in(clk),
    .tx_en(tx_en),
    .rx_en(rx_en)
);

wire[7:0] final_rx_data;
reg[7:0] rx_result;
uart_rx uart_rx0(
  .clk_in(clk),
  .rst(rst),
  .rx_en(rx_en),
  .rx_serial_data(tx_serial_data_0),
  .rx_finish(rx_finish),
  .rx_data(final_rx_data)
);

always@(posedge clk) begin
    if (rst) begin
      rx_result <= 0; 
    end
    else begin
        if(rx_finish) begin
            rx_result <= final_rx_data;
        end
    end
end




endmodule