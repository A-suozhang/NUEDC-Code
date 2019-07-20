module find_max_b # (parameter DATA_WIDTH = 8, ADDR_WIDTH = 6)(
    input wire clk_in,
    input wire rst,

    input wire s_axis_tvalid,
    input wire s_axis_tlast,
    input wire signed[DATA_WIDTH-1:0] s_axis_tdata,
    // input wire[ADDR_WIDTH-1:0] s_axis_taddr,
    output wire s_axis_tready,      

    input wire m_axis_tready,       // actually we don't care about this line
    output reg m_axis_tvalid,
    output reg signed[DATA_WIDTH-1:0] m_axis_tdata_0,
    output reg signed[DATA_WIDTH-1:0] m_axis_tdata_1,
    output reg signed[DATA_WIDTH-1:0] m_axis_tdata_2,
    output reg[ADDR_WIDTH-1:0] m_axis_taddr
);

assign s_axis_tready = 1;   // This Module is Always Ready To Receive Input

reg[ADDR_WIDTH-1:0] s_axis_taddr;
reg[DATA_WIDTH-1:0] s_axis_tdata_r;
reg signed[DATA_WIDTH-1:0] temp_max_data;
reg signed[DATA_WIDTH-1:0] temp_max_data_0;
reg signed[DATA_WIDTH-1:0] temp_max_data_r;
reg[ADDR_WIDTH-1:0] temp_max_addr;
reg clear_all;
reg save_next_data;

always @(posedge clk_in) begin

    if (rst) begin
        temp_max_addr <= 0;
        temp_max_data <= 0;
        temp_max_data_r <= 0;
        temp_max_data_0 <= 0;
        save_next_data <= 0;
        s_axis_taddr <= 0;
        s_axis_tdata_r <= 0;
        m_axis_tdata_0 <= 0;
        m_axis_tdata_1 <= 0;
        m_axis_tdata_2 <= 0;
        m_axis_taddr <= 0;
        m_axis_tvalid <= 0;
        clear_all <= 0;
    end
    else begin
        // DATA VALID  -- COMPARE NUMBERS
        if (s_axis_tvalid) begin
            s_axis_taddr <= s_axis_taddr+1;  // taddr latch 1 cycle to sync with data 
            s_axis_tdata_r <= s_axis_tdata;  // REG s_axis_data For One Cycle
            // If ' = ', Leave Out It   
            if (s_axis_tdata > temp_max_data) begin            // Output The First MAX
                temp_max_data <= s_axis_tdata;  
                if (s_axis_taddr > 0) begin    // If The 1st Data Is The MAX Leave m_axis_tdata0 as 0
                    temp_max_data_0 <= s_axis_tdata_r;
                end
                temp_max_addr <= s_axis_taddr;
                save_next_data <= 1;
            end

            else begin
                temp_max_data <= temp_max_data;
                m_axis_tdata_0 <= m_axis_tdata_0;
                temp_max_addr <= temp_max_addr;
                save_next_data <= 0;
            end

            // Save The Next Data 2 m_axis_data_2, If Possible Max Data Occurs
            if (save_next_data) begin
                // if (s_axis_taddr == last_addr) begin  // IF LAST DATA is MAX... Make It 0
                //     m_axis_tdata_2 <= 0;
                // end
                // else begin
                    temp_max_data_r <= s_axis_tdata;
                // end 
            end
            else begin
                m_axis_tdata_2 <= m_axis_tdata_2;
            end
            
        end

        // TLAST OCCURS --- FINISH COMPARING
        if (s_axis_tlast) begin
            // Mark OUTPUT VALID
            s_axis_taddr <= s_axis_taddr;
            m_axis_taddr <= temp_max_addr;
            m_axis_tdata_0 <= temp_max_data_0;
            m_axis_tdata_1 <= temp_max_data;
            m_axis_tdata_2 <= temp_max_data_r;
            m_axis_tvalid <= 1;
            clear_all <= 1;
        end
      
        // CLEAR DATA, WAITING FOR NEXT COMPARE
        if (clear_all) begin
            s_axis_taddr <= 0;
            temp_max_data <= 0;
            temp_max_data_0 <= 0;
            temp_max_data_r <= 0;
            temp_max_addr <= 0;
            save_next_data <= 0;
             m_axis_tvalid <= 0;             // When Next Compare Starting, Last Tdata Is not Valid
            clear_all <= 0;
        end
   
    end
    
end






endmodule