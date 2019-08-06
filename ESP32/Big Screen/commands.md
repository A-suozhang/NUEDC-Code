# Big Screen Commands

## A Few Usable Options
* EE 帧头       FF FC FF FF 帧尾

### Shift Between Screens
* EE | B1 00 00 01|  FF FC FF FF    
    * B1 00 切换画面指令
    * 00 01 目标画面

### Change Button State
* (貌似只能单片机解析,发现哪个按钮被按下了)
* EE | B1 11 00 02 00 01 10 01 01| FF FC FF FF
    * B1 11 组件组态控制指令
    * 00 02 当前画面为画面2
    * 00 01 当前控件为1
    * 10  按钮型控件
    * 01/00 开关类型
    * 01 从弹起变为按下

### Display Text
* EE | B1 10 00 01 00 07 32 32 30 | FF FC FF FF
    * B1 10 发送控件组态
    * 00 01 画面1
    * 00 07 组件7
    * 32 32 30   "220"的asc

### Read Text From Screen
* EE | B1 11 00 01 00 01 11 {31 31 31 00} | FF FC FF FF
    * B1 11 接受控件信息
    * 00 01 画面ID
    * 00 01 控件ID
    * 11 表示为文本控件
    * 30 30 30 30 文本内容
    * 00 表示字符结束

### Write To Chart
* 向当前通道写新数据 (超出graph的范围的话曲线右移)
    * B1 32 向当前通道写新数据(从右边进入数据)          (35 从左边进入数据)
    * 00 01 画面ID
    * 00 01 控件编号
    * 00    CH编号
    * 00 10 DATA LEN
    * XXX   不定长数据
* 清空当前通道的数据 
    * B1 33 清空
    * 00 01 画面ID
    * 00 00 控件编号
    * 00 CH
* 改变图片的长宽 (同时对每个通道做)
    * B1 34
    * 00 01 ID1
    * 00 02 ID2
    * XX XX 水平平移
    * XX XX 水平Scale(>100 为拓展)
    * XX XX 垂直平移
    * XX XX 水平平移

### Slide
* 接受
    * B1 11
    * 00 01 ID1
    * 00 01 ID2
    * 13
    * 00 00 00 01   滑块内容

### Draw On Panel
* Set Color
    * 42 背景色 
    * FF (白色)
    中间要加帧头帧尾巴
    * 41
    * CC (紫色)
* Add Text
    * 20
    * XX XX X坐标
    * XX XX Y坐标
    * 01 启用背景色(会覆盖之前在此位置的文字,需要选择)
    * 09 最大字号
    * XX XX XX (写的东西)
    