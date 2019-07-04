module uart_with_fifo_tx_tb();

reg clk = 0;
reg rst = 1;
reg data2send_valid = 0;

always begin
  # 10
  clk <= ~clk;
end

initial begin
  #50 
  rst <= 0;
  #50 
  data2send_valid <= 1;
  #20
  data2send_valid <= 0;
end

uart_with_fifo_tx uart_with_fifo_tx_0(
    .clk_in(clk),
    .rst(rst),
    .data2send_valid(data2send_valid),
    .tx_serial_data(tx_serial_data)
);

endmodule