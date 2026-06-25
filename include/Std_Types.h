#ifndef STD_TYPES_H
#define STD_TYPES_H

#include <stdint.h>

/* 학습용 최소 AUTOSAR-like 타입입니다. */
typedef uint8_t boolean;
typedef uint8_t Std_ReturnType;

#ifndef TRUE
#define TRUE  (1u)
#endif

#ifndef FALSE
#define FALSE (0u)
#endif

#define E_OK     (0u)
#define E_NOT_OK (1u)

#endif /* STD_TYPES_H */
