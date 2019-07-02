module led_test(clk_in,rst,swtich_on,led_on);

input wire clk_in;
input wire rst;
input wire swtich_on;
output reg led_on;

always@ (posedge clk_in) begin
  if (rst) begin
    led_on <= 0;
  end
  else begin
    if (switch_on) begin
      led_on <= 1;
    end
    else begin
      led_on <= 0;
    end
  end
end

endmodule