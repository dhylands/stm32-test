/**
  ******************************************************************************
  * @file    stm32f4xx_hal_cryp.c
  * @author  MCD Application Team
  * @brief   CRYP HAL module driver.
  *          This file provides firmware functions to manage the following
  *          functionalities of the Cryptography (CRYP) peripheral:
  *           + Initialization, de-initialization, set config and get config  functions
  *           + DES/TDES, AES processing functions
  *           + DMA callback functions
  *           + CRYP IRQ handler management
  *           + Peripheral State functions
  *
  @verbatim
  ==============================================================================
                     ##### How to use this driver #####
  ==============================================================================
    [..]
      The CRYP HAL driver can be used in CRYP or TinyAES IP as follows:

      (#)Initialize the CRYP low level resources by implementing the HAL_CRYP_MspInit():
         (##) Enable the CRYP interface clock using __HAL_RCC_CRYP_CLK_ENABLE()or __HAL_RCC_AES_CLK_ENABLE for TinyAES IP
         (##) In case of using interrupts (e.g. HAL_CRYP_Encrypt_IT())
             (+++) Configure the CRYP interrupt priority using HAL_NVIC_SetPriority()
             (+++) Enable the CRYP IRQ handler using HAL_NVIC_EnableIRQ()
             (+++) In CRYP IRQ handler, call HAL_CRYP_IRQHandler()
         (##) In case of using DMA to control data transfer (e.g. HAL_CRYP_Encrypt_DMA())
             (+++) Enable the DMAx interface clock using __RCC_DMAx_CLK_ENABLE()
             (+++) Configure and enable two DMA streams one for managing data transfer from
                 memory to peripheral (input stream) and another stream for managing data
                 transfer from peripheral to memory (output stream)
             (+++) Associate the initialized DMA handle to the CRYP DMA handle
                 using  __HAL_LINKDMA()
             (+++) Configure the priority and enable the NVIC for the transfer complete
                 interrupt on the two DMA Streams. The output stream should have higher
                 priority than the input stream HAL_NVIC_SetPriority() and HAL_NVIC_EnableIRQ()

      (#)Initialize the CRYP according to the specified parameters :
         (##) The data type: 1-bit, 8-bit, 16-bit or 32-bit.
         (##) The key size: 128, 192 or 256.
         (##) The AlgoMode DES/ TDES Algorithm ECB/CBC or AES Algorithm ECB/CBC/CTR/GCM or CCM.
         (##) The initialization vector (counter). It is not used in ECB mode.
         (##) The key buffer used for encryption/decryption.
         (##) The Header used only in AES GCM and CCM Algorithm for authentication.
         (##) The HeaderSize The size of header buffer in word.
         (##) The B0 block is the first authentication block used only  in AES CCM mode.

      (#)Three processing (encryption/decryption) functions are available:
         (##) Polling mode: encryption and decryption APIs are blocking functions
              i.e. they process the data and wait till the processing is finished,
              e.g. HAL_CRYP_Encrypt & HAL_CRYP_Decrypt
         (##) Interrupt mode: encryption and decryption APIs are not blocking functions
              i.e. they process the data under interrupt,
              e.g. HAL_CRYP_Encrypt_IT & HAL_CRYP_Decrypt_IT
         (##) DMA mode: encryption and decryption APIs are not blocking functions
              i.e. the data transfer is ensured by DMA,
              e.g. HAL_CRYP_Encrypt_DMA & HAL_CRYP_Decrypt_DMA

      (#)When the processing function is called at first time after HAL_CRYP_Init()
         the CRYP peripheral is configured and processes the buffer in input.
         At second call, no need to Initialize the CRYP, user have to get current configuration via
         HAL_CRYP_GetConfig() API, then only  HAL_CRYP_SetConfig() is requested to set
         new parametres, finally user can  start encryption/decryption.

       (#)Call HAL_CRYP_DeInit() to deinitialize the CRYP peripheral.

    [..]
      The cryptographic processor supports following standards:
      (#) The data encryption standard (DES) and Triple-DES (TDES) supported only by CRYP1 IP:
         (##)64-bit data block processing
         (##) chaining modes supported :
             (+++)  Electronic Code Book(ECB)
             (+++)  Cipher Block Chaining (CBC)
         (##) keys length supported :64-bit, 128-bit and 192-bit.
      (#) The advanced encryption standard (AES) supported  by CRYP1 & TinyAES IP:
         (##)128-bit data block processing
         (##) chaining modes supported :
             (+++)  Electronic Code Book(ECB)
             (+++)  Cipher Block Chaining (CBC)
             (+++)  Counter mode (CTR)
             (+++)  Galois/counter mode (GCM/GMAC)
             (+++)  Counter with Cipher Block Chaining-Message(CCM)
         (##) keys length Supported :
             (+++) for CRYP1 IP: 128-bit, 192-bit and 256-bit.
             (+++) for TinyAES IP:  128-bit and 256-bit

    [..]  This section describes the AES Galois/counter mode (GCM) supported by both CRYP1 IP:
      (#)  Algorithm supported :
         (##) Galois/counter mode (GCM)
         (##) Galois message authentication code (GMAC) :is exactly the same as
              GCM algorithm composed only by an header.
      (#)  Four phases are performed in GCM :
         (##) Init phase: IP prepares the GCM hash subkey (H) and do the IV processing
         (##) Header phase: IP processes the Additional Authenticated Data (AAD), with hash
          computation only.
         (##) Payload phase: IP processes the plaintext (P) with hash computation + keystream
          encryption + data XORing. It works in a similar way for ciphertext (C).
         (##) Final phase: IP generates the authenticated tag (T) using the last block of data.
      (#)  structure of message construction in GCM is defined as below  :
         (##) 16 bytes Initial Counter Block (ICB)composed of IV and counter
         (##) The authenticated header A (also knows as Additional Authentication Data AAD)
          this part of the message is only authenticated, not encrypted.
         (##) The plaintext message P is both authenticated and encrypted as ciphertext.
          GCM standard specifies that ciphertext has same bit length as the plaintext.
         (##) The last block is composed of the length of A (on 64 bits) and the length of ciphertext
          (on 64 bits)

    [..]  This section describe The AES Counter with Cipher Block Chaining-Message
          Authentication Code (CCM) supported by both CRYP1 IP:
      (#)  Specific parameters for CCM  :

         (##) B0 block  : According to NIST Special Publication 800-38C,
            The first block B0 is formatted as follows, where l(m) is encoded in
            most-significant-byte first order(see below table 3)

              (+++)  Q: a bit string representation of the octet length of P (plaintext)
              (+++)  q The octet length of the binary representation of the octet length of the payload
              (+++)  A nonce (N), n The octet length of the where n+q=15.
              (+++)  Flags: most significant octet containing four flags for control information,
              (+++)  t The octet length of the MAC.
         (##) B1 block (header) : associated data length(a) concatenated with Associated Data (A)
              the associated data length expressed in bytes (a) defined as below:
            (+++)  If 0 < a < 216-28, then it is encoded as [a]16, i.e. two octets
            (+++)  If 216-28 < a < 232, then it is encoded as 0xff || 0xfe || [a]32, i.e. six octets
            (+++)  If 232 < a < 264, then it is encoded as 0xff || 0xff || [a]64, i.e. ten octets
         (##) CTRx block  : control blocks
            (+++) Generation of CTR1 from first block B0 information :
              equal to B0 with first 5 bits zeroed and most significant bits storing octet
              length of P also zeroed, then incremented by one ( see below Table 4)
            (+++) Generation of CTR0: same as CTR1 with bit[0] set to zero.

      (#)  Four phases are performed in CCM for CRYP1 IP:
         (##) Init phase: IP prepares the GCM hash subkey (H) and do the IV processing
         (##) Header phase: IP processes the Additional Authenticated Data (AAD), with hash
          computation only.
         (##) Payload phase: IP processes the plaintext (P) with hash computation + keystream
          encryption + data XORing. It works in a similar way for ciphertext (C).
         (##) Final phase: IP generates the authenticated tag (T) using the last block of data.

  *** Callback registration ***
  =============================================

  The compilation define  USE_HAL_CRYP_REGISTER_CALLBACKS when set to 1
  allows the user to configure dynamically the driver callbacks.
  Use Functions @ref HAL_CRYP_RegisterCallback() or HAL_CRYP_RegisterXXXCallback()
  to register an interrupt callback.

  Function @ref HAL_CRYP_RegisterCallback() allows to register following callbacks:
    (+) InCpltCallback     :  Input FIFO transfer completed callback.
    (+) OutCpltCallback    : Output FIFO transfer completed callback.
    (+) ErrorCallback      : callback for error detection.
    (+) MspInitCallback    : CRYP MspInit.
    (+) MspDeInitCallback  : CRYP MspDeInit.
  This function takes as parameters the HAL peripheral handle, the Callback ID
  and a pointer to the user callback function.

  Use function @ref HAL_CRYP_UnRegisterCallback() to reset a callback to the default
  weak function.
  @ref HAL_CRYP_UnRegisterCallback() takes as parameters the HAL peripheral handle,
  and the Callback ID.
  This function allows to reset following callbacks:
    (+) InCpltCallback     :  Input FIFO transfer completed callback.
    (+) OutCpltCallback    : Output FIFO transfer completed callback.
    (+) ErrorCallback      : callback for error detection.
    (+) MspInitCallback    : CRYP MspInit.
    (+) MspDeInitCallback  : CRYP MspDeInit.

  By default, after the @ref HAL_CRYP_Init() and when the state is HAL_CRYP_STATE_RESET
  all callbacks are set to the corresponding weak functions :
  examples @ref HAL_CRYP_InCpltCallback() , @ref HAL_CRYP_OutCpltCallback().
  Exception done for MspInit and MspDeInit functions that are
  reset to the legacy weak function in the @ref HAL_CRYP_Init()/ @ref HAL_CRYP_DeInit() only when
  these callbacks are null (not registered beforehand).
  if not, MspInit or MspDeInit are not null, the @ref HAL_CRYP_Init() / @ref HAL_CRYP_DeInit()
  keep and use the user MspInit/MspDeInit functions (registered beforehand)

  Callbacks can be registered/unregistered in HAL_CRYP_STATE_READY state only.
  Exception done MspInit/MspDeInit callbacks that can be registered/unregistered
  in HAL_CRYP_STATE_READY or HAL_CRYP_STATE_RESET state,
  thus registered (user) MspInit/DeInit callbacks can be used during the Init/DeInit.
  In that case first register the MspInit/MspDeInit user callbacks
  using @ref HAL_CRYP_RegisterCallback() before calling @ref HAL_CRYP_DeInit()
  or @ref HAL_CRYP_Init() function.

  When The compilation define USE_HAL_CRYP_REGISTER_CALLBACKS is set to 0 or
  not defined, the callback registration feature is not available and all callbacks
  are set to the corresponding weak functions.

  Table 1. Initial Counter Block (ICB)
          +-------------------------------------------------------+
          |       Initialization vector (IV)      |  Counter      |
          |----------------|----------------|-----------|---------|
         127              95                63            31       0


              Bit Number    Register           Contents
              ----------   ---------------       -----------
              127 ...96    CRYP_IV1R[31:0]     ICB[127:96]
              95  ...64    CRYP_IV1L[31:0]     B0[95:64]
              63 ... 32    CRYP_IV0R[31:0]     ICB[63:32]
              31 ... 0     CRYP_IV0L[31:0]     ICB[31:0], where 32-bit counter= 0x2

  Table 2.  GCM last block definition

          +-------------------------------------------------------------------+
          |  Bit[0]   |  Bit[32]           |  Bit[64]  | Bit[96]              |
          |-----------|--------------------|-----------|----------------------|
          |   0x0     | Header length[31:0]|     0x0   | Payload length[31:0] |
          |-----------|--------------------|-----------|----------------------|

  Table 3. B0 block
                Octet Number   Contents
                ------------   ---------
                0              Flags
                1 ... 15-q     Nonce N
                16-q ... 15    Q

            the Flags field is formatted as follows:

                Bit Number   Contents
                ----------   ----------------------
                7            Reserved (always zero)
                6            Adata
                5 ... 3      (t-2)/2
                2 ... 0      [q-1]3

 Table 4. CTRx block
                Bit Number    Register           Contents
                ----------   ---------------       -----------
                127 ...96    CRYP_IV1R[31:0]     B0[127:96], where Q length bits are set to 0, except for
                                                 bit 0 that is set to 1
                95  ...64    CRYP_IV1L[31:0]     B0[95:64]
                63 ... 32    CRYP_IV0R[31:0]     B0[63:32]
                31 ... 0     CRYP_IV0L[31:0]     B0[31:0], where flag bits set to 0

  @endverbatim
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2016 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/** @addtogroup STM32F4xx_HAL_Driver
  * @{
  */

