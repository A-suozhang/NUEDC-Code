module find_max(clk_in,rst,data_in,addr_in,ena,temp_max_data,max_data_valid,temp_max_addr,max_addr_valid,finishb,no_max_flag);

input wire clk_in;
input wire rst;
input wire[7:0] data_in;
input wire[6:0] addr_in;
input ena;      // Denotes When The Work Is START/END

output reg[7:0] temp_max_data;
output reg max_data_valid;
output reg[6:0] temp_max_addr;
output reg max_addr_valid;
output reg finishb;
output reg no_max_flag;




// -----------------FIND MAX ----------------------
/*  temp_max must have an initial value, must use reg 
wire[7:0] temp_max_data;
assign temp_max_data = (data_in >= temp_max_data)? data_in:temp_max_data;
wire[6:0] temp_max_addr;
assign temp_max_addr = (data_in >= temp_max_data)? addr_in:temp_max_addr;
*/


// Start From 0, the actual cnt is multiple_max_cnt+1;
reg[7:0] multiple_max_cnt;
// Store 16 Same Max Value's addr_in
// The Otehr Max_addr_in was stored in these Regs (without the first one which was stored in temp_max_addr_in)
reg[6:0] multiple_max_addr[0:15]; 

reg multiple_max_overflow;
reg have_given_valid;

`define FIND_MAX 2'b00;
`define GIVE_VALID 2'b01;
`define GIVE_FINISH 2'b10;

reg ena_r;
always@(posedge clk_in) begin
    if (rst) begin
        ena_r <= 0;
    end
    else begin
        ena_r <= ena;
    end
end

wire negedge_ena;
assign negedge_ena = !ena & ena_r;

reg[1:0] state;


always @(posedge clk_in) begin
    if (rst) begin
        temp_max_data <= 0;
        temp_max_addr <= 0;
        state <= 0;
        multiple_max_cnt <= 0;
        multiple_max_overflow <= 0;
        max_data_valid <= 0;
        max_addr_valid <= 0;
        have_given_valid <= 0;
        finishb <= 0;
        no_max_flag <= 1;
    end
    else begin

        case(state)
            2'b00:  begin
                max_data_valid <= 0;
                max_addr_valid <= 0;
                // --------------------- The FIND_MAX STEP ------------------
                if (ena) begin
                    have_given_valid <= 0;
                    if (data_in > temp_max_data) begin
                        temp_max_data <= data_in;
                        multiple_max_cnt <= 0;              // If a new max enocunts, empty multiple_max
                        temp_max_addr <= addr_in;         // True addr_in Latch addr_in 2 (-2)   &&  find max procedure last 1 cycle (+1)
                        no_max_flag <= 0;
                    end
                    else if (data_in == temp_max_data) begin

                        // when First Encounts multiple max
                        // if (multiple_max_cnt == 0) begin
                        //     multiple_max_addr[0] <= temp_max_addr;  // Load Current addr_in into 
                        //     multiple_max_cnt <= multiple_max_cnt + 1;
                        // end
                        // If Overflow (More than 16 same max)
                        if (multiple_max_cnt == 15) begin
                            multiple_max_overflow <= 1;
                        end
                        else begin
                        // Because Of Some Stupid Reason, We repeated The 00 Data Twice, So We Won't Add It Here
                            if (addr_in > 0) begin
                                multiple_max_cnt <= multiple_max_cnt + 1;
                                multiple_max_addr[multiple_max_cnt] <= addr_in;
                            end
                        end
                        
                    end
                    else begin
                        temp_max_data <= temp_max_data;
                        temp_max_addr <= temp_max_addr;
                        multiple_max_cnt <= multiple_max_cnt;
                    end
                end  

                if (negedge_ena) begin
                    state <= 2'b01;
                end  
            end

            2'b01: begin
                //--------------- GIVE VALID SIGNAL --------------------
                if (!ena)  begin    // When ena == LOW, Give VALID

                    if (!max_data_valid) begin
                        max_data_valid <= 1;
                    end

                    // else begin
                    //     max_data_valid <= 0;
                    // end

                    // Hole ADDR Valid For N Cycle
                    if (max_addr_valid) begin
                        if (multiple_max_cnt > 0) begin
                            temp_max_addr <= multiple_max_addr[multiple_max_cnt-1];
                            multiple_max_cnt <= multiple_max_cnt - 1;  
                        end
                        else begin
                            max_data_valid <= 0;
                            max_addr_valid <= 0;
                            state <= 2'b10; // After Giving ADDR_VALID, mark have_given_valid TRUE
                        end
                    end
                    else begin
                        max_addr_valid <= 1;
                    end      

                end
            end  

            2'b10: begin
                // Give One Cycle Of FINISHB
                if (!finishb) begin
                    finishb <= 1;
                end
                else begin
                    finishb <= 0;
                    state <= 2'b00;
                    no_max_flag <= 1;
                end
            end      

        endcase
    end
end

endmodule
