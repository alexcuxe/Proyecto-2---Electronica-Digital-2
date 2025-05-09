/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "math.h"
#include "tones.h"
#include "stdint.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
DAC_HandleTypeDef hdac;
DMA_HandleTypeDef hdma_dac1;
DMA_HandleTypeDef hdma_dac2;

TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim4;
TIM_HandleTypeDef htim6;
TIM_HandleTypeDef htim7;

UART_HandleTypeDef huart4;
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
//uart variables
uint8_t buffer[1];
uint8_t toneNumber = 0;
uint8_t melodyIndex1 = 0;
uint8_t melodyIndex2 = 0;

int *tone1 = 0;
int *duration1 = 0;
int Nsize1 = 0;

int *tone2 = 0;
int *duration2 = 0;
int Nsize2 = 0;

typedef struct {
	int *data;
	int Dsize;
	int *durations;
} SongData;

//Music variables
#define size 128
uint32_t Ysine[size];
#define pi 3.1415926

#define TIM_FREQ 84000000
#define PSC 1
#define PSC_durations 8400



int picoParkMelody[] = {
    NOTE_E5, NOTE_E5, NOTE_FS5, NOTE_G5, NOTE_A5, NOTE_G5, NOTE_FS5, NOTE_E5,
    NOTE_CS5, NOTE_CS5, NOTE_D5, NOTE_E5, NOTE_FS5, NOTE_E5, NOTE_D5, NOTE_CS5,
    NOTE_B4, NOTE_B4, NOTE_CS5, NOTE_D5, NOTE_E5, NOTE_D5, NOTE_CS5, NOTE_B4,
    NOTE_E5, NOTE_E5, NOTE_FS5, NOTE_G5, NOTE_A5, NOTE_G5, NOTE_FS5, NOTE_E5,
    NOTE_CS5, NOTE_CS5, NOTE_D5, NOTE_E5, NOTE_FS5, NOTE_E5, NOTE_D5, NOTE_CS5,
    NOTE_B4, NOTE_GS4, NOTE_FS4, NOTE_E4, NOTE_D4, NOTE_CS4, NOTE_B3, NOTE_E4,
    NOTE_FS4, NOTE_GS4, NOTE_B4, NOTE_CS5, NOTE_D5, NOTE_E5, NOTE_FS5, NOTE_G5,
    NOTE_A5, NOTE_B5, NOTE_CS6, NOTE_DS6, NOTE_E6, NOTE_DS6, NOTE_CS6, NOTE_B5
};

int picoParkNoteDurations[] = {
    200, 200, 200, 200, 200, 200, 200, 200,
    200, 200, 200, 200, 200, 200, 200, 200,
    200, 200, 200, 200, 200, 200, 200, 200,
    200, 200, 200, 200, 200, 200, 200, 200,
    200, 200, 200, 200, 200, 200, 200, 200,
    200, 200, 200, 200, 200, 200, 200, 200,
    200, 200, 200, 200, 200, 200, 200, 200,
    200, 200, 200, 200, 200, 200, 200, 200
};

int picoParkStartMelody[] = {
    NOTE_G5, NOTE_G5, NOTE_A5, NOTE_B5, NOTE_C6, NOTE_B5, NOTE_A5, NOTE_G5,
    NOTE_E5, NOTE_E5, NOTE_FS5, NOTE_G5, NOTE_A5, NOTE_G5, NOTE_FS5, NOTE_E5,
    NOTE_C5, NOTE_C5, NOTE_D5, NOTE_E5, NOTE_F5, NOTE_E5, NOTE_D5, NOTE_C5,
    NOTE_G5, NOTE_G5, NOTE_A5, NOTE_B5, NOTE_C6, NOTE_B5, NOTE_A5, NOTE_G5,
    NOTE_E5, NOTE_E5, NOTE_FS5, NOTE_G5, NOTE_A5, NOTE_G5, NOTE_FS5, NOTE_E5,
    NOTE_C5, NOTE_A4, NOTE_G4, NOTE_FS4, NOTE_E4, NOTE_D4, NOTE_C4, NOTE_G4,
    NOTE_A4, NOTE_B4, NOTE_C5, NOTE_D5, NOTE_E5, NOTE_FS5, NOTE_G5, NOTE_A5,
    NOTE_B5, NOTE_C6, NOTE_D6, NOTE_E6, NOTE_F6, NOTE_E6, NOTE_D6, NOTE_C6
};

