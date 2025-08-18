/*
 * Error Codes Database for SomeIdea
 * 
 * This file contains the comprehensive database of Guru Meditation error codes
 * compiled from various Amiga documentation sources.
 *
 * Copyright (c) 2025 amigazen project
 * Licensed under BSD 2-Clause License
 */

#ifndef ERROR_CODES_H
#define ERROR_CODES_H

#include <exec/types.h>

/* Structure to hold error code information */
struct ErrorInfo {
    ULONG code;
    STRPTR description;
    STRPTR insight;
};

/* Comprehensive error lookup table */
extern struct ErrorInfo errorTable[];

/* Number of entries in the error table */
extern const ULONG ERROR_TABLE_SIZE;

/* Function declarations */
struct ErrorInfo* GainInsight(ULONG errorCode);
STRPTR ExpandExplanation(STRPTR compressedExplanation);
VOID FreeErrorInfo(struct ErrorInfo *errorInfo);
STRPTR LookupToken(const char *tokenName);

#endif /* ERROR_CODES_H */ 