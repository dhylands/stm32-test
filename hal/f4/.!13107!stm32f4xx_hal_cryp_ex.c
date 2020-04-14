/**
  ******************************************************************************
  * @file    stm32f4xx_hal_cryp_ex.c
  * @author  MCD Application Team
  * @brief   Extended CRYP HAL module driver
  *          This file provides firmware functions to manage the following
  *          functionalities of CRYP extension peripheral:
  *           + Extended AES processing functions
  *
  @verbatim
  ==============================================================================
                     ##### How to use this driver #####
  ==============================================================================
    [..]
    The CRYP extension HAL driver can be used as follows:
    (#)After AES-GCM or AES-CCM  Encryption/Decryption user can start following API
       to get the  authentication messages :
      (##) HAL_CRYPEx_AESGCM_GenerateAuthTAG
      (##) HAL_CRYPEx_AESCCM_GenerateAuthTAG

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
#if defined (CRYP_CR_ALGOMODE_AES_GCM)|| defined (AES)
/** @defgroup CRYPEx CRYPEx
  * @brief CRYP Extension HAL module driver.
  * @{
  */


#ifdef HAL_CRYP_MODULE_ENABLED

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/** @addtogroup CRYPEx_Private_Defines
  * @{
  */
#if defined(AES)
#define CRYP_PHASE_INIT                              0x00000000U             /*!< GCM/GMAC (or CCM) init phase */
#define CRYP_PHASE_HEADER                            AES_CR_GCMPH_0          /*!< GCM/GMAC or CCM header phase */
#define CRYP_PHASE_PAYLOAD                           AES_CR_GCMPH_1          /*!< GCM(/CCM) payload phase   */
#define CRYP_PHASE_FINAL                             AES_CR_GCMPH            /*!< GCM/GMAC or CCM  final phase  */

#define CRYP_OPERATINGMODE_ENCRYPT                   0x00000000U             /*!< Encryption mode   */
#define CRYP_OPERATINGMODE_KEYDERIVATION             AES_CR_MODE_0           /*!< Key derivation mode  only used when performing ECB and CBC decryptions  */
#define CRYP_OPERATINGMODE_DECRYPT                   AES_CR_MODE_1           /*!< Decryption       */
#define CRYP_OPERATINGMODE_KEYDERIVATION_DECRYPT     AES_CR_MODE             /*!< Key derivation and decryption only used when performing ECB and CBC decryptions  */

#else /* CRYP */

#define CRYP_PHASE_INIT                 0x00000000U
#define CRYP_PHASE_HEADER               CRYP_CR_GCM_CCMPH_0
#define CRYP_PHASE_PAYLOAD              CRYP_CR_GCM_CCMPH_1
#define CRYP_PHASE_FINAL                CRYP_CR_GCM_CCMPH

#define CRYP_OPERATINGMODE_ENCRYPT      0x00000000U
#define CRYP_OPERATINGMODE_DECRYPT      CRYP_CR_ALGODIR
#endif /* End AES or CRYP */

#define  CRYPEx_PHASE_PROCESS       0x02U     /*!< CRYP peripheral is in processing phase */
#define  CRYPEx_PHASE_FINAL         0x03U     /*!< CRYP peripheral is in final phase this is relevant only with CCM and GCM modes */

 /*  CTR0 information to use in CCM algorithm */
#define CRYP_CCM_CTR0_0            0x07FFFFFFU
#define CRYP_CCM_CTR0_3            0xFFFFFF00U


/**
  * @}
  */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/



/* Exported functions---------------------------------------------------------*/
/** @addtogroup CRYPEx_Exported_Functions
  * @{
  */

/** @defgroup CRYPEx_Exported_Functions_Group1 Extended AES processing functions
 *  @brief   Extended processing functions.
 *
@verbatim
  ==============================================================================
              ##### Extended AES processing functions #####
  ==============================================================================
    [..]  This section provides functions allowing to generate the authentication
          TAG in Polling mode
      (#)HAL_CRYPEx_AESGCM_GenerateAuthTAG
      (#)HAL_CRYPEx_AESCCM_GenerateAuthTAG
         they should be used after Encrypt/Decrypt operation.

@endverbatim
  * @{
  */


/**
  * @brief  generate the GCM authentication TAG.
  * @param  hcryp: pointer to a CRYP_HandleTypeDef structure that contains
  *         the configuration information for CRYP module
  * @param  AuthTag: Pointer to the authentication buffer
  * @param  Timeout: Timeout duration
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_CRYPEx_AESGCM_GenerateAuthTAG(CRYP_HandleTypeDef *hcryp, uint32_t *AuthTag, uint32_t Timeout)
{
  uint32_t tickstart;
  uint64_t headerlength = (uint64_t)(hcryp->Init.HeaderSize) * 32U; /* Header length in bits */
  uint64_t inputlength = (uint64_t)(hcryp->Size) * 8U; /* input length in bits */
  uint32_t tagaddr = (uint32_t)AuthTag;

  if(hcryp->State == HAL_CRYP_STATE_READY)
  {
    /* Process locked */
    __HAL_LOCK(hcryp);

    /* Change the CRYP peripheral state */
    hcryp->State = HAL_CRYP_STATE_BUSY;

    /* Check if initialization phase has already been performed */
    if(hcryp->Phase == CRYPEx_PHASE_PROCESS)
    {
      /* Change the CRYP phase */
      hcryp->Phase = CRYPEx_PHASE_FINAL;
    }
    else /* Initialization phase has not been performed*/
    {
      /* Disable the Peripheral */
      __HAL_CRYP_DISABLE(hcryp);

      /* Sequence error code field */
      hcryp->ErrorCode |= HAL_CRYP_ERROR_AUTH_TAG_SEQUENCE;

      /* Change the CRYP peripheral state */
      hcryp->State = HAL_CRYP_STATE_READY;

      /* Process unlocked */
      __HAL_UNLOCK(hcryp);
      return HAL_ERROR;
    }

#if defined(CRYP)

    /* Disable CRYP to start the final phase */
    __HAL_CRYP_DISABLE(hcryp);

    /* Select final phase */
    MODIFY_REG(hcryp->Instance->CR, CRYP_CR_GCM_CCMPH, CRYP_PHASE_FINAL);

