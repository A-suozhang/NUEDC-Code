module readfile_rb();

reg[7:0] Data_in[0:9];
reg[7:0] Data_out = 8'b0000_0100;

integer fp; // 定义文件句柄


initial begin
    // $readmemh("C:/Users/A_suozhang/Code/EE/Verilog/Read_Text/test.txt",Data_in,0,9);
    // readmemb 同理
    fp = $fopen("C:/Users/A_suozhang/Code/EE/Verilog/Read_Text/test1.txt","w");
    # 10
    $display("Writing...");
    #10
    // $fdisplay(fp,"aaaa");
    #10
    $fdisplay(fp,"%f",Data_out);
    # 10
    $fdisplay(fp,"%h",Data_out);
    #10
    $fclose(fp);

end


endmodule