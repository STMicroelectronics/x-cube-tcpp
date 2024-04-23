/**
  ******************************************************************************
  * @file    snk1m1_conf.h
  * @author  MCD Application Team
  * @brief   X-NUCLEO-DRP1M1 board configuration file.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef SNK1M1_CONF_H__
#define SNK1M1_CONF_H__

#ifdef __cplusplus
extern "C" {
#endif

#define VDDA_APPLI                                  3300u

/* Maximum digital value of the ADC output (12 Bits resolution)
   To convert ADC measurement to an absolute voltage value:
   VCHANNELx = ADCx_DATA x (VDD/ADC_FULL_SCALE)
  */
#define ADC_FULL_SCALE       (0x0FFFU)

#define SNK1M1_VSENSE_RA                            200u /* VBUS voltage divider RA in milliohm */
#define SNK1M1_VSENSE_RB                            40u  /* VBUS voltage divider RB in milliohm */

#define SNK1M1_ISENSE_GA                            42u  /* Current measure gain In V/V */
#define SNK1M1_ISENSE_RS                            7u   /* Current measure shunt resistor in milliohm */

#ifdef __cplusplus
}
#endif

#endif /* SNK1M1_CONF_H__*/
