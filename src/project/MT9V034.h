/**
 * --------------基于"拉普兰德K60底层库"的工程（Smart_Car）-----------------
 * @file MT9V034.h
 * @version 0.0
 * @date 2015-2-7
 * @brief 关于该工程的描述
 *
 * 版权所有:北华航天工业学院 第十届飞思卡尔  摄像头2队 
 * 硬件平台:  MK60FX512
 * 
 */
#ifndef __MT9V034_H__
#define __MT9V034_H__

#include "common.h"

/* 定义SCCB设备地址 */
#define SCCB_DEV_ADR    0xB8                             //MT9V034

/* MT9V03寄存器 */
#define BINNING_ROW_A					4
#define BINNING_COLUMN_A				4
#define BINNING_ROW_B					2
#define BINNING_COLUMN_B				2
#define MINIMUM_HORIZONTAL_BLANKING		        91      // see datasheet
#define MAX_IMAGE_HEIGHT				480
#define MAX_IMAGE_WIDTH					752
#define MINIMUM_COLUMN_START			        1
#define MINIMUM_ROW_START				4

/* Context A */
#define MTV_COLUMN_START_REG_A  		0x01
#define MTV_ROW_START_REG_A     		0x02
#define MTV_WINDOW_HEIGHT_REG_A 		0x03
#define MTV_WINDOW_WIDTH_REG_A  		0x04
#define MTV_HOR_BLANKING_REG_A  		0x05
#define MTV_VER_BLANKING_REG_A  		0x06
#define MTV_COARSE_SW_1_REG_A			0x08
#define MTV_COARSE_SW_2_REG_A			0x09
#define MTV_COARSE_SW_CTRL_REG_A		0x0A
#define MTV_COARSE_SW_TOTAL_REG_A 		0x0B
#define MTV_FINE_SW_1_REG_A			0xD3
#define MTV_FINE_SW_2_REG_A			0xD4
#define MTV_FINE_SW_TOTAL_REG_A			0xD5
#define MTV_READ_MODE_REG_A        		0x0D
#define MTV_V1_CTRL_REG_A			0x31
#define MTV_V2_CTRL_REG_A			0x32
#define MTV_V3_CTRL_REG_A			0x33
#define MTV_V4_CTRL_REG_A			0x34
#define MTV_ANALOG_GAIN_CTRL_REG_A		0x35

/* Context B */
#define MTV_COLUMN_START_REG_B  		0xC9
#define MTV_ROW_START_REG_B     		0xCA
#define MTV_WINDOW_HEIGHT_REG_B 		0xCB
#define MTV_WINDOW_WIDTH_REG_B  		0xCC
#define MTV_HOR_BLANKING_REG_B  		0xCD
#define MTV_VER_BLANKING_REG_B  		0xCE
#define MTV_COARSE_SW_1_REG_B			0xCF
#define MTV_COARSE_SW_2_REG_B			0xD0
#define MTV_COARSE_SW_CTRL_REG_B		0xD1
#define MTV_COARSE_SW_TOTAL_REG_B 		0xD2
#define MTV_FINE_SW_1_REG_B			0xD6
#define MTV_FINE_SW_2_REG_B			0xD7
#define MTV_FINE_SW_TOTAL_REG_B			0xD8
#define MTV_READ_MODE_REG_B        		0x0E
#define MTV_V1_CTRL_REG_B			0x39
#define MTV_V2_CTRL_REG_B			0x3A
#define MTV_V3_CTRL_REG_B			0x3B
#define MTV_V4_CTRL_REG_B			0x3C
#define MTV_ANALOG_GAIN_CTRL_REG_B		0x36

/* General Registers */
#define MTV_CHIP_VERSION_REG    		0x00
#define MTV_CHIP_CONTROL_REG    		0x07
#define MTV_SOFT_RESET_REG      		0x0C
#define MTV_VREF_ADC_REG                        0x2C    //ADC转换参考电压，默认为0x0000 
                                                        /*该寄存器的2:0位值如下：
                                                          0---VREF_ADC = 1.0V      1---VREF_ADC = 1.1V
                                                          2---VREF_ADC = 1.2V      3---VREF_ADC = 1.3V
                                                          4---VREF_ADC = 1.4V      5---VREF_ADC = 1.5V
                                                          6---VREF_ADC = 1.6V      7---VREF_ADC = 2.1V  */
#define MTV_HDR_ENABLE_REG			0x0F
#define MTV_ADC_RES_CTRL_REG			0x1C
#define MTV_ROW_NOISE_CORR_CTRL_REG		0x70
#define MTV_TEST_PATTERN_REG       		0x7F
#define MTV_TILED_DIGITAL_GAIN_REG		0x80
#define MTV_AGC_AEC_DESIRED_BIN_REG		0xA5
#define MTV_MAX_GAIN_REG        		0xAB  
#define MTV_MIN_EXPOSURE_REG       		0xAC  // datasheet min coarse shutter width
#define MTV_MAX_EXPOSURE_REG       		0xAD  // datasheet max coarse shutter width
#define MTV_AEC_AGC_ENABLE_REG			0xAF
#define MTV_AGC_AEC_PIXEL_COUNT_REG		0xB0
#define MTV_AEC_UPDATE_REG			0xA6
#define MTV_AEC_LOWPASS_REG			0xA8
#define MTV_AGC_UPDATE_REG			0xA9
#define MTV_AGC_LOWPASS_REG		        0xAA
#define MTV_DIGITAL_TEST_REG			0x7F

//函数申明
void MT9V034_Init();

#endif /* __MT9V034_H__ */