#ifndef USER_TA_HEADER_DEFINES_H
#define USER_TA_HEADER_DEFINES_H

/*
 * To get the TA_SHA1_UUID define
 */
#include <sha1_ta.h>

#define TA_UUID         TA_SHA1_UUID

/*
 * TA properties: multi-instance TA, no specific attribute
 * TA_FLAG_EXEC_DDR is meaningless but mandated.
 */
#define TA_FLAGS        TA_FLAG_EXEC_DDR

/*
 * Provisioned stack size
 */
#define TA_STACK_SIZE   (2 * 1024)

/*
 * Provisioned head size for TEE_Malloc() and friends
 */
#define TA_DATA_SIZE    (32 * 1024)

/* 
 * Extra properties (give a version id and a string name) 
 */
#define TA_CURRENT_TA_EXT_PROPERTIES \
    { "gp.ta.description",  USER_TA_PROP_TYPE_STRING,   "Example of TA using SHA1 calculation" }, \
    { "gp.ta.version",      USER_TA_PROP_TYPE_U32,      &(const uint32_t){ 0x0010 }}

#endif /* USER_TA_HEADER_DEFINES_H */