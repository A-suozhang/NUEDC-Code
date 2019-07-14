module fifo_reader#(parameter num_to_read = 10, DATA_WIDTH = 16)(
    input wire clk,
    input wire rst,
    input wire [DATA_WIDTH-1:0] dout,
    output reg rd_en,
    input wire empty,
    input wire s_axis_tready,       
    output wire s_axis_tvalid,
    output reg s_axis_tlast,
    output wire [DATA_WIDTH-1:0]s_axis_tdata  
);
// ------------- Implementing The FIFO To AXI4 ---------------------------
// -------------_ Reading X Data ---------------------------------------

assign s_axis_tdata = (~rst_done)?{(DATA_WIDTH-1){1'b0}}:dout;  // If not rst_done, tdataout = 0, else = dout
assign s_axis_tvalid = (~rst_done)?0:rd_en;         // If Not rst_done, s_axis_tavlid = 0, else = rd_en


reg[$clog2(num_to_read):0] cnt;       // Based On The Parameter
reg rst_done;

always@(posedge clk) begin
    // For The First Time
    // FIFO will read an invalid Data From Former Module
    // If fifo is not empty
    // Will Read An Invalid Value
    // So We Read One In Advance
    if (rst)  begin
        rst_done <= 0;
        rd_en <= 0;
        cnt <= 0;
        
    end
    else begin
        if (!rst_done) begin
            // For The first Time held rd_en HIGH for 1 clk
            if (!empty) begin
                rd_en <= 1;
            end

            if (rd_en) begin
                rd_en <= 0;
                rst_done <= 1;
            end 
        end

        // After Reset Done, Read N Data 
        else begin
            if (s_axis_tready) begin            // If The Latter Module Is Ready, Start Giving
                
                if (cnt == num_to_read-1) begin
                    s_axis_tlast <= 1;
                    cnt <= cnt + 1;
                end
                else if (cnt == num_to_read) begin
                    s_axis_tlast <= 0;
                    rd_en <= 0;
                end
                else begin
                    rd_en <= 1;
                    cnt <= cnt + 1;
                    s_axis_tlast = 0;
                end
                
            end
            else begin                          // If The Latter Module Is Not Ready, DONT Read FIFO
                rd_en <= 0;
            end
            
        end
           
    end
end


// // -------------- The FIFO ----------------
// fifo_generator_0 your_instance_name (
//   .clk(clk),                // input wire clk
//   .srst(rst),              // input wire srst
//   .din(din),                // input wire [15 : 0] din
//   .wr_en(wr_en),            // input wire wr_en
//   .rd_en(rd_en),            // input wire rd_en
//   .dout(dout),              // output wire [15 : 0] dout
//   .full(full),              // output wire full
//   .empty(empty),            // output wire empty
//   .data_count(data_count)  // output wire [9 : 0] data_count
// );


endmodule