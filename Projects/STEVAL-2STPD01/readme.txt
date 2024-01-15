/**
  @page The STSW-2STPD01 software package contains the application source code and 
        libraries designed to demonstrate the capabilities of the STEVAL-2STPD01 
        USB Type-C™ Power delivery dual port adapter kit.

  @verbatim
  ******************** (C) COPYRIGHT 2021 STMicroelectronics *******************
  * @file    Readme.txt 
  * @version 1.0.1
  * @date    2021-11-08
  * @author  SRA.SLDU.SC Team
  * @brief   USB Type-C and PD firmware for STEVAL-USBPD427S 27W AC-DC adapter.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This firmware package is licensed by ST under Mix Ultimate Liberty+OSS+3rd-party 
  * V1 license SLA0048 the "License"; You may not use this file except in compliance 
  * with the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0048
  ******************************************************************************
  @endverbatim


@par Features
- Software package for the STEVAL-2STPD01 USB Type-C™ Power delivery dual port adapter kit
- Based on the STM32Cube MCU Package for STM32G0 series running on the
ARM® 32-bit Cortex®-M0 STM32G071RBT6 microcontroller
- Included libraries: Power Sharing and USBPD Stack

@par Description
The STSW-2STPD01 software package for the STEVAL-2STPD01 evaluation kit contains an application example
specific for the STM32G071RBT6 microcontroller embedding the USB Type-C and Power Delivery management
with the two ports and a module featuring the power sharing on these two stages.

To set up the demo and run the application with two Sink devices, you need:
<ul>
<li>an STEVAL-2STPD01 evaluation kit</li>
<li>a 24V, 6A DC power supply: the rate of the input DC power supply can be between 20W (for example, 6V,
3.4A) and 144W (for example, 24V, 6A)</li>
<li>two Type-C cables</li>
<li>two Sink platforms (for example, two USB-C and Power Delivery mobile phones)</li>
</ul>
The application can support 144W rated power supply (voltage = 24V, current = 6A) 
plugged to the STEVAL-2STPD01 J801 connector allowing the board to reach the 120W maximum power rate 
(60W for each port) and making the PDOs (5V, 9V, 15V and 20V, up to 3A) available to both ports.

Configuring the board input power rate (using STM32CubeMonUCPD application avaialble on st.com) 
with a low power supply as input, enabling the power sharing module to share the available 
power between the two ports.
Note: To choose the most suitable power supply, refer to the following absolute ratings:
<ul>
<li>Voltage min. = 6V  </li>
<li>Voltage max. = 24V </li>
<li>Current min. = 1A  </li>
<li>Current max. = 6A  </li>
<li>Power   min. = 20W </li>
</ul>
The power sharing firmware module integrated in the STSW-2STP01 software automatically defines the PDOs
available to each port.
<b>Note: Do not exceed these limits</b>


For more details and information about how-to manage the firmware
refere to the online documentation for STSW-2STPD01 (http://www.st.com)
<ul>
<li>DB4512</li> 
<li>UM2882</li>
<li>DB4525</li> 
<li>UM2880</li>
</ul>

/*
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
