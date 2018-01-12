/* ###################################################################
**     This component module is generated by Processor Expert. Do not modify it.
**     Filename    : Utility.c
**     Project     : Reader-Lib-HAL-PE
**     Processor   : MC9S12ZVL32MLC
**     Component   : Utility
**     Version     : Component 01.111, Driver 01.00, CPU db: 3.00.000
**     Compiler    : CodeWarrior HCS12Z C Compiler
**     Date/Time   : 2016-02-15, 16:16, # CodeGen: 59
**     Abstract    :
**          Contains various utility functions.
**     Settings    :
**          Component name                                 : Utility
**     Contents    :
**         strcpy         - void Utility_strcpy(uint8_t *dst, size_t dstSize, const unsigned char *src);
**         strcat         - void Utility_strcat(uint8_t *dst, size_t dstSize, const unsigned char *src);
**         strcatPad      - void Utility_strcatPad(uint8_t *dst, size_t dstSize, const unsigned char...
**         chcat          - void Utility_chcat(uint8_t *dst, size_t dstSize, uint8_t ch);
**         Num16uToStr    - void Utility_Num16uToStr(uint8_t *dst, size_t dstSize, uint16_t val);
**         strcatNum16u   - void Utility_strcatNum16u(uint8_t *dst, size_t dstSize, uint16_t val);
**         strcatNum8Hex  - void Utility_strcatNum8Hex(uint8_t *dst, size_t dstSize, uint8_t num);
**         strcatNum16Hex - void Utility_strcatNum16Hex(uint8_t *dst, size_t dstSize, uint16_t num);
**
**     License   :  Open Source (LGPL)
**     Copyright : (c) Copyright Erich Styger, 2014-2015, all rights reserved.
**     xatoi(): Copyright (C) 2010, ChaN, all right reserved. (see copyright notice and license at the function implementation).
**     This an open source software implementing utility functions using Processor Expert.
**     This is a free software and is opened for education,  research and commercial developments under license policy of following terms:
**     * This is a free software and there is NO WARRANTY.
**     * No restriction on use. You can use, modify and redistribute it for
**       personal, non-profit or commercial product UNDER YOUR RESPONSIBILITY.
**     * Redistributions of source code must retain the above copyright notice.
** ###################################################################*/
/*!
** @file Utility.c
** @version 01.00
** @brief
**          Contains various utility functions.
*/         
/*!
**  @addtogroup Utility_module Utility module documentation
**  @{
*/         

/* MODULE Utility. */

#include "Utility.h"

/* Internal method prototypes */
static void ShiftRightAndFill(uint8_t *dst, uint8_t fill, uint8_t nofFill);

/*
** ===================================================================
**     Method      :  Utility_strcpy (component Utility)
**     Description :
**         Same as normal strcpy, but safe as it does not write beyond
**         the buffer.
**     Parameters  :
**         NAME            - DESCRIPTION
**       * dst             - Pointer to destination string
**         dstSize         - Size of the destination buffer (in
**                           bytes).
**       * src             - Pointer to source string.
**     Returns     : Nothing
** ===================================================================
*/
/*!
  \brief copy the string src into dst. It performs the same task as strncpy, except
     - always terminates the result string.
     - does not zero out the remaining part in dst.
     Note: dstSize is the size of dst INCLUDING zero uint8_t.
     Precondition: src, dst != NULL
  \param[in,out] dst Start of buffer, where to append the number string
  \param[in] dstSize The size of the buffer, including the zero uint8_t
  \param[in] src The source string to copy
*/
void Utility_strcpy(uint8_t *dst, size_t dstSize, const unsigned char *src)
{
  dstSize--; /* for zero uint8_t */
  while (dstSize > 0 && *src != '\0') {
    *dst++ = *src++;
    dstSize--;
  }
  *dst = '\0';
}

/*
** ===================================================================
**     Method      :  Utility_strcat (component Utility)
**     Description :
**         Same as normal strcat, but safe as it does not write beyond
**         the buffer.
**     Parameters  :
**         NAME            - DESCRIPTION
**       * dst             - Pointer to destination string
**         dstSize         - Size of the destination buffer (in
**                           bytes).
**       * src             - Pointer to source string.
**     Returns     : Nothing
** ===================================================================
*/
/*!
  \brief Concat the string src into dst. Always terminates the result string.
     Note: count is the size of dst INCLUDING zero uint8_t.
     Precondition: src, dst != NULL
  \param[in,out] dst Start of buffer, where to append the number string
  \param[in] dstSize The size of the buffer, including the zero uint8_t
  \param[in] src The source string to add
  */
