# [第九届飞思卡尔智能车](https://www.fanhaobai.com)

此为第九届飞思卡尔智能车摄像头组参赛作品，为了该作品我和团队泡了 1 年半的实验室（第八届华北二等奖）。该作品我早已在 [智能车制作](http://www.znczz.com) 论坛开源过，现整理于 Github。

![预览图](https://github.com/fan-haobai/smart-car-freescale/blob/master/三维模型.png)

![整车图](https://github.com/fan-haobai/smart-car-freescale/blob/master/整车.jpeg)

该作品系统架构为 K60 处理器 + MT9V034 CMOS 摄像头。

## [自动行驶原理](https://github.com/fan-haobai/smart-car-freescale/tree/master/4.Paper)

![原理](https://github.com/fan-haobai/smart-car-freescale/blob/master/5.Photos/视觉导航原理.png)

整个系统通过图像识别技术感知赛道路况，进而使用 PID 算法控制智能车的速度和方向，实现智能车的自动导航功能。

## [三维建模](https://github.com/fan-haobai/smart-car-freescale/tree/master/1.3D)

使用 SolidWorks 软件对智能车进行建模分析。

![车架](https://github.com/fan-haobai/smart-car-freescale/blob/master/1.3D/SolidWorks建模.png)

![PCB](https://github.com/fan-haobai/smart-car-freescale/blob/master/1.3D/PCB.png)

## [硬件电路](https://github.com/fan-haobai/smart-car-freescale/blob/master/2.Hardware)

硬件电路主要分为4个子部分：

* [摄像头](https://github.com/fan-haobai/smart-car-freescale/blob/master/2.Hardware/摄像头.PDF)
* [K60 最小系统](https://github.com/fan-haobai/smart-car-freescale/blob/master/2.Hardware/K60最小系统.PDF)
* [可视化交互界面](https://github.com/fan-haobai/smart-car-freescale/blob/master/2.Hardware/1.8-TFT-128x160.PDF)
* [主板](https://github.com/fan-haobai/smart-car-freescale/blob/master/2.Hardware/主板__V1.0.PDF)

其中，部分电路原理图：

![电机驱动](https://github.com/fan-haobai/smart-car-freescale/blob/master/5.Photos/电路原理图-电机驱动.jpg)

![图像数模转换](https://github.com/fan-haobai/smart-car-freescale/blob/master/5.Photos/电路原理图-图像数模转换.jpg)

## [软件源码](https://github.com/fan-haobai/smart-car-freescale/blob/master/3.Software)

软件主要功能包括:

* [K60 初始化和调度](https://github.com/fan-haobai/smart-car-freescale/blob/master/3.Software/src/Init.c)
* [MT9V034 图像采集](https://github.com/fan-haobai/smart-car-freescale/blob/master/3.Software/src/MT9V034.c)
* [图像分析](https://github.com/fan-haobai/smart-car-freescale/blob/master/3.Software/src/Analyze.c)
* [Flash数据存储](https://github.com/fan-haobai/smart-car-freescale/blob/master/3.Software/src/Flash.c)
* [可视化界面（GUI）](https://github.com/fan-haobai/smart-car-freescale/blob/master/3.Software/src/Gui.c)
* [PID 自动控制](https://github.com/fan-haobai/smart-car-freescale/blob/master/3.Software/src/Control.c)

## [毕业论文](https://github.com/fan-haobai/smart-car-freescale/tree/master/4.Paper)

我的本科毕业设计就是基于该作品，并获得了《优秀毕业论文》称号。

## [相册](https://github.com/fan-haobai/smart-car-freescale/tree/master/5.Photos)

制作过程中所有的图片汇集。例如赛道识别图像：

![二值化图像-直道](https://github.com/fan-haobai/smart-car-freescale/blob/master/5.Photos/二值化图像-直道.jpg)

![二值化图像-右弯](https://github.com/fan-haobai/smart-car-freescale/blob/master/5.Photos/二值化图像-右弯.jpg)

![二值化图像-S弯](https://github.com/fan-haobai/smart-car-freescale/blob/master/5.Photos/二值化图像-S弯.jpg)

## 自动行驶视频

* [实验室调试视频](https://v.youku.com/v_show/id_XODE0NTg2NzY4.html)
* [校内赛](https://v.youku.com/v_show/id_XOTI0NzM1NDM2.html)
* [华北赛](https://v.youku.com/v_show/id_XODUxOTUyNjk2.html)

**最后要说的是，通过这次比赛，巩固了基础知识，也充实了大学生活。**