#if defined (AES)  || defined (CRYP)

/** @defgroup CRYP CRYP
  * @brief CRYP HAL module driver.
  * @{
  */


#ifdef HAL_CRYP_MODULE_ENABLED

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/** @addtogroup CRYP_Private_Defines
  * @{
  */
#define CRYP_TIMEOUT_KEYPREPARATION      82U         /*The latency of key preparation operation is 82 clock cycles.*/
#define CRYP_TIMEOUT_GCMCCMINITPHASE     299U        /*  The latency of  GCM/CCM init phase to prepare hash subkey is 299 clock cycles.*/
#define CRYP_TIMEOUT_GCMCCMHEADERPHASE   290U        /*  The latency of  GCM/CCM header phase is 290 clock cycles.*/

#define  CRYP_PHASE_READY                0x00000001U /*!< CRYP peripheral is ready for initialization. */
#define  CRYP_PHASE_PROCESS              0x00000002U /*!< CRYP peripheral is in processing phase */

#if defined(AES)
#define CRYP_OPERATINGMODE_ENCRYPT                   0x00000000U     /*!< Encryption mode(Mode 1)  */
#define CRYP_OPERATINGMODE_KEYDERIVATION             AES_CR_MODE_0   /*!< Key derivation mode  only used when performing ECB and CBC decryptions (Mode 2) */
#define CRYP_OPERATINGMODE_DECRYPT                   AES_CR_MODE_1   /*!< Decryption    (Mode 3)    */
#define CRYP_OPERATINGMODE_KEYDERIVATION_DECRYPT     AES_CR_MODE     /*!< Key derivation and decryption only used when performing ECB and CBC decryptions (Mode 4) */
#define CRYP_PHASE_INIT                              0x00000000U        /*!< GCM/GMAC (or CCM) init phase */
#define CRYP_PHASE_HEADER                            AES_CR_GCMPH_0     /*!< GCM/GMAC or CCM header phase */
#define CRYP_PHASE_PAYLOAD                           AES_CR_GCMPH_1     /*!< GCM(/CCM) payload phase      */
#define CRYP_PHASE_FINAL                             AES_CR_GCMPH       /*!< GCM/GMAC or CCM  final phase */
#else  /* CRYP */
#define CRYP_PHASE_INIT                              0x00000000U             /*!< GCM/GMAC (or CCM) init phase */
#define CRYP_PHASE_HEADER                            CRYP_CR_GCM_CCMPH_0     /*!< GCM/GMAC or CCM header phase */
#define CRYP_PHASE_PAYLOAD                           CRYP_CR_GCM_CCMPH_1     /*!< GCM(/CCM) payload phase      */
#define CRYP_PHASE_FINAL                             CRYP_CR_GCM_CCMPH       /*!< GCM/GMAC or CCM  final phase */
#define CRYP_OPERATINGMODE_ENCRYPT                   0x00000000U             /*!< Encryption mode   */
#define CRYP_OPERATINGMODE_DECRYPT                   CRYP_CR_ALGODIR         /*!< Decryption        */
#endif /* End CRYP or  AES */

 /*  CTR1 information to use in CCM algorithm */
