# Zynq板
## Pin Out
* 一般都定义为LVCMOS 3.3

|||
|---|---|
|引脚作用|数字|
|RST_N(SW2)|Y6|
|Sys_CLK(板上晶振)|Y7|
|LED|R19|
|SW3|W6|
|SW4|V6|
|SW5|V10|
|SW6|V11|

## 使用指南
* rstpulse - 让所有模块同时被重置(注意rst与rstn的极性)
    * 目的是让clk_wizard先rst,然后再rst其他模块(其他模块可能是在当前模块的时钟上升沿被rst,如果不这样可能会导致其他模块rst不上)
* clk_wizard 模块的rst应该是板上rst_n(从按钮引出的)的取反

## Tips
* Rst信号置好
* top module中除了input之外其他有用的信号要么mark debug,要么作为output
* 取消Debug Core的同时需要在XDC里修改
    * 采用Debug核的时候很容易时序不收敛,原因目前不明,猜测是rstpulse
* (由于Zynq板子没有外出引脚,所以是从按钮上面跳出的线,但是按钮上面有一个消抖电容,把高f滤掉了)
