# ESP32 Wifi

## WiFi Scan
* 扫描目前可以连接的WiFi
    * (并且能够找出最大RSSI的公共WiFi(用于自动连接))

## ESP32 Act As Server
* 虽然是做Server,但是并不是AP
    * AP是一个网络 (可以是手机热点或者是路由器) 需要将其ssid和password在Wifi.begin()的时候调用
    * 建立一个Server对象,初始化时候填写端口(port)
    * ESP32作为server读取或者是写数据都采用: client.read/write()
    * WiFiClient client = server.available(); 建立一个Client对象
    
## ESP32 Act As AP
* 做AP,同时做Server,开启一个WiFi
    * 测试机需要**在此WiFi**下,连接对应port的server
    * 连接有一些慢(原因不明)

## ESP32 Act As Client
* 作为client,需要在上位机(PC)建立一个TCP Server(用调试助手或者是Python)
    * Server的IP应该是当前WiFi下的PC机IP(Server IP),端口则和代码中的port相同
    * 使用Stream库的一些方法会有不明bug (是网络调试助手的bug)
        * 但是readstringuntil的效果和我想想的不大一样

    
