# lacation-gps
GPS农场畜牧定位项目，2款STM32芯片分别控制采集和网关传输，分为gps-control和gps-gateway

项目介绍

定位终端（STM32F103C8T6，HAL 库）：
          通过 USART 驱动 GPS 模块、I2C 读取计步传感器；
          实现 MCU 低功耗模式；
          支持两种上报方案：①USART AT 指令驱动 4G 模组，上传位置、步数数据至服务器；
                          ②SPI 驱动 LoRa 模块，无线向网关传输监测数据。
网关板（STM32F103ZET6，HAL 库）：
          LoRa 接收终端上报数据；SPI 驱动 W5500 以太网芯片，基于Socket完成监测数据向服务器转发。

