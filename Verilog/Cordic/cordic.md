# Cordic Usage
* Guidance To Use The Xilinx Cordic IP CORE (v 6.0)


## IP CORE Configuration
* (事实证明很多时候结果出错都是因为IP核没有配置对)
* Tips
    * 每一个模式的输入输出变量不一样
    * Functional Selection - 选择工作模式
    * 输入输出数据位宽 (大部分时候输入数据需要拼接 {A,B})
    * Round Mode: (低位宽的时候重要) 最好选择 Nearest Mean
    * Iteration 选择0 (自动配置)
    * Coarse Rotation: (会被默认配置在第一象限,实际是[-pi/4,pi/4])
        * 取消的话理论可以拓展到整个圆
    * Compensation Scaling (**一定要勾选**,否则意义错误)
    * Square Root的Data Format : unsign fraction/integer

## DATA FORMAT
* Amp或者是y,x ( 输入数据xy在(-1,1)之间, Amp在(-sqrt(2),sqrt(2)) )
    * 前两位为整数位(第一位是符号位,第二位为0/1),后面的是定点数
* Phase
    * 前三位是整数位(第一位是符号位)
* *建议用signed wire/reg 来承载* (由于会产生负数补码的问题)

## Mode
|Mode|Input|Output|Usage|
|--|--|--|--|
|Rotation|{y,x};phase|{y',x'}|向量的旋转,当phase=0,y=0,x=amp时极坐标转直角坐标|
|Transition|{y,x}|{phase,Amp}|向量的转化,直角坐标转极坐标|
|Sin/Cos|phase|{sin(phase),cos(phase)}|求正弦余弦|
|Sinh/Cosh|(同理)|||
|Arctan|{y,x}|phase|求反正切|
|Sqrt|x|sqrt(x)|求平方根 * 有两种表达方式(可以在ip核定义),小数(unsigned fraction - 1位整数位)或者整数(unsigned integer)|