void Utility_strcat(uint8_t *dst, size_t dstSize, const unsigned char *src)
{
  dstSize--; /* for zero uint8_t */
  /* point to the end of the source */
  while (dstSize > 0 && *dst != '\0') {
    dst++;
    dstSize--;
  }
  /* copy the src in the destination */
  while (dstSize > 0 && *src != '\0') {
    *dst++ = *src++;
    dstSize--;
  }
  /* terminate the string */
  *dst = '\0';
}

/*
** ===================================================================
**     Method      :  Utility_chcat (component Utility)
**     Description :
**         Adds a single character to a zero byte terminated buffer. It
**         cares about buffer overflow.
**     Parameters  :
**         NAME            - DESCRIPTION
**       * dst             - Pointer to destination string
**         dstSize         - Size of the destination buffer (in
**                           bytes).
**         ch              - character to append
**     Returns     : Nothing
** ===================================================================
*/
void Utility_chcat(uint8_t *dst, size_t dstSize, uint8_t ch)
{
  dstSize--; /* for zero uint8_t */
  /* point to the end of the source */
  while (dstSize > 0 && *dst != '\0') {
    dst++;
    dstSize--;
  }
  /* copy the ch in the destination */
  if (dstSize > 0) {
    *dst++ = ch;
  }
  /* terminate the string */
  *dst = '\0';
}

/*
** ===================================================================
**     Method      :  Utility_Num16uToStr (component Utility)
**     Description :
**         Converts a signed 16bit value into a string.
**     Parameters  :
**         NAME            - DESCRIPTION
**       * dst             - Pointer to destination string
**         dstSize         - Size of the destination buffer (in
**                           bytes).
**         val             - Value to be converted.
**     Returns     : Nothing
** ===================================================================
*/
/*!
  \brief Converts a 16bit unsigned number into a string.
  \param[in,out] dst String buffer to store the number.
  \param[in] dstSize Size of the destination buffer in uint8_ts.
  \param[in] val 16bit unsigned number to convert.
 */
void Utility_Num16uToStr(uint8_t *dst, size_t dstSize, uint16_t val)
{
  unsigned char *ptr = ((unsigned char *)dst);
  unsigned char i=0, j;
  unsigned char tmp;

  dstSize--; /* for zero uint8_t */
  if (val == 0 && dstSize > 0){
    ptr[i++] = '0';
    dstSize--;
  }
  while (val > 0 && dstSize > 0) {
    ptr[i++] = (unsigned char)((val % 10) + '0');
    dstSize--;
    val /= 10;
  }
  for(j=0; j<(i/2); j++) { /* swap buffer */
    tmp = ptr[j];
    ptr[j] = ptr[(i-j)-1];
    ptr[(i-j)-1] = tmp;
  }
  ptr[i] = '\0';
}

/*
** ===================================================================
**     Method      :  ShiftRightAndFill (component Utility)
**
**     Description :
**         Moves the content of a string to the right and fills the space 
**         on the left side with a pattern
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
static void ShiftRightAndFill(uint8_t *dst, uint8_t fill, uint8_t nofFill)
{
  signed char i, j;

  j = 0;
  while(dst[j] != '\0') {
    j++;
  }
  i = (signed char)nofFill;
  if (i==j) {
    /* nothing to do, we are done */
  } else if (i>j) {
    while (j>=0) {
      dst[i] = dst[j];
      i--; j--;
    }
    while(i>=0) {
      dst[i] = fill;
      i--;
    }
  } else {
    /* hmmm, not enough space, return what we have, do nothing */
  }
}

/*
** ===================================================================
**     Method      :  Utility_strcatNum16u (component Utility)
**     Description :
**         Appends a 16bit unsigned value to a string buffer.
**     Parameters  :
**         NAME            - DESCRIPTION
**       * dst             - Pointer to destination string
**         dstSize         - Size of the destination buffer (in
**                           bytes).
**         val             - Value to convert.
**     Returns     : Nothing
** ===================================================================
*/
/*!
  \brief appends a 16bit unsigned number to a string. Always terminates the result string.
     Note: count is the size of dst INCLUDING zero uint8_t.
     Precondition: src, dst != NULL
  \param[in,out] dst Start of buffer, where to append the number string
  \param[in] dstSize The size of the buffer, including the zero uint8_t
  \param[in] val The 16bit unsigned number to add
  */
