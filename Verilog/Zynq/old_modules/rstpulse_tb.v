module  rstpulse_tb();

reg clk = 0;
reg clk_in = 0;
reg rst_in = 1;


initial begin 
	#100
	rst_in <= 0;
end

always begin 
	#10 clk <= ~clk;
end

always begin 
	if (!rst_in) begin 
		 #20 clk_in <= ~clk_in;
	end
end

rstpulse rst_inst(
        .clk_in(clk_in),
        .clk_sys(clk),
        .rst_in(rst_in),
        .rst(rst)
    );

endmodule // rstpulse_tb