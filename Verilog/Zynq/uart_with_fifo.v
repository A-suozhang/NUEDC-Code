module uart_with_fifo ()
input wire clk_in;
input wire rst;
input wire wr_en;
input wire[7:0] data_in;    // Other Module's Input To FIFO
input wire rx_serial_data;  // Data Coming From FIFO

output wire[7:0] data_out;  // Data Loaded From FIFO
output wire tx_serial_data; // Data Coming Out FROM UART
output wire fifo_full_error; // When Data is Full/Empty, Throw Data Error
// ...

// Variables For FIFO
reg[1:0] state_0; 
reg[1:0] state_1; 
wire wr_en_0,rd_en_0,full_0,empty_0,almost_empty_0,almost_full_0;
wire wr_en_0,rd_en_1,full_0,empty_1,almost_empty_1,almost_full_1;
reg[9:0] data_count_0;
reg[9:0] data_count_1;
reg[7:0] din_0;
reg[7:0] din_1;
reg[7:0] dout_0;
reg[7:0] dout_0;



// ---data_in(din_0)---|FIFO0|--(dout_0)---()----|Uart_TX|---(serial_clk_tx)-------

// ---(serial_clk_rx)----|Uart_RX|----()----(din_1)------|FIFO1|---(dout_1)----data_out---

// Acts For FIFO0
assign wr_en_0 = wr_en;     // When Input Wr_en = 1, Start Writting Into FIFO

always@(posedge clk_in) begin

    if (rst) begin
        wr_en_0 <= 0;
        rd_en_0 <= 0;

    end

    else begin
        

        end
        
    end
end




// --------The FIFO For TX (Writing)----------------
fifo_generator_0 fifo0 (
  .clk(clk_in),                    // input wire clk
  .srst(rst),                  // input wire srst
  .din(din_0),                    // input wire [7 : 0] din
  .wr_en(wr_en_0),                // input wire wr_en
  .rd_en(rd_en_0),                // input wire rd_en
  .dout(dout_0),                  // output wire [7 : 0] dout
  .full(full_0),                  // output wire full
  .almost_full(almost_full_0),    // output wire almost_full
  .empty(empty_0),                // output wire empty
  .almost_empty(almost_empty_0),  // output wire almost_empty
  .data_count(data_count_0)      // output wire [9 : 0] data_count
);

// --------The FIFO For RX (Reading)----------------
fifo_generator_0 fifo1 (
  .clk(clk_in),                    // input wire clk
  .srst(rst),                  // input wire srst
  .din(din_1),                    // input wire [7 : 0] din
  .wr_en(wr_en_1),                // input wire wr_en
  .rd_en(rd_en_1),                // input wire rd_en
  .dout(dout_1),                  // output wire [7 : 0] dout
  .full(full_1),                  // output wire full
  .almost_full(almost_full_1),    // output wire almost_full
  .empty(empty_1),                // output wire empty
  .almost_empty(almost_empty_1),  // output wire almost_empty
  .data_count(data_count_1)      // output wire [9 : 0] data_count
);


// -------------Uart_Variables-----------
wire tx_en, rx_en;
wire tx_data_en, tx_finish;
wire rx_finish;

reg[7:0] tx_data_in;
reg[7:0] rx_data;

// -----------------Uart-----------------
uart_tx uart_tx0(
    .clk_in(clk_in),
    .tx_en(tx_en),
    .rst(rst),
    .tx_data_en(tx_data_en),
    .tx_data_in(tx_data_in),
    .tx_finish(tx_finish),
    .tx_serial_data(tx_serial_data)
);

uart_rx uart_rx0(
    .clk_in(clk_in),
    .rx_en(rx_en),
    .rx_serial_data(rx_serial_data),
    .rx_finish(rx_finish),
    .rx_data(rx_data)
);

baud_gen baud_gen0(
    .clk_in(clk_in),
    .tx_en(tx_en),
    .rx_en(rx_en)
);


endmodule