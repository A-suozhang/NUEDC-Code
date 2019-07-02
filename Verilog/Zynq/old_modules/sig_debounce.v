module sig_debounce(clk_in,sig_in,rst_n,sig_out);
input wire clk_in;
input wire sig_in;
input wire rst_n;

output reg sig_out;

reg signal;
reg signal_r;
wire signal_negedge;
reg frozen;
reg[15:0] freeze_cnt; 

always @(posedge clk_in) begin 
	signal <= sig_in;
	signal_r <= signal;
end

assign signal_negedge = (signal_r)&(!signal);

always @(posedge clk_in) begin 
	if (signal_negedge) begin 
		freeze_cnt <= 16'b0000000000000011;
		frozen <= 1;
	end
	else begin 
		freeze_cnt <= freeze_cnt - 1;
		if (freeze_cnt == 0) begin 
			frozen <= 0;
		end
		else begin 
			frozen <= frozen;
		end
	end
end

always @(posedge clk_in or negedge rst_n)
	if(~rst_n) begin
		frozen <= 0;
		freeze_cnt <= 0;
		sig_out <= 0;
	end else begin
		if (frozen) begin 
			sig_out <= 0;
		end
		else begin
			sig_out <= signal;
		end
end

endmodule // sig_debounce