/****************************************************************************
 * include/nuttx/audio/wm8904.h
 *
 *   Copyright (C) 2014 Gregory Nutt. All rights reserved.
 *   Author:  Gregory Nutt <gnutt@nuttx.org>
 *
 * Reference:
 *   "WM8904 Ultra Low Power CODEC for Portable Audio Applications, Pre-
 *    Production", September 2012, Rev 3.3, Wolfson Microelectronics
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

#ifndef __INCLUDE_NUTTX_AUDIO_WM8904_H
#define __INCLUDE_NUTTX_AUDIO_WM8904_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <stdint.h>
#include <stdbool.h>

#include <nuttx/irq.h>

#ifdef CONFIG_AUDIO_WM8904

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
/* Configuration ************************************************************
 *
 * CONFIG_AUDIO_WM8904 - Enabled WM8904 support
 */

/* Pre-requisistes */

#ifndef CONFIG_I2C_TRANSFER
#  error "CONFIG_I2C_TRANSFER is required in the I2C configuration"
#endif

/* Default configuration values */

/* Helper macros ************************************************************/

#define WM8904_ATTACH(s,isr,arg) ((s)->attach(s,isr,arg))
#define WM8904_DETACH(s)         ((s)->attach(s,NULL,NULL))
#define WM8904_ENABLE(s)         ((s)->enable(s,true))
#define WM8904_DISABLE(s)        ((s)->enable(s,false))
#define WM8904_CLEAR(s)          ((s)->clear(s))

/****************************************************************************
 * Public Types
 ****************************************************************************/
/* This is the type of the WM8904 interrupt handler.  The lower level code
 * will intercept the interrupt and provide the upper level with the private
 * data that was provided when the interrupt was attached.
 */

struct wm8904_lower_s; /* Forward reference.  Defined below */

typedef CODE int (*wm8904_handler_t)(FAR const struct wm8904_lower_s *lower,
                                     FAR void *arg);

/* A reference to a structure of this type must be passed to the WM8904
 * driver.  This structure provides information about the configuration
 * of the WM8904 and provides some board-specific hooks.
 *
 * Memory for this structure is provided by the caller.  It is not copied
 * by the driver and is presumed to persist while the driver is active.
 */

struct wm8904_lower_s
{
  /* Device characterization */

  uint32_t frequency;  /* Initial I2C frequency */
  uint8_t address;     /* 7-bit I2C address (only bits 0-6 used) */

  /* IRQ/GPIO access callbacks.  These operations all hidden behind
   * callbacks to isolate the WM8904 driver from differences in GPIO
   * interrupt handling by varying boards and MCUs.  If possible,
   * interrupts should be configured on both rising and falling edges
   * so that contact and loss-of-contact events can be detected.
   *
   * attach  - Attach the WM8904 interrupt handler to the GPIO interrupt
   * enable  - Enable or disable the GPIO interrupt
   * clear   - Acknowledge/clear any pending GPIO interrupt
   */

  int  (*attach)(FAR const struct wm8904_lower_s *lower, wm8904_handler_t isr,
                 FAR void *arg);
  void (*enable)(FAR const struct wm8904_lower_s *lower, bool enable);
  void (*clear)(FAR const struct wm8904_lower_s *lower);
};

/****************************************************************************
 * Public Data
 ****************************************************************************/

#ifdef __cplusplus
#define EXTERN extern "C"
extern "C"
{
#else
#define EXTERN extern
#endif

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

/****************************************************************************
 * Name: wm8904_register
 *
 * Description:
 *   Initialize the WM8904 device.
 *
 * Input Parameters:
 *   i2c     - An I2C driver instance
 *   i2s     - An I2S driver instance
 *   lower   - Persistent board configuration data
 *   minor   - The input device minor number
 *   session - Returned if multi-sessions are supported
 *
 * Returned Value:
 *   Zero is returned on success.  Otherwise, a negated errno value is
 *   returned to indicate the nature of the failure.
 *
 ****************************************************************************/

struct i2c_dev_s;         /* Forward reference. Defined in include/nuttx/i2c.h */
struct i2s_dev_s;         /* Forward reference. Defined in include/nuttx/audio/i2s.h */
struct audio_lowerhalf_s; /* Forward reference. Defined in nuttx/audio/audio.h */

FAR struct audio_lowerhalf_s *
  wm8904_initialize(FAR struct i2c_dev_s *i2c, FAR struct i2s_dev_s *i2s,
                    FAR const struct wm8904_lower_s *lower, unsigned int devno);

#undef EXTERN
#ifdef __cplusplus
}
#endif

#endif /* CONFIG_AUDIO_WM8904 */
#endif /* __INCLUDE_NUTTX_AUDIO_WM8904_H */