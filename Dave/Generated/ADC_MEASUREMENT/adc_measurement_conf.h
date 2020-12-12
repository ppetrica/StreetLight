/*********************************************************************************************************************
* DAVE APP Name : ADC_MEASUREMENT       APP Version: 4.1.22
*
* NOTE:
* This file is generated by DAVE. Any manual modification done to this file will be lost when the code is regenerated.
*********************************************************************************************************************/

/* @cond
 ***********************************************************************************************************************
 *
 * Copyright (c) 2015-2016, Infineon Technologies AG
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,are permitted provided that the
 * following conditions are met:
 *
 *   Redistributions of source code must retain the above copyright notice, this list of conditions and the  following
 *   disclaimer.
 *
 *   Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *   following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *   Neither the name of the copyright holders nor the names of its contributors may be used to endorse or promote
 *   products derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE  FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY,OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT  OF THE
 * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * To improve the quality of the software, users are encouraged to share modifications, enhancements or bug fixes
 * with Infineon Technologies AG (dave@infineon.com).
 ***********************************************************************************************************************
 *
 * Change History
 * --------------
 *
 * 2015-02-16:
 *     - Initial version for DAVEv4.<BR>
 *
 * 2015-02-19:
 *     - Added GetResult APIs for XMC1100 devices.
 *
 * 2015-05-14:
 *     - Optimized the Manifest.<BR>
 *     - Modified the structure ADC_MEASUREMENT_channel_t to ADC_MEASUREMENT_CHANNEL_t.<BR>
 *
 * 2015-06-26:
 *     - Added input class resource consumption.<BR>
 *
 * 2015-07-09:
 *     - Documentation updated.<BR>
 *
 * 2015-07-31:
 *     - Updated the APP to support new devices.<BR>
 *
 * 2015-10-01:
 *     - Updated the APP to support the interrupt control in XMC14.<BR>
 *
 * 2015-10-08:
 *     - Added support for XMC4700/XMC4800 devices.<BR>
 *
 * 2015-10-20:
 *     - Documentation updated.<BR>
 *
 * 2015-12-03:
 *     - Added support for XMC4300 device.<BR>
 *
 * 2015-02-09:
 *     - Updated the APP for the minimum required ANALOG_IO APP version.<BR>
 *
 * 2016-03-18:
 *     - Maximum number of channels for XMC41/42 increased to 9.<BR>
 *     - Added Post calibration time for XMC1x series in Total conversion Time.<BR>
 *     - Fixed the ADC_MEASUREMENT_Init() for ANALOG_IO initialization.<BR>
 *     - For XMC1100 series only: Deprecated the API ADC_MEASUREMENT_GetResult() and 
 *       ADC_MEASUREMENT_GetDetailedResult().<BR>
 *     - For XMC1100 series only: Added new API ADC_MEASUREMENT_GetGlobalResult() and 
 *       ADC_MEASUREMENT_GetGlobalDetailedResult().<BR>
 *
 * 2016-08-18:
 *     - Minimum sample time at low frequencies changed to incorporate Errata ADC_AI.H006 and ADC_AI.H007.
 *
 * @endcond
 *
 */

#ifndef ADC_MEASUREMENTCONF_H
#define ADC_MEASUREMENTCONF_H

/***********************************************************************************************************************
 * HEADER FILES
 **********************************************************************************************************************/

/**********************************************************************************************************************
 * MACROS
 **********************************************************************************************************************/


#define ADC_MEASUREMENT_MAXCHANNELS (1U)

#define EndofMeasure IRQ_Hdlr_16
#define ADC_MEASUREMENT_Channel_A (ADC_MEASUREMENT_Channel_A_handle)
#define ADC_MEASUREMENT_Channel_A_RES (VADC_G0->RES[15])


#define ADC_MEASUREMENT_CPU_4X
#define ADC_MEASUREMENT_ICLASS_NUM (0U)
#define ADC_MEASUREMENT_MAJOR_VERSION (4U) /**< Major version number of ADC_MEASUREMENT APP*/
#define ADC_MEASUREMENT_MINOR_VERSION (1U) /**< Minor version number of ADC_MEASUREMENT APP*/
#define ADC_MEASUREMENT_PATCH_VERSION (22U) /**< Patch version number of ADC_MEASUREMENT APP*/

  
#endif //ADC_MEASUREMENTCONF_H_

