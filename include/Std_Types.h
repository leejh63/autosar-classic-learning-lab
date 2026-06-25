#ifndef STD_TYPES_H
#define STD_TYPES_H

#include <stdint.h>

typedef uint8_t boolean;
typedef uint8_t Std_ReturnType;

#ifndef TRUE
#define TRUE  ((boolean)1u)
#endif

#ifndef FALSE
#define FALSE ((boolean)0u)
#endif

#ifndef E_OK
#define E_OK     ((Std_ReturnType)0u)
#endif

#ifndef E_NOT_OK
#define E_NOT_OK ((Std_ReturnType)1u)
#endif

#endif /* STD_TYPES_H */
