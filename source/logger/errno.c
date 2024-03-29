/**
  * File Name 		- errno.c
  * Description 	- contains error enums, and related functions
  * Author			- Atharva Nandanwar
  * Tools			- GNU C Compiler / ARM Compiler Toolchain
  * Leveraged Code 	- https://android.googlesource.com/kernel/lk/+/upstream-master/include/errno.h
  * URL				-
  */

#include "errno.h"

/**
 * Get_Error_Message
 * returns with error message for particular errors
 * @param
 * 		error - error code
 * @return
 * 		returns error message
 */
const char* Get_Error_Message(error_t error)
{
	switch(error)
	{
	case eUART_Parity_Error:
		return "Parity Error";
		break;
	case eUART_Noise_Error:
		return "Noise Error";
		break;
	case eUART_Framing_Error:
		return "Framing Error";
		break;
	case eUART_Overrun_Error:
		return "Overrun Error";
		break;
	default:
		return "";
		break;
	}
}
