;/*****************************************************************************
; * @file:    startup_K60.s
; * @purpose: 兼容CMSIS规范的Freescale K60启动文件
; * @version: V1.01[By LPLD]
; * @date:    2013-06-18
; *----------------------------------------------------------------------------
; *
; * 更改建议:禁止修改
; *
; * 版权所有:北京拉普兰德电子技术有限公司
; * http://www.lpld.cn
; * mail:support@lpld.cn
; *
; * @par
; * 本代码由拉普兰德[LPLD]开发并维护，并向所有使用者开放源代码。
; * 开发者可以随意修使用或改源代码。但本段及以上注释应予以保留。
; * 不得更改或删除原版权所有者姓名，二次开发者可以加注二次版权所有者。
; * 但应在遵守此协议的基础上，开放源代码、不得出售代码本身。
; * 拉普兰德不负责由于使用本代码所带来的任何事故、法律责任或相关不良影响。
; * 拉普兰德无义务解释、说明本代码的具体原理、功能、实现方法。
; * 除非拉普兰德[LPLD]授权，开发者不得将本代码用于商业产品。
; *


;
; The modules in this file are included in the libraries, and may be replaced
; by any user-defined modules that define the PUBLIC symbol _program_start or
; a user defined start symbol.
; To override the cstartup defined in the library, simply add your modified
; version to the workbench project.
;
; The vector table is normally located at address 0.
; When debugging in RAM, it can be located in RAM, aligned to at least 2^6.
; The name "__vector_table" has special meaning for C-SPY:
; it is where the SP start value is found, and the NVIC vector
; table register (VTOR) is initialized to this address if != 0.
;
; Cortex-M version
;

        MODULE  ?cstartup

        ;; Forward declaration of sections.
        SECTION CSTACK:DATA:NOROOT(3)

        SECTION .intvec:CODE:NOROOT(2)
	
        EXTERN  main
        EXTERN  SystemInit
        PUBLIC  __vector_table
        PUBLIC  __vector_table_0x1c
        PUBLIC  __Vectors
        PUBLIC  __Vectors_End
        PUBLIC  __Vectors_Size

        DATA


; K60中断向量表
__vector_table
        DCD     sfe(CSTACK)               ; Top of Stack
        DCD     Reset_Handler       ; Reset Handler
        DCD     NMI_Handler               ; NMI Handler
        DCD     HardFault_Handler         ; Hard Fault Handler
        DCD     MemManage_Handler         ; MPU Fault Handler
        DCD     BusFault_Handler          ; Bus Fault Handler
        DCD     UsageFault_Handler        ; Usage Fault Handler
__vector_table_0x1c
        DCD     0                         ; Reserved
        DCD     0                         ; Reserved
        DCD     0                         ; Reserved
        DCD     0                         ; Reserved
        DCD     SVC_Handler               ; SVCall Handler
        DCD     DebugMon_Handler          ; Debug Monitor Handler
        DCD     0                         ; Reserved
        DCD     OS_CPU_PendSVHandler      ;PendSV_Handler
        DCD     OS_CPU_SysTickHandler     ;SysTick_Handler
        ; 外部中断
        DCD     DMA0_IRQHandler           ; 0:  DMA Channel 0 transfer complete
        DCD     DMA1_IRQHandler           ; 1:  DMA Channel 1 transfer complete
        DCD     DMA2_IRQHandler           ; 2:  DMA Channel 2 transfer complete
        DCD     DMA3_IRQHandler           ; 3:  DMA Channel 3 transfer complete
        DCD     DMA4_IRQHandler           ; 4:  DMA Channel 4 transfer complete
        DCD     DMA5_IRQHandler           ; 5:  DMA Channel 5 transfer complete
        DCD     DMA6_IRQHandler           ; 6:  DMA Channel 6 transfer complete
        DCD     DMA7_IRQHandler           ; 7:  DMA Channel 7 transfer complete
        DCD     DMA8_IRQHandler           ; 8:  DMA Channel 8 transfer complete
        DCD     DMA9_IRQHandler           ; 9:  DMA Channel 9 transfer complete
        DCD     DMA10_IRQHandler          ;10:  DMA Channel 10 transfer complete
        DCD     DMA11_IRQHandler          ;11:  DMA Channel 11 transfer complete
        DCD     DMA12_IRQHandler          ;12:  DMA Channel 12 transfer complete
        DCD     DMA13_IRQHandler          ;13:  DMA Channel 13 transfer complete
        DCD     DMA14_IRQHandler          ;14:  DMA Channel 14 transfer complete
        DCD     DMA15_IRQHandler          ;15:  DMA Channel 15 transfer complete
        DCD     DMA_ERR_IRQHandler        ;16:  DMA Error Interrupt Channels 0-15
        DCD     MCM_IRQHandler            ;17:  MCM Normal interrupt
        DCD     FLASH_CC_IRQHandler       ;18:  Flash memory command complete
        DCD     FLASH_RC_IRQHandler       ;19:  Flash memory read collision
        DCD     VLD_IRQHandler            ;20:  Low Voltage Detect, Low Voltage Warning
        DCD     LLWU_IRQHandler           ;21:  Low Leakage Wakeup
        DCD     WDOG_IRQHandler           ;22:  WDOG interrupt
        DCD     RNGB_IRQHandler           ;23:  Random number generator (RNGB)
        DCD     I2C0_IRQHandler           ;24:  I2C0 interrupt
        DCD     I2C1_IRQHandler           ;25:  I2C1 interrupt
        DCD     SPI0_IRQHandler           ;26:  SPI 0 interrupt
        DCD     SPI1_IRQHandler           ;27:  SPI 1 interrupt
        DCD     SPI2_IRQHandler           ;28:  SPI 2 interrupt
        DCD     CAN0_MESS_IRQHandler      ;29:  CAM 0 OR'ed Message buffer (0-15)
        DCD     CAN0_BUS_OFF_IRQHandler   ;30:  CAM 0 Bus Off
        DCD     CAN0_ERR_IRQHandler       ;31:  CAM 0 Error
        DCD     CAN0_TW_IRQHandler        ;32:  CAM 0 Transmit Warning
        DCD     CAN0_RW_IRQHandler        ;33:  CAM 0 Receive Warning
        DCD     CAN0_WAKE_UP_IRQHandler   ;34:  CAM 0 WakeUp
        DCD     CAN0_IMEU_IRQHandler      ;35:  CAM 0 Individual Matching Elements Update (IMEU)
        DCD     CAN0_LR_IRQHandler        ;36:  CAM 0 Lost receive
        DCD     CAN1_MESS_IRQHandler      ;37:  CAM 1 OR'ed Message buffer (0-15)
        DCD     CAN1_BUS_OFF_IRQHandler   ;38:  CAM 1 Bus Off
        DCD     CAN1_ERR_IRQHandler       ;39:  CAM 1 Error
        DCD     CAN1_TW_IRQHandler        ;40:  CAM 1 Transmit Warning
        DCD     CAN1_RW_IRQHandler        ;41:  CAM 1 Receive Warning
        DCD     CAN1_WAKE_UP_IRQHandler   ;42:  CAM 1 WakeUp
        DCD     CAN1_IMEU_IRQHandler      ;43:  CAM 1 Individual Matching Elements Update (IMEU)
        DCD     CAN1_LR_IRQHandler        ;44:  CAM 1 Lost receive
        DCD     UART0_IRQHandler          ;45:  UART 0 intertrupt
        DCD     UART0_ERR_IRQHandler      ;46:  UART 0 error intertrupt
        DCD     UART1_IRQHandler          ;47:  UART 1 intertrupt
        DCD     UART1_ERR_IRQHandler      ;48:  UART 1 error intertrupt
        DCD     UART2_IRQHandler          ;49:  UART 2 intertrupt
        DCD     UART2_ERR_IRQHandler      ;50:  UART 2 error intertrupt
        DCD     UART3_IRQHandler          ;51:  UART 3 intertrupt
        DCD     UART3_ERR_IRQHandler      ;52:  UART 3 error intertrupt
        DCD     UART4_IRQHandler          ;53:  UART 4 intertrupt
        DCD     UART4_ERR_IRQHandler      ;54:  UART 4 error intertrupt
        DCD     UART5_IRQHandler          ;55:  UART 5 intertrupt
        DCD     UART5_ERR_IRQHandler      ;56:  UART 5 error intertrupt
        DCD     ADC0_IRQHandler           ;57:  ADC 0 interrupt
        DCD     ADC1_IRQHandler           ;58:  ADC 1 interrupt
        DCD     CMP0_IRQHandler           ;59:  CMP 0 High-speed comparator interrupt
        DCD     CMP1_IRQHandler           ;60:  CMP 1 interrupt
        DCD     CMP2_IRQHandler           ;61:  CMP 2 interrupt
        DCD     FTM0_IRQHandler           ;62:  FTM 0 interrupt
        DCD     FTM1_IRQHandler           ;63:  FTM 1 interrupt
        DCD     FTM2_IRQHandler           ;64:  FTM 2 interrupt
        DCD     CMT_IRQHandler            ;65:  CMT intrrupt
        DCD     RTC_IRQHandler            ;66:  RTC interrupt
        DCD     RTC_Seconds_IRQHandler    ;67:  RTC seconds interrupt
        DCD     PIT0_IRQHandler           ;68:  PIT 0 interrupt
        DCD     PIT1_IRQHandler           ;69:  PIT 1 interrupt
        DCD     PIT2_IRQHandler           ;70:  PIT 2 interrupt
        DCD     PIT3_IRQHandler           ;71:  PIT 3 interrupt
        DCD     PDB_IRQHandler            ;72:  PDB interrupt
        DCD     USB_OTG_IRQHandler        ;73:  USB OTG interrupt
        DCD     USB_CD_IRQHandler         ;74:  USB Charger Detect interrupt
        DCD     Eth_IEEE1588_IRQHandler   ;75:  IEEE 1588 Timer Interrupt
        DCD     Eth_TX_IRQHandler         ;76:  Transmit interrupt
        DCD     Eth_RX_IRQHandler         ;77:  Receive interrupt
        DCD     Eth_Err_Misc_IRQHandler   ;78:  Error and miscellaneous interrupt
        DCD     I2S_IRQHandler            ;79:  I2S interrupt
        DCD     SDHC_IRQHandler           ;80:  SDHC interrupt
        DCD     DAC0_IRQHandler           ;81:  DAC 0 interrupt
        DCD     DAC1_IRQHandler           ;82:  DAC 1 interrupt
        DCD     TSI_IRQHandler            ;83:  TSI interrupt
        DCD     MCG_IRQHandler            ;84:  MCG interrupt
        DCD     LPT_IRQHandler            ;85:  LPT interrupt
        DCD     Reserved102_IRQHandler                         ;86:  Reserved
        DCD     PORTA_IRQHandler          ;87:  PORT A interrupt
        DCD     PORTB_IRQHandler          ;88:  PORT B interrupt
        DCD     PORTC_IRQHandler          ;89:  PORT C interrupt
        DCD     PORTD_IRQHandler          ;90:  PORT D interrupt
        DCD     PORTE_IRQHandler          ;91:  PORT E interrupt
        DCD     PORTF_IRQHandler          ;92:  PORT F interrupt
        DCD     Reserved109_IRQHandler    ;93:  Reserved interrupt 109
        DCD     SWI_IRQHandler            ;94:  Software interrupt
        DCD     NFC_IRQHandler            ;95:  NAND flash controller interrupt
        DCD     USBHS_IRQHandler          ;96:  USB high speed OTG interrupt
        DCD     Reserved113_IRQHandler    ;97:  Reserved interrupt 113
        DCD     CMP3_IRQHandler           ;98:  CMP3 interrupt
        DCD     Reserved115_IRQHandler    ; Reserved interrupt 115
        DCD     Reserved116_IRQHandler    ; Reserved interrupt 116
        DCD     FTM3_IRQHandler           ;101: FTM3 fault, overflow and channels interrupt
        DCD     ADC2_IRQHandler           ;102: ADC2 interrupt
        DCD     ADC3_IRQHandler           ;103: ADC3 interrupt
        DCD     I2S1_Tx_IRQHandler        ;104: I2S1 transmit interrupt
        DCD     I2S1_Rx_IRQHandler        ;105: I2S1 receive interrupt
          DCD     DefaultISR  ; 122
          DCD     DefaultISR  ; 123
          DCD     DefaultISR  ; 124
          DCD     DefaultISR  ; 125
          DCD     DefaultISR  ; 126
          DCD     DefaultISR  ; 127
          DCD     DefaultISR  ; 128
          DCD     DefaultISR  ; 129
          DCD     DefaultISR  ; 130
          DCD     DefaultISR  ; 131
          DCD     DefaultISR  ; 132
          DCD     DefaultISR  ; 133
          DCD     DefaultISR  ; 134
          DCD     DefaultISR  ; 135
          DCD     DefaultISR  ; 136
          DCD     DefaultISR  ; 137
          DCD     DefaultISR  ; 138
          DCD     DefaultISR  ; 139
          DCD     DefaultISR  ; 140
          DCD     DefaultISR  ; 141
          DCD     DefaultISR  ; 142
          DCD     DefaultISR  ; 143
          DCD     DefaultISR  ; 144
          DCD     DefaultISR  ; 145
          DCD     DefaultISR  ; 146
          DCD     DefaultISR  ; 147
          DCD     DefaultISR  ; 148
          DCD     DefaultISR  ; 149
          DCD     DefaultISR  ; 150
          DCD     DefaultISR  ; 151
          DCD     DefaultISR  ; 152
          DCD     DefaultISR  ; 153
          DCD     DefaultISR  ; 154
          DCD     DefaultISR  ; 155
          DCD     DefaultISR  ; 156
          DCD     DefaultISR  ; 157
          DCD     DefaultISR  ; 158
          DCD     DefaultISR  ; 159
          DCD     DefaultISR  ; 160
          DCD     DefaultISR  ; 161
          DCD     DefaultISR  ; 162
          DCD     DefaultISR  ; 163
          DCD     DefaultISR  ; 164
          DCD     DefaultISR  ; 165
          DCD     DefaultISR  ; 166
          DCD     DefaultISR  ; 167
          DCD     DefaultISR  ; 168
          DCD     DefaultISR  ; 169
          DCD     DefaultISR  ; 170
          DCD     DefaultISR  ; 171
          DCD     DefaultISR  ; 172
          DCD     DefaultISR  ; 173
          DCD     DefaultISR  ; 174
          DCD     DefaultISR  ; 175
          DCD     DefaultISR  ; 176
          DCD     DefaultISR  ; 177
          DCD     DefaultISR  ; 178
          DCD     DefaultISR  ; 179
          DCD     DefaultISR  ; 180
          DCD     DefaultISR  ; 181
          DCD     DefaultISR  ; 182
          DCD     DefaultISR  ; 183
          DCD     DefaultISR  ; 184
          DCD     DefaultISR  ; 185
          DCD     DefaultISR  ; 186
          DCD     DefaultISR  ; 187
          DCD     DefaultISR  ; 188
          DCD     DefaultISR  ; 189
          DCD     DefaultISR  ; 190
          DCD     DefaultISR  ; 191
          DCD     DefaultISR  ; 192
          DCD     DefaultISR  ; 193
          DCD     DefaultISR  ; 194
          DCD     DefaultISR  ; 195
          DCD     DefaultISR  ; 196
          DCD     DefaultISR  ; 197
          DCD     DefaultISR  ; 198
          DCD     DefaultISR  ; 199
          DCD     DefaultISR  ; 200
          DCD     DefaultISR  ; 201
          DCD     DefaultISR  ; 202
          DCD     DefaultISR  ; 203
          DCD     DefaultISR  ; 204
          DCD     DefaultISR  ; 205
          DCD     DefaultISR  ; 206
          DCD     DefaultISR  ; 207
          DCD     DefaultISR  ; 208
          DCD     DefaultISR  ; 209
          DCD     DefaultISR  ; 210
          DCD     DefaultISR  ; 211
          DCD     DefaultISR  ; 212
          DCD     DefaultISR  ; 213
          DCD     DefaultISR  ; 214
          DCD     DefaultISR  ; 215
          DCD     DefaultISR  ; 216
          DCD     DefaultISR  ; 217
          DCD     DefaultISR  ; 218
          DCD     DefaultISR  ; 219
          DCD     DefaultISR  ; 220
          DCD     DefaultISR  ; 221
          DCD     DefaultISR  ; 222
          DCD     DefaultISR  ; 223
          DCD     DefaultISR  ; 224
          DCD     DefaultISR  ; 225
          DCD     DefaultISR  ; 226
          DCD     DefaultISR  ; 227
          DCD     DefaultISR  ; 228
          DCD     DefaultISR  ; 229
          DCD     DefaultISR  ; 230
          DCD     DefaultISR  ; 231
          DCD     DefaultISR  ; 232
          DCD     DefaultISR  ; 233
          DCD     DefaultISR  ; 234
          DCD     DefaultISR  ; 235
          DCD     DefaultISR  ; 236
          DCD     DefaultISR  ; 237
          DCD     DefaultISR  ; 238
          DCD     DefaultISR  ; 239
          DCD     DefaultISR  ; 240
          DCD     DefaultISR  ; 241
          DCD     DefaultISR  ; 242
          DCD     DefaultISR  ; 243
          DCD     DefaultISR  ; 244
          DCD     DefaultISR  ; 245
          DCD     DefaultISR  ; 246
          DCD     DefaultISR  ; 247
          DCD     DefaultISR  ; 248
          DCD     DefaultISR  ; 249
          DCD     DefaultISR  ; 250
          DCD     DefaultISR  ; 251
          DCD     DefaultISR  ; 252
          DCD     DefaultISR  ; 253
          DCD     DefaultISR  ; 254
          DCD     DefaultISR  ; 255


__Vectors_End

__Vectors       EQU   __vector_table
__Vectors_Size 	EQU   __Vectors_End - __Vectors

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; 默认中断处理函数
;;
        THUMB

        PUBWEAK Reset_Handler
        SECTION .noinit:CODE:REORDER:NOROOT(2)
;复位函数
Reset_Handler
        LDR     R0, =SystemInit         ;执行系统初始化函数SystemInit()
        BLX     R0
        LDR     R0, =main               ;执行用户主函数main()
        BX      R0

      PUBWEAK NMI_Handler
      PUBWEAK HardFault_Handler
      PUBWEAK MemManage_Handler
      PUBWEAK BusFault_Handler
      PUBWEAK UsageFault_Handler
      PUBWEAK SVC_Handler
      PUBWEAK DebugMon_Handler
      PUBWEAK OS_CPU_PendSVHandler    ;PendSV_Handler
      PUBWEAK OS_CPU_SysTickHandler   ;SysTick_Handler
      PUBWEAK DMA0_IRQHandler
      PUBWEAK DMA1_IRQHandler
      PUBWEAK DMA2_IRQHandler
      PUBWEAK DMA3_IRQHandler
      PUBWEAK DMA4_IRQHandler
      PUBWEAK DMA5_IRQHandler
      PUBWEAK DMA6_IRQHandler
      PUBWEAK DMA7_IRQHandler
      PUBWEAK DMA8_IRQHandler
      PUBWEAK DMA9_IRQHandler
      PUBWEAK DMA10_IRQHandler
      PUBWEAK DMA11_IRQHandler
      PUBWEAK DMA12_IRQHandler
      PUBWEAK DMA13_IRQHandler
      PUBWEAK DMA14_IRQHandler
      PUBWEAK DMA15_IRQHandler
      PUBWEAK DMA_ERR_IRQHandler
      PUBWEAK MCM_IRQHandler
      PUBWEAK FLASH_CC_IRQHandler
      PUBWEAK FLASH_RC_IRQHandler
      PUBWEAK VLD_IRQHandler
      PUBWEAK LLWU_IRQHandler
      PUBWEAK WDOG_IRQHandler
      PUBWEAK RNGB_IRQHandler
      PUBWEAK I2C0_IRQHandler
      PUBWEAK I2C1_IRQHandler
      PUBWEAK SPI0_IRQHandler
      PUBWEAK SPI1_IRQHandler
      PUBWEAK SPI2_IRQHandler
      PUBWEAK CAN0_MESS_IRQHandler
      PUBWEAK CAN0_BUS_OFF_IRQHandler
      PUBWEAK CAN0_ERR_IRQHandler
      PUBWEAK CAN0_TW_IRQHandler
      PUBWEAK CAN0_RW_IRQHandler
      PUBWEAK CAN0_WAKE_UP_IRQHandler
      PUBWEAK CAN0_IMEU_IRQHandler
      PUBWEAK CAN0_LR_IRQHandler
      PUBWEAK CAN1_MESS_IRQHandler
      PUBWEAK CAN1_BUS_OFF_IRQHandler
      PUBWEAK CAN1_ERR_IRQHandler
      PUBWEAK CAN1_TW_IRQHandler
      PUBWEAK CAN1_RW_IRQHandler
      PUBWEAK CAN1_WAKE_UP_IRQHandler
      PUBWEAK CAN1_IMEU_IRQHandler
      PUBWEAK CAN1_LR_IRQHandler
      PUBWEAK UART0_IRQHandler
      PUBWEAK UART0_ERR_IRQHandler
      PUBWEAK UART1_IRQHandler
      PUBWEAK UART1_ERR_IRQHandler
      PUBWEAK UART2_IRQHandler
      PUBWEAK UART2_ERR_IRQHandler
      PUBWEAK UART3_IRQHandler
      PUBWEAK UART3_ERR_IRQHandler
      PUBWEAK UART4_IRQHandler
      PUBWEAK UART4_ERR_IRQHandler
      PUBWEAK UART5_IRQHandler
      PUBWEAK UART5_ERR_IRQHandler
      PUBWEAK ADC0_IRQHandler
      PUBWEAK ADC1_IRQHandler
      PUBWEAK CMP0_IRQHandler
      PUBWEAK CMP1_IRQHandler
      PUBWEAK CMP2_IRQHandler
      PUBWEAK FTM0_IRQHandler
      PUBWEAK FTM1_IRQHandler
      PUBWEAK FTM2_IRQHandler
      PUBWEAK CMT_IRQHandler
      PUBWEAK RTC_IRQHandler
      PUBWEAK RTC_Seconds_IRQHandler
      PUBWEAK PIT0_IRQHandler
      PUBWEAK PIT1_IRQHandler
      PUBWEAK PIT2_IRQHandler
      PUBWEAK PIT3_IRQHandler
      PUBWEAK PDB_IRQHandler
      PUBWEAK USB_OTG_IRQHandler
      PUBWEAK USB_CD_IRQHandler
      PUBWEAK Eth_IEEE1588_IRQHandler
      PUBWEAK Eth_TX_IRQHandler
      PUBWEAK Eth_RX_IRQHandler
      PUBWEAK Eth_Err_Misc_IRQHandler
      PUBWEAK I2S_IRQHandler
      PUBWEAK SDHC_IRQHandler
      PUBWEAK DAC0_IRQHandler
      PUBWEAK DAC1_IRQHandler
      PUBWEAK TSI_IRQHandler
      PUBWEAK MCG_IRQHandler
      PUBWEAK LPT_IRQHandler
      PUBWEAK  Reserved102_IRQHandler 
      PUBWEAK PORTA_IRQHandler
      PUBWEAK PORTB_IRQHandler
      PUBWEAK PORTC_IRQHandler
      PUBWEAK PORTD_IRQHandler
      PUBWEAK PORTE_IRQHandler   
      PUBWEAK PORTF_IRQHandler
      PUBWEAK Reserved109_IRQHandler
      PUBWEAK SWI_IRQHandler
      PUBWEAK NFC_IRQHandler
      PUBWEAK USBHS_IRQHandler
      PUBWEAK Reserved113_IRQHandler
      PUBWEAK CMP3_IRQHandler
      PUBWEAK Reserved115_IRQHandler    
      PUBWEAK Reserved116_IRQHandler    
      PUBWEAK FTM3_IRQHandler
      PUBWEAK ADC2_IRQHandler
      PUBWEAK ADC3_IRQHandler
      PUBWEAK I2S1_Tx_IRQHandler
      PUBWEAK I2S1_Rx_IRQHandler
      PUBWEAK  DefaultISR                   

      SECTION .noinit:CODE:REORDER:NOROOT(1)
      THUMB
NMI_Handler
HardFault_Handler
MemManage_Handler
BusFault_Handler
UsageFault_Handler
SVC_Handler
DebugMon_Handler
OS_CPU_PendSVHandler    ;PendSV_Handler
OS_CPU_SysTickHandler   ;SysTick_Handler
DMA0_IRQHandler
DMA1_IRQHandler
DMA2_IRQHandler
DMA3_IRQHandler
DMA4_IRQHandler
DMA5_IRQHandler
DMA6_IRQHandler
DMA7_IRQHandler
DMA8_IRQHandler
DMA9_IRQHandler
DMA10_IRQHandler
DMA11_IRQHandler
DMA12_IRQHandler
DMA13_IRQHandler
DMA14_IRQHandler
DMA15_IRQHandler
DMA_ERR_IRQHandler
MCM_IRQHandler
FLASH_CC_IRQHandler
FLASH_RC_IRQHandler
VLD_IRQHandler
LLWU_IRQHandler
WDOG_IRQHandler
RNGB_IRQHandler
I2C0_IRQHandler
I2C1_IRQHandler
SPI0_IRQHandler
SPI1_IRQHandler
SPI2_IRQHandler
CAN0_MESS_IRQHandler
CAN0_BUS_OFF_IRQHandler
CAN0_ERR_IRQHandler
CAN0_TW_IRQHandler
CAN0_RW_IRQHandler
CAN0_WAKE_UP_IRQHandler
CAN0_IMEU_IRQHandler
CAN0_LR_IRQHandler
CAN1_MESS_IRQHandler
CAN1_BUS_OFF_IRQHandler
CAN1_ERR_IRQHandler
CAN1_TW_IRQHandler
CAN1_RW_IRQHandler
CAN1_WAKE_UP_IRQHandler
CAN1_IMEU_IRQHandler
CAN1_LR_IRQHandler
UART0_IRQHandler
UART0_ERR_IRQHandler
UART1_IRQHandler
UART1_ERR_IRQHandler
UART2_IRQHandler
UART2_ERR_IRQHandler
UART3_IRQHandler
UART3_ERR_IRQHandler
UART4_IRQHandler
UART4_ERR_IRQHandler
UART5_IRQHandler
UART5_ERR_IRQHandler
ADC0_IRQHandler
ADC1_IRQHandler
CMP0_IRQHandler
CMP1_IRQHandler
CMP2_IRQHandler
FTM0_IRQHandler
FTM1_IRQHandler
FTM2_IRQHandler
CMT_IRQHandler
RTC_IRQHandler
RTC_Seconds_IRQHandler
PIT0_IRQHandler
PIT1_IRQHandler
PIT2_IRQHandler
PIT3_IRQHandler
PDB_IRQHandler
USB_OTG_IRQHandler
USB_CD_IRQHandler
Eth_IEEE1588_IRQHandler
Eth_TX_IRQHandler
Eth_RX_IRQHandler
Eth_Err_Misc_IRQHandler
I2S_IRQHandler
SDHC_IRQHandler
DAC0_IRQHandler
DAC1_IRQHandler
TSI_IRQHandler
MCG_IRQHandler
LPT_IRQHandler
Reserved102_IRQHandler
PORTA_IRQHandler
PORTB_IRQHandler
PORTC_IRQHandler
PORTD_IRQHandler
PORTE_IRQHandler
PORTF_IRQHandler
Reserved109_IRQHandler
SWI_IRQHandler
NFC_IRQHandler
USBHS_IRQHandler
Reserved113_IRQHandler
CMP3_IRQHandler
Reserved115_IRQHandler
Reserved116_IRQHandler
FTM3_IRQHandler
ADC2_IRQHandler
ADC3_IRQHandler
I2S1_Tx_IRQHandler
I2S1_Rx_IRQHandler
DefaultISR

        B DefaultISR
        END