#define CRYP_CCM_CTR1_0                  0x07FFFFFFU
#define CRYP_CCM_CTR1_1                  0xFFFFFF00U
#define CRYP_CCM_CTR1_2                  0x00000001U


/**
  * @}
  */


/* Private macro -------------------------------------------------------------*/
/** @addtogroup CRYP_Private_Macros
  * @{
  */

#if defined(CRYP)

#define CRYP_SET_PHASE(__HANDLE__, __PHASE__)  do{(__HANDLE__)->Instance->CR &= (uint32_t)(~CRYP_CR_GCM_CCMPH);\
                                                        (__HANDLE__)->Instance->CR |= (uint32_t)(__PHASE__);\
                                                       }while(0)

#define HAL_CRYP_FIFO_FLUSH(__HANDLE__) ((__HANDLE__)->Instance->CR |=  CRYP_CR_FFLUSH)

#else /*AES*/
#define CRYP_SET_PHASE(__HANDLE__, __PHASE__)  do{(__HANDLE__)->Instance->CR &= (uint32_t)(~AES_CR_GCMPH);\
                                                        (__HANDLE__)->Instance->CR |= (uint32_t)(__PHASE__);\
                                                       }while(0)
#endif /* End AES or CRYP*/


/**
  * @}
  */

/* Private struct -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/** @addtogroup CRYP_Private_Functions_prototypes
  * @{
  */

