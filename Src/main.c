/**
 ******************************************************************************
 * @file    main.c
 * @author  MCU Application Team
 * @author  AssemCorp Application Team
 * @brief   Main program body
 * @date
 ******************************************************************************
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
/* define ------------------------------------------------------------------*/
#define LED_GPIO_PORT GPIOB
#define LED_GPIO_PIN GPIO_PIN_0
#define LED2_GPIO_PIN GPIO_PIN_1

#define BUTTON_GPIO_PORT GPIOB
#define BUTTON_GPIO_PIN GPIO_PIN_4

#define BUTTON2_GPIO_PORT GPIOB
#define BUTTON2_GPIO_PIN GPIO_PIN_5
/* Private define ------------------------------------------------------------*/
#define FLASH_USER_START_ADDR 0x08005C00
#define SAVE_BYTE 128
/* Private variables ---------------------------------------------------------*/
typedef struct
{
  uint32_t arrA[SAVE_BYTE];
} NewDataType; /* Struct define */
#define VarA (*(volatile NewDataType *)FLASH_USER_START_ADDR)

uint32_t DATA[SAVE_BYTE] = {
    0x65737341, // "Asse"
    0x726F436D, // "mCor"
    0x6D452070, // "p Em"
    0x4B206572, // "re K"
    0x62617261, // "arab"
    0x20206B65, // "ek  "
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
};

/* Function prototypes -----------------------------------------------*/
static void APP_FlashErase(void);
static void APP_FlashProgram(void);
static void APP_FlashBlank(void);
static void APP_FlashVerify(void);
static void gpioInit(void);
void LED_On(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void LED_Off(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
uint8_t Button_Read(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);

/**
 * @brief  Main program.
 * @retval int
 */
int main(void)
{
  /* Reset of all peripherals, Initializes the Systick. */
  HAL_Init();
  gpioInit();

  while (1)
  {
    
    if (Button_Read(GPIOB, GPIO_PIN_4)) 	// Buton 1 read
    {
      LED_On(GPIOB, GPIO_PIN_0);

      HAL_FLASH_Unlock(); /* Unlock Flash */
      APP_FlashErase();   /* Erase Flash */
      APP_FlashBlank();   /* Check Flash */
      APP_FlashProgram(); /* Program Flash */
      HAL_FLASH_Lock();   /* Lock Flash */
      APP_FlashVerify();  /* Verify Flash */

      HAL_Delay(500);
    }
    else
    {
      LED_Off(GPIOB, GPIO_PIN_0);
    }
		
    if (Button_Read(GPIOB, GPIO_PIN_5))		// Buton 2 read
    {
      LED_On(GPIOB, GPIO_PIN_1);

      HAL_FLASH_Unlock(); /* Unlock Flash */
      APP_FlashErase();   /* Erase Flash */
      HAL_FLASH_Lock();   /* Lock Flash */

      HAL_Delay(500);
    }
    else
    {
      LED_Off(GPIOB, GPIO_PIN_1);
    }
  }
}

/**
 * @brief  Flash Erase
 * @param  None
 * @retval None
 */
static void APP_FlashErase(void)
{
  uint32_t PAGEError = 0;
  FLASH_EraseInitTypeDef EraseInitStruct;

  EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGEERASE;         /* Page Erase */
  EraseInitStruct.PageAddress = FLASH_USER_START_ADDR;           /* Erase Start Address */
  EraseInitStruct.NbPages = sizeof(DATA) / FLASH_PAGE_SIZE;      /* Erase Page Number */
  if (HAL_FLASHEx_Erase(&EraseInitStruct, &PAGEError) != HAL_OK) /* Page Erase */
  {
    APP_ErrorHandler();
  }
}

/**
 * @brief  Flash Program
 * @param  None
 * @retval None
 */
static void APP_FlashProgram(void)
{
  uint32_t flash_program_start = FLASH_USER_START_ADDR;                /* flash program start address */
  uint32_t flash_program_end = (FLASH_USER_START_ADDR + sizeof(DATA)); /* flash program end address */
  uint32_t *src = (uint32_t *)DATA;                                    /* Program data */

  while (flash_program_start < flash_program_end)
  {
    if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_PAGE, flash_program_start, src) == HAL_OK) /* Program */
    {
      flash_program_start += FLASH_PAGE_SIZE; /* flash Start point first page */
      src += FLASH_PAGE_SIZE / 4;             /* Update data point */
    }
  }
}

/**
 * @brief  Flash Check
 * @param  None
 * @retval None
 */
static void APP_FlashBlank(void)
{
  uint32_t addr = 0;

  while (addr < sizeof(DATA))
  {
    if (0xFFFFFFFF != HW32_REG(FLASH_USER_START_ADDR + addr))
    {
      APP_ErrorHandler();
    }
    addr += 4;
  }
}

/**
 * @brief  Flash Verify
 * @param  None
 * @retval None
 */
static void APP_FlashVerify(void)
{
  uint32_t addr = 0;

  while (addr < sizeof(DATA))
  {
    if (DATA[addr / 4] != HW32_REG(FLASH_USER_START_ADDR + addr))
    {
      APP_ErrorHandler();
    }
    addr += 4;
  }
}

/**
 * @brief  gpioInıt
 * @param  None
 * @retval None
 */
static void gpioInit(void)
{
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* LED pin init */
  GPIO_InitStruct.Pin = LED_GPIO_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(LED_GPIO_PORT, &GPIO_InitStruct);

  /* LED pin init */
  GPIO_InitStruct.Pin = LED2_GPIO_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(LED_GPIO_PORT, &GPIO_InitStruct);

  /* Button pin init */
  GPIO_InitStruct.Pin = BUTTON_GPIO_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(BUTTON_GPIO_PORT, &GPIO_InitStruct);

  /* Button2 (PB5) pin init */
  GPIO_InitStruct.Pin = BUTTON2_GPIO_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(BUTTON2_GPIO_PORT, &GPIO_InitStruct);
}

void LED_On(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin) {
    HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_RESET);
}

void LED_Off(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin) {
    HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_SET);
}

uint8_t Button_Read(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin) {
    return (HAL_GPIO_ReadPin(GPIOx, GPIO_Pin) == GPIO_PIN_RESET) ? 1 : 0;
}

/**
 * @brief  This function is executed in case of error occurrence.
 * @param  None
 * @retval None
 */
void APP_ErrorHandler(void)
{
  while (1)
  {
  }
}

#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT Puya *****END OF FILE****/
