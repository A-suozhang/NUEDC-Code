set_property IOSTANDARD LVCMOS33 [get_ports Sys_clk]
set_property IOSTANDARD LVCMOS33 [get_ports rst_n]
set_property IOSTANDARD LVCMOS33 [get_ports led_on]
set_property IOSTANDARD LVCMOS33 [get_ports SW3]
set_property IOSTANDARD LVCMOS33 [get_ports out]
set_property IOSTANDARD LVCMOS33 [get_ports clk_out]


set_property PACKAGE_PIN Y6 [get_ports rst_n]
set_property PACKAGE_PIN Y7 [get_ports Sys_clk]
set_property PACKAGE_PIN R19 [get_ports led_on]
set_property PACKAGE_PIN W6 [get_ports SW3]
set_property PACKAGE_PIN V10 [get_ports out]
set_property PACKAGE_PIN V11 [get_ports clk_out]





create_debug_core u_ila_0 ila
set_property ALL_PROBE_SAME_MU true [get_debug_cores u_ila_0]
set_property ALL_PROBE_SAME_MU_CNT 4 [get_debug_cores u_ila_0]
set_property C_ADV_TRIGGER true [get_debug_cores u_ila_0]
set_property C_DATA_DEPTH 1024 [get_debug_cores u_ila_0]
set_property C_EN_STRG_QUAL true [get_debug_cores u_ila_0]
set_property C_INPUT_PIPE_STAGES 0 [get_debug_cores u_ila_0]
set_property C_TRIGIN_EN false [get_debug_cores u_ila_0]
set_property C_TRIGOUT_EN false [get_debug_cores u_ila_0]
set_property port_width 1 [get_debug_ports u_ila_0/clk]
connect_debug_port u_ila_0/clk [get_nets [list clk_wiz_test/inst/clk_out1]]
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_0/probe0]
set_property port_width 3 [get_debug_ports u_ila_0/probe0]
connect_debug_port u_ila_0/probe0 [get_nets [list {clk_divide0/cnt[0]} {clk_divide0/cnt[1]} {clk_divide0/cnt[2]}]]
set_property C_CLK_INPUT_FREQ_HZ 300000000 [get_debug_cores dbg_hub]
set_property C_ENABLE_CLK_DIVIDER false [get_debug_cores dbg_hub]
set_property C_USER_SCAN_CHAIN 1 [get_debug_cores dbg_hub]
connect_debug_port dbg_hub/clk [get_nets clk_out_OBUF]