int picoParkStartNoteDurations[] = {
    200, 200, 200, 200, 200, 200, 200, 200,
    200, 200, 200, 200, 200, 200, 200, 200,
    200, 200, 200, 200, 200, 200, 200, 200,
    200, 200, 200, 200, 200, 200, 200, 200,
    200, 200, 200, 200, 200, 200, 200, 200,
    200, 200, 200, 200, 200, 200, 200, 200,
    200, 200, 200, 200, 200, 200, 200, 200,
    200, 200, 200, 200, 200, 200, 200, 200
};

int picoParkGrassLandMelody[] = {
    NOTE_C5, NOTE_E5, NOTE_G5, NOTE_C6,
    NOTE_D5, NOTE_F5, NOTE_A5, NOTE_D6,
    NOTE_E5, NOTE_G5, NOTE_B5, NOTE_E6,
    NOTE_F5, NOTE_A5, NOTE_C6, NOTE_F6,
    NOTE_G4, NOTE_C5, NOTE_E5, NOTE_G5,
    NOTE_A4, NOTE_D5, NOTE_F5, NOTE_A5,
    NOTE_B4, NOTE_E5, NOTE_G5, NOTE_B5,
    NOTE_C5, NOTE_F5, NOTE_A5, NOTE_C6
};

int picoParkGrassLandNoteDurations[] = {
    200, 200, 200, 200,
    200, 200, 200, 200,
    200, 200, 200, 200,
    200, 200, 200, 200,
    200, 200, 200, 200,
    200, 200, 200, 200,
    200, 200, 200, 200,
    200, 200, 200, 200
};

// Notas de la melodía de Pico Park
int picopark_notes[] = {
    NOTE_E5, NOTE_FS5, NOTE_B5, NOTE_A5, NOTE_FS5, NOTE_E5, NOTE_FS5,
    NOTE_A5, NOTE_B5, NOTE_E6, NOTE_FS6, NOTE_B5, NOTE_A5, NOTE_FS5,
    NOTE_E5, NOTE_FS5, NOTE_B5, NOTE_A5, NOTE_FS5, NOTE_E5, NOTE_FS5,
    NOTE_A5, NOTE_B5, NOTE_E6, NOTE_FS6, NOTE_B5, NOTE_A5, NOTE_FS5,

    NOTE_B5, NOTE_CS6, NOTE_FS6, NOTE_E6, NOTE_CS6, NOTE_B5, NOTE_CS6,
    NOTE_E6, NOTE_FS6, NOTE_B6, NOTE_CS7, NOTE_FS6, NOTE_E6, NOTE_CS6,
    NOTE_B5, NOTE_CS6, NOTE_FS6, NOTE_E6, NOTE_CS6, NOTE_B5, NOTE_CS6,
    NOTE_E6, NOTE_FS6, NOTE_B6, NOTE_CS7, NOTE_FS6, NOTE_E6, NOTE_CS6
};

// Duración de cada nota en milisegundos
int picopark_durations[] = {
    250, 250, 250, 250, 250, 250, 250,
    250, 250, 250, 250, 250, 250, 250,
    250, 250, 250, 250, 250, 250, 250,
    250, 250, 250, 250, 250, 250, 250,

    250, 250, 250, 250, 250, 250, 250,
    250, 250, 250, 250, 250, 250, 250,
    250, 250, 250, 250, 250, 250, 250,
    250, 250, 250, 250, 250, 250, 250
};

// Notas de la melodía durante el gameplay de Pico Park
int picopark_game_notes[] = {
    NOTE_E5, NOTE_FS5, NOTE_B5, NOTE_A5, NOTE_FS5, NOTE_E5, NOTE_FS5, NOTE_A5,
    NOTE_B5, NOTE_E6, NOTE_D6, NOTE_B5, NOTE_A5, NOTE_FS5, NOTE_E5, NOTE_FS5,
    NOTE_A5, NOTE_B5, NOTE_E6, NOTE_FS6, NOTE_E6, NOTE_B5, NOTE_A5, NOTE_FS5,

    // Variación rítmica
    NOTE_B5, NOTE_CS6, NOTE_FS6, NOTE_E6, NOTE_CS6, NOTE_B5, NOTE_A5, NOTE_B5,
    NOTE_CS6, NOTE_FS6, NOTE_E6, NOTE_CS6, NOTE_B5, NOTE_A5, NOTE_FS5, NOTE_E5,

    // Parte final
    NOTE_FS5, NOTE_A5, NOTE_B5, NOTE_E6, NOTE_FS6, NOTE_B5, NOTE_A5, NOTE_FS5,
    NOTE_E5, NOTE_FS5, NOTE_A5, NOTE_B5, NOTE_E6, NOTE_FS6, NOTE_E6, NOTE_B5
};