void Utility_strcatNum16u(uint8_t *dst, size_t dstSize, uint16_t val)
{
  unsigned char buf[sizeof("32768")]; /* maximum buffer size we need */

  Utility_Num16uToStr(buf, sizeof(buf), val);
  Utility_strcat(dst, dstSize, buf);
}

/*
** ===================================================================
**     Method      :  Utility_strcatNum8Hex (component Utility)
**     Description :
**         Appends a 8bit unsigned value to a buffer as hex number
**         (without a 0x prefix).
**     Parameters  :
**         NAME            - DESCRIPTION
**       * dst             - Pointer to destination string
**         dstSize         - Size of the destination buffer (in
**                           bytes).
**         num             - Value to convert.
**     Returns     : Nothing
** ===================================================================
*/
/*!
  \brief Adds a 8bit number as hex value to a string.
  \param[in,out] dst Start of buffer, where to append the number string
  \param[in] dstSize The size of the buffer, including the zero uint8_t
  \param[in] num The 8bit number to add
  */
void Utility_strcatNum8Hex(uint8_t *dst, size_t dstSize, uint8_t num)
{
  unsigned char buf[sizeof("FF")]; /* maximum buffer size we need */
  unsigned char hex;

  buf[2] = '\0';
  hex = (char)(num & 0x0F);
  buf[1] = (char)(hex + ((hex <= 9) ? '0' : ('A'-10)));
  hex = (char)((num>>4) & 0x0F);
  buf[0] = (char)(hex + ((hex <= 9) ? '0' : ('A'-10)));
  Utility_strcat(dst, dstSize, buf);
}

/*
** ===================================================================
**     Method      :  Utility_strcatNum16Hex (component Utility)
**     Description :
**         Appends a 16bit unsigned value to a buffer as hex number
**         (without a 0x prefix).
**     Parameters  :
**         NAME            - DESCRIPTION
**       * dst             - Pointer to destination string
**         dstSize         - Size of the destination buffer (in
**                           bytes).
**         num             - Value to convert.
**     Returns     : Nothing
** ===================================================================
*/
/*!
  \brief Adds a 16bit number as hex value to a string.
  \param[in,out] dst Start of buffer, where to append the number string
  \param[in] dstSize The size of the buffer, including the zero uint8_t
  \param[in] num The 16bit number to add
  */
void Utility_strcatNum16Hex(uint8_t *dst, size_t dstSize, uint16_t num)
{
  unsigned char buf[sizeof("FFFF")]; /* maximum buffer size we need */
  unsigned char hex;
  int8_t i;

  buf[4] = '\0';
  i = 3;
  do {
    hex = (char)(num & 0x0F);
    buf[i] = (char)(hex + ((hex <= 9) ? '0' : ('A'-10)));
    num >>= 4;                          /* next nibble */
    i--;
  } while (i>=0);
  Utility_strcat(dst, dstSize, buf);
}

/*
** ===================================================================
**     Method      :  Utility_strcatPad (component Utility)
**     Description :
**         Same as normal strcat, but safe as it does not write beyond
**         the buffer. The buffer will be filled with a pad character
**         for a given length.
**     Parameters  :
**         NAME            - DESCRIPTION
**       * dst             - Pointer to destination string
**         dstSize         - Size of the destination buffer (in
**                           bytes).
**       * src             - Pointer to source string.
**         padChar         - Character to be used for padding
**         srcPadSize      - To which size the src string
**                           has to be padded.
**     Returns     : Nothing
** ===================================================================
*/
void Utility_strcatPad(uint8_t *dst, size_t dstSize, const unsigned char *src, char padChar, uint8_t srcPadSize)
{
  uint8_t *p;
  size_t nof = 0;

  if (dstSize<2) {
    return; /* hmm, really to small for anything than the zero byte? */
  }
  p = dst;
  while(*p != '\0') { /* find end of string */
    p++;
    nof++;
  }
  Utility_strcat(dst+nof, dstSize-nof, src); /* add string */
  dstSize -= nof;
  while(*p != '\0' && srcPadSize>0 && dstSize>1) {
    p++;
    srcPadSize--;
    dstSize--;
  }
  while(srcPadSize>0 && dstSize>1) {
    *p++ = padChar; /* add padding char */
    srcPadSize--;
    dstSize--;
  }
  *p = '\0'; /* terminate string */
}

/* END Utility. */

/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.3 [05.09]
**     for the Freescale HCS12Z series of microcontrollers.
**
** ###################################################################
*/