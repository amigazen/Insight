/*
 * Error Codes Database for SomeIdea
 * 
 * This file contains the comprehensive database of Guru Meditation error codes
 * compiled from various Amiga documentation sources.
 *
 * After meditiation comes... Insight
 *
 * Copyright (c) 2025 amigazen project
 * Licensed under BSD 2-Clause License
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
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