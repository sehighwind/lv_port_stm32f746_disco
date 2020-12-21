
#include "stm32f7xx.h"
#include "lvgl/lvgl.h"
#include "main.h"
#include "hal_stm_lvgl/tft/tft.h"
#include "hal_stm_lvgl/touchpad/touchpad.h"

#include "lv_examples/lv_examples.h"

static void GpioToggle(void);
static void CPU_CACHE_Enable(void);

int main(void)
{
/* Enable the CPU Cache */
  CPU_CACHE_Enable();

  /* STM32F7xx HAL library initialization:
       - Configure the Flash prefetch
       - Systick timer is configured by default as source of time base, but user 
         can eventually implement his proper time base source (a general purpose 
         timer for example or other time source), keeping in mind that Time base 
         duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and 
         handled in milliseconds basis.
       - Set NVIC Group Priority to 4
       - Low Level Initialization
     */
  HAL_Init();

    lv_init();

    tft_init();
    touchpad_init();



    //lv_demo_music();
    lv_demo_widgets();
    //lv_demo_benchmark();
    //BSP_LED_Init(LED1);

    while (1)
    {
        HAL_Delay(5);
        lv_task_handler();
		
		/* Execute the code from QSPI memory ------------------------------------ */
    	//GpioToggle();
    }
}

/**
* @brief  CPU L1-Cache enable.
* @param  None
* @retval None
*/
static void CPU_CACHE_Enable(void)
{
  /* Enable I-Cache */
  SCB_EnableICache();

  /* Enable D-Cache */
  SCB_EnableDCache();
}


/**
  * @brief  Toggle the GPIOs
  * @param  None
  * @retval None
  */
#if defined(__CC_ARM)
 #pragma arm section code = ".qspi"
 #pragma no_inline
 static void GpioToggle(void)
 #elif defined(__ICCARM__)
 static void GpioToggle(void) @ ".qspi"
 #elif defined(__GNUC__)
 static void __attribute__((section(".qspi"), noinline)) GpioToggle(void)
 #endif
{
  static uint32_t lastTick = 0;

  if( HAL_GetTick()-lastTick > 10000 )
  {
	  BSP_LED_Toggle(LED1);
	  lastTick = HAL_GetTick();
  }

}