// Duración de cada nota en milisegundos (con variaciones rítmicas)
int picopark_game_durations[] = {
    180, 180, 180, 180, 180, 180, 180, 180,
    180, 180, 180, 180, 180, 180, 180, 180,
    180, 180, 250, 250, 180, 180, 180, 180,

    // Variación rítmica
    150, 150, 150, 150, 150, 150, 150, 150,
    150, 250, 180, 150, 150, 150, 150, 150,

    // Parte final
    180, 180, 180, 250, 250, 180, 180, 180,
    180, 180, 180, 180, 250, 250, 180, 180
};



int hornpipe_notes[] = {
    NOTE_D4, NOTE_D4, NOTE_D4, 0,
    NOTE_G4, NOTE_G4, NOTE_G4, 0,
    NOTE_B4, NOTE_B4, NOTE_B4, 0,
    NOTE_A4, NOTE_G4, NOTE_FS4, NOTE_E4,
    NOTE_D4, 0, 0, 0
};

// Duración de cada nota en milisegundos (ajustable)
int hornpipe_durations[] = {
    200, 200, 200, 50,
    200, 200, 200, 50,
    200, 200, 200, 50,
    150, 150, 150, 150,
    400, 50, 50, 50
};



int picoParkJumpSimpleMelody[] = {
    NOTE_E5, NOTE_G5, NOTE_E6
};

// Duraciones (en milisegundos) para el salto simple
int picoParkJumpSimpleDurations[] = {
    50, 50, 50
};


int interstellar[] = {
		NOTE_C4, NOTE_G3, 0, NOTE_C4, NOTE_G3
};

int interstellarDurations[] = {
		500, 500, 150, 800, 800
};


int silent[] = {0};
int silentDur[] = {10};


int picoParkLevelWin1Melody[] = {
		NOTE_E4, NOTE_F4, NOTE_C5, NOTE_B4
};

// Duraciones (en milisegundos) para la victoria de nivel (Opción 1)
int picoParkLevelWin1Durations[] = {
		500, 200, 600, 250
};



int picoParkDead[] = {
    NOTE_C6, NOTE_E6, NOTE_G6, NOTE_C7
};

// Duraciones (en milisegundos) para la victoria de nivel (Opción 1)
int picoParkDeadDur[] = {
    150, 150, 150, 300
};


// Arreglo de notas para la victoria de nivel (Opción 7 - Ascenso Brillante)
int victoryBrightAscendMelody[] = {
    NOTE_C5, NOTE_D5, NOTE_E5, NOTE_G5, NOTE_C5, NOTE_C5, NOTE_D5, NOTE_E5, NOTE_G5, NOTE_C5
};

// Duraciones (en milisegundos) para la victoria de nivel (Opción 7 - Ascenso Brillante)
int victoryBrightAscendDurations[] = {
    100, 100, 100, 200, 400, 100, 100, 100, 200, 400
};


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_DAC_Init(void);
static void MX_TIM6_Init(void);
static void MX_UART4_Init(void);
static void MX_TIM7_Init(void);
static void MX_TIM3_Init(void);
static void MX_TIM4_Init(void);
/* USER CODE BEGIN PFP */
void genSin(void);
int calculateARR(int freq);
int ARRdurationTimers(int time);
void playTone(int *tone, int *duration, int *pause, int Nsize);
void noTone1();
void noTone2();
void selectSong(SongData songData);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
SongData cancion0 = {picoParkStartMelody, sizeof(picoParkStartMelody)/sizeof(picoParkStartMelody[0]), picoParkStartNoteDurations};
SongData cancion1 = {picoParkGrassLandMelody, sizeof(picoParkGrassLandMelody)/sizeof(picoParkGrassLandMelody[0]), picoParkGrassLandNoteDurations};
SongData cancion2 = {picoParkMelody, sizeof(picoParkMelody)/sizeof(picoParkMelody[0]), picoParkNoteDurations};
SongData cancion3 = {picopark_game_notes, sizeof(picopark_game_notes)/sizeof(picopark_game_notes[0]), picopark_game_durations};
SongData cancion4 = {picopark_notes, sizeof(picopark_notes)/sizeof(picopark_notes[0]), picopark_durations};
SongData cancion5 = {hornpipe_notes, sizeof(hornpipe_notes)/sizeof(hornpipe_notes[0]), hornpipe_durations};

