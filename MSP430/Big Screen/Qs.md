# Current Prolems
* 当同时使用Energia的Serial和寄存器编程的uart时,Serial.begin(baudrate)实际是baudrate/2,很迷但是不影响其他使用
* 封装的uart_read函数是阻塞的(有一个while())直接调用
    * 不能像Arduino一样用 while(Serial.read()){}; 方式来清空buffer
    * 这个问题好搞
* 较高的波特率怀疑是分频指数精度太差
    * 比如115200对应指数是4

## 其他问题
* 当使用SoftSerial的时候,TX正常,RX不出东西
* 当使用Serial1的时候调动write时候卡死

##!
* msp430 使用Energia封装使用现有外设时(比如SPI,UART)不能对其内置引脚进行pinMode(LZZSCL)