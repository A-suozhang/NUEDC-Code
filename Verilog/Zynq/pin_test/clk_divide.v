module clk_divide(clk_in,rst,clk_out);

input wire clk_in;
input wire rst;
output reg clk_out;

reg[2:0] cnt;

always@(posedge clk_in) begin
    if (rst) begin
        cnt <= 3'b0;
        clk_out <= 1'b0;
    end
    else begin
      cnt <= cnt + 1;
      if (cnt == 3'b0) begin
        clk_out <= ~clk_out;
      end
    end
end

endmodule