SongData sound1 = {picoParkJumpSimpleMelody, sizeof(picoParkJumpSimpleMelody)/sizeof(picoParkJumpSimpleMelody[0]), picoParkJumpSimpleDurations};
SongData sound2 = {interstellar, sizeof(interstellar)/sizeof(interstellar[0]), interstellarDurations};
SongData sound3 = {silent, sizeof(silent)/sizeof(silent), silentDur};
SongData sound4 = {picoParkLevelWin1Melody, sizeof(picoParkLevelWin1Melody)/sizeof(picoParkLevelWin1Melody[0]), picoParkLevelWin1Durations};
SongData sound5 = {picoParkDead, sizeof(picoParkDead)/sizeof(picoParkDead[0]), picoParkDeadDur};
SongData sound6 = {victoryBrightAscendMelody, sizeof(victoryBrightAscendMelody)/sizeof(victoryBrightAscendMelody[0]), victoryBrightAscendDurations};


void genSin(void){
	for(int x = 0; x < size; x++) Ysine[x] = ((sin(x * 2 * pi / size) + 1) * (4096/2));
}

int calculateARR(int freq){			// calculates period
	if(freq == 0) return 0;
	int TF = size * freq;
	return ((TIM_FREQ / ((PSC + 1) * TF)) - 1);
}

int ARRdurationTimers(int time){			// calculates period
	if(time == 0) return 0;
	int freq = 1000 / time;
	return ((TIM_FREQ / ((PSC_durations + 1) * freq)) - 1);
}

void selectSong1(SongData songData){
	  tone1 = songData.data;
	  duration1 = songData.durations;
	  Nsize1 = songData.Dsize;
}

void selectSong2(SongData songData){
	  tone2 = songData.data;
	  duration2 = songData.durations;
	  Nsize2 = songData.Dsize;
}

void noTone1(){
	TIM6 -> ARR = 0;								// sets ARR to zero
}

void noTone2(){
	TIM7 -> ARR = 0;								// sets ARR to zero
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART2_UART_Init();
  MX_DAC_Init();
  MX_TIM6_Init();
  MX_UART4_Init();
  MX_TIM7_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  /* USER CODE BEGIN 2 */
  genSin();
  HAL_DAC_Init(&hdac);
  HAL_TIM_Base_Start(&htim6);
  HAL_TIM_Base_Start(&htim7);

  HAL_UART_Receive_IT(&huart4, buffer, 1);		// Start receiving, save data to buffer

  HAL_DAC_Start_DMA(&hdac, DAC1_CHANNEL_1, Ysine, size, DAC_ALIGN_12B_R);
  HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_2, Ysine, size, DAC_ALIGN_12B_R);

  selectSong1(sound3);
  selectSong2(sound3);
  noTone1();
  noTone2();
  HAL_TIM_Base_Start_IT(&htim3);		// starts timer 3, select song first
  HAL_TIM_Base_Start_IT(&htim4);		// starts timer 3, select song first
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief DAC Initialization Function
  * @param None
  * @retval None
  */
static void MX_DAC_Init(void)
{

  /* USER CODE BEGIN DAC_Init 0 */

  /* USER CODE END DAC_Init 0 */

  DAC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN DAC_Init 1 */

  /* USER CODE END DAC_Init 1 */

  /** DAC Initialization
  */
  hdac.Instance = DAC;
  if (HAL_DAC_Init(&hdac) != HAL_OK)
  {
    Error_Handler();
  }

  /** DAC channel OUT1 config
  */
  sConfig.DAC_Trigger = DAC_TRIGGER_T6_TRGO;
  sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
  if (HAL_DAC_ConfigChannel(&hdac, &sConfig, DAC_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }

  /** DAC channel OUT2 config
  */
  sConfig.DAC_Trigger = DAC_TRIGGER_T7_TRGO;
  if (HAL_DAC_ConfigChannel(&hdac, &sConfig, DAC_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN DAC_Init 2 */

  /* USER CODE END DAC_Init 2 */

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 8400-1;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 9-1;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */

}

/**
  * @brief TIM4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM4_Init(void)
{

  /* USER CODE BEGIN TIM4_Init 0 */

  /* USER CODE END TIM4_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM4_Init 1 */

  /* USER CODE END TIM4_Init 1 */
  htim4.Instance = TIM4;
  htim4.Init.Prescaler = 8400-1;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = 9-1;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim4) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim4, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM4_Init 2 */

  /* USER CODE END TIM4_Init 2 */

}

/**
  * @brief TIM6 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM6_Init(void)
{

  /* USER CODE BEGIN TIM6_Init 0 */

  /* USER CODE END TIM6_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM6_Init 1 */

  /* USER CODE END TIM6_Init 1 */
  htim6.Instance = TIM6;
  htim6.Init.Prescaler = 1;
  htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim6.Init.Period = 1000-1;
  htim6.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim6) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim6, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM6_Init 2 */

  /* USER CODE END TIM6_Init 2 */

}

