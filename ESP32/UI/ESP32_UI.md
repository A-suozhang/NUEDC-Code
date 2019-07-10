# ESP32 UI Guidance
## Getting Started
### 1. include
```
#include "UIX/UIX.h"
UIX uix;    // instantiation the UI Main OBJ
```
### 2. Create Button Click Callback
```
void btnsendclick(int tag,UIXButton* obj);
// tag means which button to use
// provide a virtual obj in

void btnsendclick(int tag,UIXButton* obj){
    // Do Something
}

```

### 3. Create Control Unit
* 3 Main Types
    * 1. UIXInputBox inptsend(20,120,20,50,COLVSBLUE,COLCYAN,sendstr);
        *  输入文本框: (左,右,上,下像素点, 前景颜色,后景颜色, 对应的字符串)
    * 2. UIXButton btnsend(20,90,60,90,COLVSBLUE,COLCYAN,"Send",btnsendclick);
        * 按钮 (左,右,上,下像素点, 前景颜色,后景颜色,显示的文字,对应的回调函数)
    * 3. UIXDynamicText txtrecv(recvstr,COLWHITE,20,100,1,0);
        * 动态文字 (对应的字符串,文字颜色,左,右,上,下像素点)
    * 4. UIXSlider()
        * 滚动条(左,右,上,下像素点,percentage,颜色)

### 4.  构造UIX布局 (Setup时)
* 首先定义uixuserpanelnum(Panel的数目,最大为4)(全局变量)
```
uixuserpanelnum=1;
```
* 给每个Panel命名
```
uixpanels[0].label="SerialTest";
```
* 加入控件 (进行了运算符重载,可以采用+=添加控件)
```
uixpanels[0].uixobjects+=inptsend;
uixpanels[0].uixobjects+=btnsend;]
uixpanels[0].uixobjects+=txtrecv;
```
* 启动
```
uix.begin();
```
* 在loop中,刷新UIX
```
uix.tick();
```

### 5. Use The Control Unit
* UIInputBox : 构造函数传入的字符串是global的,字符串可在外部修改,读取这个字符串就能知道数据(是否在tick的时候才会传入?)
* UIButton: 与回调函数绑定,每次被按下相当于调用了回调函数, 内置了按下和不被按下两种状态
* UIDynamicText: 构造函数传入的字符串改变,经过调用string.invalidate()之后,文本的内容也随之改变
* UIXSlider: 可以从外部读取percentage