static void CRYP_SetDMAConfig(CRYP_HandleTypeDef *hcryp, uint32_t inputaddr, uint16_t Size, uint32_t outputaddr);
static void CRYP_DMAInCplt(DMA_HandleTypeDef *hdma);
static void CRYP_DMAOutCplt(DMA_HandleTypeDef *hdma);
static void CRYP_DMAError(DMA_HandleTypeDef *hdma);
static void CRYP_SetKey(CRYP_HandleTypeDef *hcryp, uint32_t KeySize);
static void CRYP_AES_IT(CRYP_HandleTypeDef *hcryp);
#if defined (CRYP_CR_ALGOMODE_AES_GCM)|| defined (AES)
static HAL_StatusTypeDef CRYP_GCMCCM_SetHeaderPhase(CRYP_HandleTypeDef *hcryp, uint32_t Timeout);
static void CRYP_GCMCCM_SetPayloadPhase_IT(CRYP_HandleTypeDef *hcryp);
static void CRYP_GCMCCM_SetHeaderPhase_IT(CRYP_HandleTypeDef *hcryp);
static HAL_StatusTypeDef CRYP_GCMCCM_SetHeaderPhase_DMA(CRYP_HandleTypeDef *hcryp);
static void CRYP_Workaround(CRYP_HandleTypeDef *hcryp, uint32_t Timeout);
static HAL_StatusTypeDef CRYP_AESGCM_Process_DMA(CRYP_HandleTypeDef *hcryp);
static HAL_StatusTypeDef CRYP_AESGCM_Process_IT (CRYP_HandleTypeDef *hcryp);
static HAL_StatusTypeDef CRYP_AESGCM_Process(CRYP_HandleTypeDef *hcryp, uint32_t Timeout);
static HAL_StatusTypeDef CRYP_AESCCM_Process(CRYP_HandleTypeDef *hcryp, uint32_t Timeout);
static HAL_StatusTypeDef CRYP_AESCCM_Process_IT(CRYP_HandleTypeDef *hcryp);
static HAL_StatusTypeDef CRYP_AESCCM_Process_DMA(CRYP_HandleTypeDef *hcryp);
#endif /* AES or GCM CCM defined*/
static void CRYP_AES_ProcessData(CRYP_HandleTypeDef *hcrypt, uint32_t Timeout);
static HAL_StatusTypeDef CRYP_AES_Encrypt(CRYP_HandleTypeDef *hcryp, uint32_t Timeout);
static HAL_StatusTypeDef CRYP_AES_Decrypt(CRYP_HandleTypeDef *hcryp, uint32_t Timeout);
static HAL_StatusTypeDef CRYP_AES_Decrypt_IT(CRYP_HandleTypeDef *hcryp);
static HAL_StatusTypeDef CRYP_AES_Encrypt_IT(CRYP_HandleTypeDef *hcryp);
static HAL_StatusTypeDef CRYP_AES_Decrypt_DMA(CRYP_HandleTypeDef *hcryp);
#if defined (CRYP)
static void CRYP_TDES_IT(CRYP_HandleTypeDef *hcryp);
#if defined (CRYP_CR_ALGOMODE_AES_GCM)
static HAL_StatusTypeDef CRYP_WaitOnIFEMFlag(CRYP_HandleTypeDef *hcryp, uint32_t Timeout);
#endif /* GCM CCM defined*/
static HAL_StatusTypeDef CRYP_WaitOnBUSYFlag(CRYP_HandleTypeDef *hcryp, uint32_t Timeout);
static HAL_StatusTypeDef CRYP_WaitOnOFNEFlag(CRYP_HandleTypeDef *hcryp, uint32_t Timeout);
static HAL_StatusTypeDef CRYP_TDES_Process(CRYP_HandleTypeDef *hcryp, uint32_t Timeout);
#else /*AES*/
static HAL_StatusTypeDef CRYP_WaitOnCCFlag(CRYP_HandleTypeDef *hcryp, uint32_t Timeout);
#endif /* End CRYP or AES */