/**
  * @brief TIM7 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM7_Init(void)
{

  /* USER CODE BEGIN TIM7_Init 0 */

  /* USER CODE END TIM7_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM7_Init 1 */

  /* USER CODE END TIM7_Init 1 */
  htim7.Instance = TIM7;
  htim7.Init.Prescaler = 1;
  htim7.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim7.Init.Period = 1000-1;
  htim7.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim7) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim7, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM7_Init 2 */

  /* USER CODE END TIM7_Init 2 */

}

/**
  * @brief UART4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_UART4_Init(void)
{

  /* USER CODE BEGIN UART4_Init 0 */

  /* USER CODE END UART4_Init 0 */

  /* USER CODE BEGIN UART4_Init 1 */

  /* USER CODE END UART4_Init 1 */
  huart4.Instance = UART4;
  huart4.Init.BaudRate = 115200;
  huart4.Init.WordLength = UART_WORDLENGTH_8B;
  huart4.Init.StopBits = UART_STOPBITS_1;
  huart4.Init.Parity = UART_PARITY_NONE;
  huart4.Init.Mode = UART_MODE_TX_RX;
  huart4.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart4.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart4) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN UART4_Init 2 */

  /* USER CODE END UART4_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Stream5_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);
  /* DMA1_Stream6_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream6_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream6_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LED2_Pin */
  GPIO_InitStruct.Pin = LED2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED2_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LED_Pin */
  GPIO_InitStruct.Pin = LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED_GPIO_Port, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	HAL_UART_Transmit(&huart2, "Send: ",sizeof("Send: "), 250);

	switch (buffer[0]) {
		case '0': noTone1(); selectSong1(cancion0); melodyIndex1 = 0; TIM4 -> ARR = ARRdurationTimers(duration1[melodyIndex1]); break;
		case '1': noTone1(); selectSong1(cancion1); melodyIndex1 = 0; TIM4 -> ARR = ARRdurationTimers(duration1[melodyIndex1]); break;
		case '2': noTone1(); selectSong1(cancion2); melodyIndex1 = 0; TIM4 -> ARR = ARRdurationTimers(duration1[melodyIndex1]); break;
		case '3': noTone1(); selectSong1(cancion3); melodyIndex1 = 0; TIM4 -> ARR = ARRdurationTimers(duration1[melodyIndex1]); break;
		case '4': noTone1(); selectSong1(cancion4); melodyIndex1 = 0; TIM4 -> ARR = ARRdurationTimers(duration1[melodyIndex1]); break;
		case '5': noTone1(); selectSong1(cancion5); melodyIndex1 = 0; TIM4 -> ARR = ARRdurationTimers(duration1[melodyIndex1]); break;
		case '6': noTone1(); selectSong1(sound3); melodyIndex1 = 0; TIM4 -> ARR = ARRdurationTimers(duration1[melodyIndex1]); break;


		case 'A': noTone2(); selectSong2(sound1); melodyIndex2 = 0; TIM3 -> ARR = ARRdurationTimers(duration2[melodyIndex2]); break;
		case 'B': noTone2(); selectSong2(sound6); melodyIndex2 = 0; TIM3 -> ARR = ARRdurationTimers(duration2[melodyIndex2]); break;
		case 'C': noTone2(); selectSong2(sound4); melodyIndex2 = 0; TIM3 -> ARR = ARRdurationTimers(duration2[melodyIndex2]); break;


		default: HAL_UART_Transmit(&huart2, buffer[0],sizeof(buffer[0]), 250); break;
	}

	HAL_UART_Receive_IT(&huart4, buffer, 1);		// Restart receiving
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	// Channel 1 DAC control
	if(htim == &htim4){
		HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);
		noTone1();									// pause
		int valARR1= calculateARR(tone1[melodyIndex1]);			//calculate ARR values
		TIM6 -> ARR = valARR1;						// sets ARR value
		TIM4 -> ARR = ARRdurationTimers(duration1[melodyIndex1]);
		melodyIndex1++;
		if(melodyIndex1 >= Nsize1) melodyIndex1 = 0;
	}

	// Channel 2 DAC control
	if(htim == &htim3){
		HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
		noTone2();									// pause
		if(melodyIndex2 >= Nsize2) TIM7 -> ARR = 0;
		else{
			int valARR2 = calculateARR(tone2[melodyIndex2]);			//calculate ARR values
			TIM7 -> ARR = valARR2;										// sets ARR value
			TIM3 -> ARR = ARRdurationTimers(duration2[melodyIndex2]);
			melodyIndex2++;
		}
	}
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
