# [第九届飞思卡尔智能车](https://www.fanhaobai.com)

此为第九届飞思卡尔智能车摄像头组参赛作品，为了该作品我和团队泡了 1 年半的实验室（第八届华北二等奖）。该作品我早已在 [智能车制作](http://www.znczz.com) 论坛开源过，现整理于 Github。

![预览图](https://github.com/fan-haobai/smart-car/blob/master/三维模型预览.png)

该作品系统架构为 K60 处理器+ MT9V034 CMOS 摄像头，通过图像识别和 PID 速度控制，实现智能车自动导航功能。更多相关信息，见 [天翊的个人空间](http://www.znczz.com/home.php?mod=space&uid=125200&do=album&id=1777。

## [源码](https://github.com/fan-haobai/smart-car/blob/master/)

此部分主要包括 [K60 初始化和调度]()、[MT9V034 图像采集]()、[图像分析]()、[Flash]()、[可视化界面（Gui）]()、[PID 自动控制]() 部分。

## [PCB](https://github.com/fan-haobai/smart-car/blob/master/)

PCB 即为控制电路，分为 [主板]()、[摄像头]()、[K60 最小系统]()、[TFT — 可视化界面]() 部分。

## [3D](https://github.com/fan-haobai/smart-car/blob/master/)

![预览图](https://github.com/fan-haobai/smart-car/blob/master/solidworks.png)

通过 SolidWorks 制作三维模型，并生成 3D 预览效果。可以分为 [车架]()、[PCB]() 部分，该部分源文件为 SolidWorks 文件。

## [论文]()

我的本科毕业设计就是基于该作品，并获得了优秀毕业论文称号。

## [相册]()

制作过程中所有的图片汇集。

>最后要说的是，通过这次比赛，巩固了基础知识，也充实了大学生活，更重要的是认识了一群人 [地球村]，如果再来一次，我还会这么选择。