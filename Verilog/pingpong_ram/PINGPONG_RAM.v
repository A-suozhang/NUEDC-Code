module PINGPONG_RAM(
        input clka,     //clock A
        input rsta,     //reset A
        input [6:0]addra,   //address A
        input wea,      //write enable A
        input [7:0]dina,    //data input A
        input finisha,  //finished A
        output readya,
        input clkb,     //clock B
        input rstb,
        input [6:0]addrb,   //address B
        output [7:0]doutb,  //dout B
        input finishb,  //finished B
        output readyb   //data ready B
    );
    reg sectora=1'b0;
    reg sectoraflop=1'b0;
    reg sectorb=1'b0;
    reg sectorbflop=1'b0;
    reg readya_r=1'b1;
    reg readyb_r=1'b0;
    assign readya=readya_r;
    assign readyb=readyb_r;
    
    always @(posedge clka) begin
        if(rsta)begin
            sectora<=1'b0;
            readya_r<=1'b1;
            sectorbflop<=1'b0;
        end else begin
            sectorbflop<=sectorb;
            if(finisha)begin
                readya_r<=1'b0;
                sectora<=~sectora;
            end else begin
                sectora<=sectora;
                if((!readya)&&(sectora!=sectorbflop))begin
                    readya_r<=1'b1;
                end else begin
                    readya_r<=readya_r;
                end
            end
        end
    end
    
    always @(posedge clkb) begin
        if(rstb)begin
            sectorb<=1'b0;
            readyb_r<=1'b0;
            sectoraflop<=1'b0;
        end else begin
            sectoraflop<=sectora;
            if(finishb)begin
                readyb_r<=1'b0;
                sectorb<=~sectorb;
            end else begin
                sectorb<=sectorb;
                if((!readyb)&&(sectorb!=sectoraflop))begin
                    readyb_r<=1'b1;
                end else begin
                    readyb_r<=readyb_r;
                end
            end
        end
    end
    
    //always enabled, simple dual port RAM
    blk_mem_gen_0 blk_mem_0 (
        .clka(clka),    // input wire clka
        .ena(ena),      // input wire ena
        .wea(wea),      // input wire [0 : 0] wea
        .addra(addra),  // input wire [7 : 0] addra
        .dina(dina),    // input wire [7 : 0] dina
        .douta(douta),  // output wire [7 : 0] douta
        .clkb(clkb),    // input wire clkb
        .web(web),      // input wire [0 : 0] web
        .addrb(addrb),  // input wire [7 : 0] addrb
        .dinb(dinb),    // input wire [7 : 0] dinb
        .doutb(doutb)  // output wire [7 : 0] doutb
    );
endmodule
