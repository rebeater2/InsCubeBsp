# 实时组合导航算法
MCU:STM32H743IIT6    
IMU:ADIS16460    
GNSS:MXT906B，外挂   
Middleware: FATFS,FREE_RTOS, PSINS   

说明：    
时间同步策略：初始对时完成之后记录此时gps时间为$gpst_0$,同时开始维持本地时间。   
$$
t = gpst_0 + (TIM->CNT + INT->CNT)/PPS_MARGIN
$$ 


## 开发日志
##### 20210325
完成实时任务和输出任务设计，能够满足204.8Hz运行
初始对准方式目前为给定初始姿态
存在bug，运行时间长了触发hardware fault 中断，原因待查

##### 20210328 
添加LED任务，完成动态对准移植   
LED灯颜色含义：   
1. 开机上电 红色持续
2. 本地授时 黄色慢闪
3. 本地授时完成，进行动对准中,黄色快闪  
 对准完成，开始进行组合导航之后，根据导航状态决定颜色;
4. SPP/INS 青色
5. RTK_FLOAT DGPS/INS 蓝色
6. RTK_FIX/INS 绿色
7. INS 粉色
8. 系统错误，爆栈等等 红色快闪


##### 20210608
1. 添加里程計支持
2. 添加高速校驗輸出
3. 搬動代碼到TCM內存區域，提高系統運行速度
```
Memory region         Used Size  Region Size  %age Used
         DTCMRAM:      116044 B       128 KB     88.53%
          RAM_D1:        7220 B       512 KB      1.38%
          RAM_D2:       10880 B       288 KB      3.69%
          RAM_D3:          0 GB        64 KB      0.00%
         ITCMRAM:         40 KB        64 KB     62.50%
           FLASH:      349344 B         2 MB     16.66%
```
##### 20210827
1. 支持多种IMU，可以通过全局宏切换
2. 支持多种GNSS,可以通过宏切换
3. 支持多种