/**
  * @}
  */

/* Exported functions ---------------------------------------------------------*/

/** @defgroup CRYP_Exported_Functions CRYP Exported Functions
  * @{
  */


/** @defgroup CRYP_Exported_Functions_Group1 Initialization and de-initialization functions
 *  @brief    Initialization and Configuration functions.
 *
@verbatim
  ========================================================================================
     ##### Initialization, de-initialization and Set and Get configuration functions #####
  ========================================================================================
    [..]  This section provides functions allowing to:
      (+) Initialize the CRYP
      (+) DeInitialize the CRYP
      (+) Initialize the CRYP MSP
      (+) DeInitialize the CRYP MSP
      (+) configure CRYP (HAL_CRYP_SetConfig) with the specified parameters in the CRYP_ConfigTypeDef
          Parameters which are configured in This section are :
          (+) Key size
          (+) Data Type : 32,16, 8 or 1bit
          (+) AlgoMode :
              - for CRYP1 IP :
                 ECB and CBC in DES/TDES Standard
                 ECB,CBC,CTR,GCM/GMAC and CCM in AES Standard.
              - for TinyAES2 IP, only ECB,CBC,CTR,GCM/GMAC and CCM in AES Standard are supported.
      (+) Get CRYP configuration (HAL_CRYP_GetConfig) from the specified parameters in the CRYP_HandleTypeDef


@endverbatim
  * @{
  */


