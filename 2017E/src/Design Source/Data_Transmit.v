module Data_Transmit (
    input wire clk,
    input wire rst,

    input wire data_valid,
    input wire[24:0] data,
    input wire[10:0] addr,

    input wire RX,
    output wire TX

);

// 3+25+12 = 8*5
reg[7:0] DataPack[0:4];

// Note That This Is For Test ONLY!!
// reg data_valid;

// Load The Result, Encode It, Then Send
always @(posedge clk) begin
    if (rst) begin
        DataPack[0] <= 0;
        DataPack[1] <= 0;
        DataPack[2] <= 0;
        DataPack[3] <= 0;
        DataPack[4] <= 0;
        // data_valid <= 1;
        
    end
    else begin
        if (data_valid) begin
            DataPack[0] <= {3'b0, data[24:20]};
            DataPack[1] <= data[19:12];
            DataPack[2] <= data[11:4];
            DataPack[3] <= {data[3:0],addr[10:7]};
            DataPack[4] <= {addr[6:0],1'b0};

            // --- For Test -----
            // DataPack[0] <= 48;
            // DataPack[1] <= 49;
            // DataPack[2] <= 50;
            // DataPack[3] <= 51;
            // DataPack[4] <= 52;
        
        end
        
    end
    
end


// ---------------------  Read Data From Uart_RX --------------------
//    rd_en HIGH so dout will read From Uart FIFO
//    If RX_FIFO not empty will read From RX FIFO

reg[2:0] wait_time;
reg rd_en;
reg state;


always@(posedge clk) begin
    if(rst) begin
      state <= 0;
      rd_en <= 0;
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

    end
end

// The UATY-RX With FIFO
// Receiving rx_Serial_data
// Decode & Fed Into FIFO
// Read The FIFO Outside The Module
(*mark_debug = "true"*) wire[7:0] dout;
uart_with_fifo_rx uart_with_fifo_rx_0(
    .clk_in(clk),
    .rst(rst),
    .rx_en(rx_en),
    .rx_serial_data(RX),
    .rd_en(rd_en),
    .dout(dout),
    .almost_empty(almost_empty),
    .empty(empty)
);

// -------------------- Write Data To TX FIFO -------------
reg[2:0] tx_cnt;
reg wr_en;
reg tx_state;

always@(posedge clk) begin
    if (rst) begin
        tx_cnt <= 0;
        wr_en <= 0;
        din <= 0;
        tx_state <= 0;
    end
    else begin
        case(tx_state) 
            1'b0: begin
                wr_en <= 0;
                if (rd_en) begin
                    tx_state <= 1;
                end
            end
            1'b1: begin
                if (dout == 8'd48) begin
                // When Recieved '0' Send Data
                    if (tx_cnt == 5) begin
                        wr_en <= 0;
                        tx_cnt <= 0;
                        tx_state <= 0;
                    end
                    else begin
                        tx_cnt <= tx_cnt+1;
                        wr_en <= 1;
                        din = DataPack[tx_cnt];
                    end
                end
            end
        endcase
    end
end

// The UART-TX With FIFO
// Reading The Processed Data Into FIFO
// Send Out From tx_serial_data
(*mark_debug = "true"*) reg[7:0] din;
uart_with_fifo_tx uart_with_fifo_tx_0(
  .clk_in(clk),
  .rst(rst),
  .tx_en(tx_en),
  .wr_en(wr_en),
  .din(din),
  .full(full),
  .almost_full(almost_full),
  .tx_serial_data(TX)
);


// Generate The Baud
baud_gen baud_gen0(
    .clk_in(clk),
    .tx_en(tx_en),
    .rx_en(rx_en)
);


endmodule