/**
  * @brief  Initializes the CRYP according to the specified
  *         parameters in the CRYP_ConfigTypeDef and creates the associated handle.
  * @param  hcryp: pointer to a CRYP_HandleTypeDef structure that contains
  *         the configuration information for CRYP module
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_CRYP_Init(CRYP_HandleTypeDef *hcryp)
{
  /* Check the CRYP handle allocation */
  if(hcryp == NULL)
  {
    return HAL_ERROR;
  }

  /* Check parameters */
  assert_param(IS_CRYP_KEYSIZE(hcryp->Init.KeySize));
  assert_param(IS_CRYP_DATATYPE(hcryp->Init.DataType));
  assert_param(IS_CRYP_ALGORITHM(hcryp->Init.Algorithm));

  #if (USE_HAL_CRYP_REGISTER_CALLBACKS == 1)
  if(hcryp->State == HAL_CRYP_STATE_RESET)
  {
    /* Allocate lock resource and initialize it */
    hcryp->Lock = HAL_UNLOCKED;

    hcryp->InCpltCallback  = HAL_CRYP_InCpltCallback;  /* Legacy weak InCpltCallback   */
    hcryp->OutCpltCallback = HAL_CRYP_OutCpltCallback; /* Legacy weak OutCpltCallback  */
    hcryp->ErrorCallback   = HAL_CRYP_ErrorCallback;   /* Legacy weak ErrorCallback    */

    if(hcryp->MspInitCallback == NULL)
    {
      hcryp->MspInitCallback = HAL_CRYP_MspInit; /* Legacy weak MspInit  */
    }

    /* Init the low level hardware */
    hcryp->MspInitCallback(hcryp);
  }
#else
  if(hcryp->State == HAL_CRYP_STATE_RESET)
  {
    /* Allocate lock resource and initialize it */
    hcryp->Lock = HAL_UNLOCKED;

    /* Init the low level hardware */
    HAL_CRYP_MspInit(hcryp);
  }
 #endif /* (USE_HAL_CRYP_REGISTER_CALLBACKS) */

