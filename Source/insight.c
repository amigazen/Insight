/*
 * After meditiation comes... Insight
 *
 * Copyright (c) 2025-26 amigazen project
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

#include <exec/types.h>
#include <exec/execbase.h>
#include <exec/alerts.h>
#include <dos/dos.h>
#include <intuition/intuition.h>
#include <intuition/intuitionbase.h>
#include <intuition/classusr.h>
#include <classes/requester.h>
#include <reaction/reaction_macros.h>
#include <utility/tagitem.h>
#include <proto/exec.h>
#include <clib/alib_protos.h>
#include <intuition/classes.h>
#include <proto/intuition.h>
#include <proto/dos.h>
#include <proto/requester.h>
#include <proto/utility.h>
#include <proto/graphics.h>
#include <dos/rdargs.h>
#include <workbench/startup.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "error_codes.h"

/* External error table reference */
extern struct ErrorInfo errorTable[];
extern const ULONG ERROR_TABLE_SIZE;

/* Forward declarations */
VOID ShowErrorDialog(ULONG errorCode, STRPTR description, STRPTR explanation, ULONG taskID);
BOOL InitializeLibraries(VOID);
VOID Cleanup(VOID);
BOOL InitializeApplication(VOID);
STRPTR WordWrapText(STRPTR text, ULONG maxWidth, struct Screen *screen);
VOID SafeExit(int status);  /* Safe exit with cleanup */
ULONG HexStringToULong(STRPTR hexString);  /* Convert hex string to ULONG */
ULONG ParseLastAlert(ULONG *taskID);  /* Parse LastAlert array and return error code */
BOOL LooksLikeHexNumber(STRPTR str);  /* Check if string looks like a hex number */
BOOL ValidateHexErrorCode(STRPTR hexString);  /* Validate that string is exactly 8 hex digits */


/* Library base pointers */
extern struct ExecBase *SysBase;
extern struct IntuitionBase *IntuitionBase;
extern struct Library *UtilityBase;
extern struct GfxBase *GfxBase;

/* Reaction class library bases */
struct ClassLibrary *RequesterBase = NULL;

/* Reaction class handles */
Class *RequesterClass = NULL;

static const char *verstag = "$VER: Insight 47.5 (1/2/2026)\n";
static const char *stack_cookie = "$STACK: 8192\n";
long oslibversion  = 47L; 

/* Application variables - none needed for this simple app */

/* Main entry point */
int main(int argc, char *argv[])
{
    struct ErrorInfo *errorInfo;
    ULONG guruCode;
    ULONG taskID;
    struct RDArgs *rdargs;
    BOOL testMode = FALSE;
    BOOL success = FALSE;
    BOOL fromWorkbench = FALSE;
    
    /* Check if running from Workbench */
    /* When Workbench starts a program, argc = 0 and argv[0] contains WBStartup message */
    /* According to Amiga documentation, the startup code handles the WBStartup message */
    /* We should NOT call ReplyMsg() ourselves - let the startup code handle it */
    fromWorkbench = (argc == 0);
    
    if (fromWorkbench) {
        /* Workbench mode: check SysBase LastAlert and show requester if error exists */
        /* Note: Do NOT access argv[0] or try to handle WBStartup message manually */
        /* The startup code will handle removing the message and replying to Workbench */
        
        /* Check SysBase LastAlert for errors */
        /* LastAlert[4] is an array containing the last 4 system alert codes */
        /* The 4 LONG values together form the complete error code */
        /* Note: We must be careful accessing SysBase fields - ensure system is stable */
        /* Validate that SysBase is accessible before accessing its fields */
        if (SysBase == NULL) {
            /* SysBase not available - exit cleanly */
            SafeExit(RETURN_FAIL);
        }
        
        /* Parse the LastAlert array to get the complete error code and task ID */
        guruCode = ParseLastAlert(&taskID);
        
        /* Check if there's actually an error code (not -1) */
        /* According to Amiga documentation, LastAlert[0] = -1 (0xFFFFFFFF) means no error */
        /* Additional safety check: ensure the value is reasonable (not corrupted) */
        if (guruCode == (ULONG)-1) {
            /* No error - LastAlert[0] = 0xFFFFFFFF means no error exists */
            SafeExit(RETURN_OK);
        } else if (guruCode == 0) {
            /* No error - LastAlert[0] = 0 means no error exists */
            SafeExit(RETURN_OK);
        } else {
            /* Error exists - parse the error code and show requester */
            if (!InitializeLibraries()) {
                SafeExit(RETURN_FAIL);
            }
            if (!InitializeApplication()) {
                Cleanup();
                SafeExit(RETURN_FAIL);
            }
            
            /* Parse the error code to get description and explanation */
            errorInfo = GainInsight(guruCode);
            if (errorInfo != NULL) {
                /* Found error in database - show with proper description */
                ShowErrorDialog(guruCode, errorInfo->description, errorInfo->insight, taskID);
                FreeErrorInfo(errorInfo);  /* Free allocated memory */
            } else {
                /* Error code not in database - show generic error dialog */
                ShowErrorDialog(guruCode, "Unknown Error", "This error is not recognized.", taskID);
            }
            
            Cleanup();
            
            SafeExit(RETURN_FAIL);  /* Exit with failure status since there was an error */
        }
    } else {
        /* Command line mode: parse arguments and handle accordingly */
        STRPTR args[2] = {NULL, NULL};  /* ERROR/K, GURU/S */
        STRPTR errorArg = NULL;
        int i;  /* C89 compliance - declare at start of block */
        int j;  /* C89 compliance - for implicit hex check */
        
        /* First, check for implicit hex numbers before ReadArgs processing */
        /* This handles cases like "insight 8000000B" where no ERROR keyword is used */
        for (j = 1; j < argc; j++) {
            if (LooksLikeHexNumber(argv[j])) {
                /* Found a hex number - copy to buffer */
                static char errorBufferImplicit[32];
                int k;
                
                memset(errorBufferImplicit, 0, sizeof(errorBufferImplicit));
                k = 0;
                while (k < sizeof(errorBufferImplicit) - 1 && argv[j][k] != '\0') {
                    errorBufferImplicit[k] = argv[j][k];
                    k++;
                }
                errorBufferImplicit[k] = '\0';
                errorArg = errorBufferImplicit;
                break;  /* Use first hex number found */
            }
        }
        
        /* Parse command line arguments */
        
        rdargs = ReadArgs("ERROR/K,GURU/S", (LONG *)args, NULL);
        if (rdargs != NULL) {
            /* Set flags */
            testMode = (args[1] != NULL);      /* GURU/S is a switch */
            
            /* Copy the string content before freeing the args */
            /* Only use ReadArgs result if we didn't already find an implicit hex number */
            if (args[0] != NULL && errorArg == NULL) {
                /* Use a local buffer to store the error argument */
                static char errorBuffer[32];  /* Buffer for error code (max 8 hex chars + 0x prefix + null) */
                
                /* Clear the buffer first */
                memset(errorBuffer, 0, sizeof(errorBuffer));
                
                /* Try to copy the string manually character by character */
                i = 0;
                while (i < sizeof(errorBuffer) - 1 && args[0][i] != '\0') {
                    errorBuffer[i] = args[0][i];
                    i++;
                }
                errorBuffer[i] = '\0';  /* Ensure null termination */
                
                errorArg = errorBuffer;
            }
            /* If errorArg is still NULL here, we'll check for implicit hex numbers below */
            
            FreeArgs(rdargs);
        } else {
            /* Try manual parsing as fallback */
            for (i = 1; i < argc; i++) {
                /* Manual string comparison for ERROR= */
                if (argv[i][0] == 'E' && argv[i][1] == 'R' && argv[i][2] == 'R' && 
                    argv[i][3] == 'O' && argv[i][4] == 'R' && argv[i][5] == '=') {
                    /* Handle ERROR=value format for compatibility */
                    errorArg = argv[i] + 6;
                    break;
                } else if (argv[i][0] == 'E' && argv[i][1] == 'R' && argv[i][2] == 'R' && 
                           argv[i][3] == 'O' && argv[i][4] == 'R' && argv[i][5] == '\0' && i + 1 < argc) {
                    /* Handle ERROR value format (space separated) */
                    errorArg = argv[i + 1];
                    break;
                } else if (argv[i][0] == 'G' && argv[i][1] == 'U' && argv[i][2] == 'R' && 
                           argv[i][3] == 'U' && argv[i][4] == '\0') {
                    testMode = TRUE;
                } else if (errorArg == NULL && LooksLikeHexNumber(argv[i])) {
                    /* Implicit ERROR: if argument looks like a hex number, treat it as error code */
                    /* Copy to buffer to avoid issues with string pointers */
                    static char errorBufferFallback[32];
                    int k;
                    
                    memset(errorBufferFallback, 0, sizeof(errorBufferFallback));
                    k = 0;
                    while (k < sizeof(errorBufferFallback) - 1 && argv[i][k] != '\0') {
                        errorBufferFallback[k] = argv[i][k];
                        k++;
                    }
                    errorBufferFallback[k] = '\0';
                    errorArg = errorBufferFallback;
                }
            }
        }
        
        /* If ReadArgs succeeded but didn't find ERROR, and we haven't found an implicit hex number yet, check again */
        /* (This handles the case where ReadArgs processed the arguments but didn't match ERROR keyword) */
        if (rdargs != NULL && errorArg == NULL) {
            for (j = 1; j < argc; j++) {
                if (LooksLikeHexNumber(argv[j])) {
                    /* Use a local buffer to store the error argument */
                    static char errorBufferReadArgs[32];  /* Buffer for error code (max 8 hex chars + 0x prefix + null) */
                    int k;  /* C89 compliance - character index */
                    
                    /* Clear the buffer first */
                    memset(errorBufferReadArgs, 0, sizeof(errorBufferReadArgs));
                    
                    /* Copy the string manually character by character */
                    k = 0;
                    while (k < sizeof(errorBufferReadArgs) - 1 && argv[j][k] != '\0') {
                        errorBufferReadArgs[k] = argv[j][k];
                        k++;
                    }
                    errorBufferReadArgs[k] = '\0';  /* Ensure null termination */
                    
                    errorArg = errorBufferReadArgs;
                    break;
                }
            }
        }
        
        /* If we have arguments but ReadArgs failed and we didn't find any valid parameters, exit */
        if (argc > 1 && errorArg == NULL && !testMode) {
            SafeExit(RETURN_OK);
        }
        
        /* Initialize random number generator for test mode */
        if (testMode) {
            srand((unsigned int)time(NULL));
        }
        
        /* Initialize libraries */
        if (!InitializeLibraries()) {
            SafeExit(RETURN_FAIL);
        }
        
        /* Initialize application */
        if (!InitializeApplication()) {
            Cleanup();
            SafeExit(RETURN_FAIL);
        }
        
        /* Determine what to do based on parameters */
        if (errorArg != NULL) {
            /* ERROR parameter specified - parse and show specific error */
            /* First validate that it's exactly 8 hex digits */
            if (!ValidateHexErrorCode(errorArg)) {
                Printf("Error: Invalid error code format. Error code must be exactly 8 hexadecimal digits.\n");
                Printf("Example: 8000000B or 0x8000000B\n");
                success = FALSE;
            } else {
                guruCode = HexStringToULong(errorArg);
                
                /* Check if parsing was successful - HexStringToULong returns (ULONG)-1 for invalid input */
                if (guruCode != (ULONG)-1) {
                    errorInfo = GainInsight(guruCode);
                    if (errorInfo != NULL) {
                        if (testMode) {
                            ShowErrorDialog(guruCode, errorInfo->description, errorInfo->insight, 0);
                        }
                        /* Always show error information when ERROR parameter is specified */
                        Printf("Error Code: 0x%08lX\nError: %s\n%s\n", 
                               guruCode, errorInfo->description, errorInfo->insight);
                        success = TRUE;  /* Successfully parsed and displayed error */
                        FreeErrorInfo(errorInfo);  /* Free allocated memory */
                    } else {
                        Printf("Unknown error code: 0x%08lX\n", guruCode);
                        success = FALSE;  /* Error code not found */
                    }
                } else {
                    Printf("Error: Failed to parse error code. Use format: 8000000B or 0x8000000B\n");
                    success = FALSE;  /* Invalid format */
                }
            }
        } else if (testMode) {
            /* GURU parameter - show random error */
            ULONG randomIndex = (ULONG)rand() % ERROR_TABLE_SIZE;
            guruCode = errorTable[randomIndex].code;
            
            /* Use GainInsight to get expanded explanation with token expansion */
            errorInfo = GainInsight(guruCode);
            if (errorInfo != NULL) {
                ShowErrorDialog(guruCode, errorInfo->description, errorInfo->insight, 0);
                success = TRUE;  /* Successfully showed random error */
                
                FreeErrorInfo(errorInfo);  /* Free allocated memory */
            } else {
                Printf("Failed to lookup error code 0x%08lX\n", guruCode);
                success = FALSE;
            }
        } else {
            /* No special parameters - check SysBase LastAlert */
            /* LastAlert[4] is an array containing the last 4 system alert codes */
            /* The 4 LONG values together form the complete error code */
            guruCode = ParseLastAlert(&taskID);
            
            /* Check if there's actually an error code (not -1) */
            /* According to Amiga documentation, LastAlert[0] = -1 (0xFFFFFFFF) means no error */
            if (guruCode == (ULONG)-1) {
                /* No error - this is normal, not a failure */
                success = TRUE;  /* Successfully determined no error exists */
            } else {
                /* Error exists - parse the error code and print to console */
                errorInfo = GainInsight(guruCode);
                if (errorInfo != NULL) {
                    Printf("Error Code: 0x%08lX\n\nTask ID: 0x%08lX\n\nError: %s\n\n%s\n", 
                           guruCode, taskID, errorInfo->description, errorInfo->insight);
                    FreeErrorInfo(errorInfo);  /* Free allocated memory */
                } else {
                    Printf("Error Code: 0x%08lX\n\nTask ID: 0x%08lX\n\nError: Unknown Error\n\nNo Insight for this error code.\n", 
                           guruCode, taskID);
                }
                success = FALSE;  /* Exit with failure status since there was an error */
            }
        }
        
        /* Clean up and exit */
        Cleanup();
        
        /* Exit with appropriate code */
        SafeExit(success ? RETURN_OK : RETURN_FAIL);
    }
}

/*
 * Word wrap text to fit within specified width
 */
STRPTR WordWrapText(STRPTR text, ULONG maxWidth, struct Screen *screen)
{
    static char wrappedText[1024];
    char *src = (char *)text;
    char *dst = wrappedText;
    char *wordStart = src;
    ULONG currentWidth = 0;
    ULONG wordWidth;
    
    /* Clear the output buffer */
    memset(wrappedText, 0, sizeof(wrappedText));
    
    /* Safety check - if screen is invalid, just return original text */
    if (screen == NULL || screen->Width <= 0 || screen->Height <= 0) {
        strcpy(wrappedText, text);
        return wrappedText;
    }
    
    while (*src && (dst - wrappedText) < sizeof(wrappedText) - 1) {
        if (*src == ' ' || *src == '\n' || *src == '\0') {
            /* End of word, calculate its width - use temporary buffer */
            char tempChar = *src;
            char tempWord[256];
            ULONG wordLen = src - wordStart;
            
            /* Copy word to temporary buffer safely */
            if (wordLen < sizeof(tempWord) - 1) {
                strncpy(tempWord, wordStart, wordLen);
                tempWord[wordLen] = '\0';
                wordWidth = TextLength(&screen->RastPort, tempWord, wordLen);
            } else {
                /* Word too long, truncate safely */
                strncpy(tempWord, wordStart, sizeof(tempWord) - 1);
                tempWord[sizeof(tempWord) - 1] = '\0';
                wordWidth = TextLength(&screen->RastPort, tempWord, strlen(tempWord));
            }
            
            /* Check if adding this word would exceed line width */
            if (currentWidth + wordWidth > maxWidth && currentWidth > 0) {
                /* Start new line */
                *dst++ = '\n';
                currentWidth = 0;
            }
            
            /* Copy the word */
            strcpy(dst, tempWord);
            dst += strlen(tempWord);
            currentWidth += wordWidth;
            
            /* Add space or newline */
            if (tempChar == ' ') {
                *dst++ = ' ';
                currentWidth += TextLength(&screen->RastPort, " ", 1);
            } else if (tempChar == '\n') {
                *dst++ = '\n';
                currentWidth = 0;
            }
            
            /* Move to next word */
            wordStart = src + 1;
        }
        src++;
    }
    
    /* Handle the last word if any */
    if (*wordStart && (dst - wrappedText) < sizeof(wrappedText) - 1) {
        strcpy(dst, wordStart);
    }
    
    return wrappedText;
}

/*
 * Show error dialog using Reaction requester
 * Automatically detects test mode when taskID is 0
 */
VOID ShowErrorDialog(ULONG errorCode, STRPTR description, STRPTR explanation, ULONG taskID)
{
    Object *reqobj;
    char title[256];
    char message[1024];
    char wrappedMessage[2048];
    struct Screen *screen;
    ULONG screenWidth;
    ULONG maxTextWidth;
    BOOL testMode;
    
    /* Determine if this is test mode based on taskID */
    testMode = (taskID == 0);
    
    /* Get the default screen information - use ActiveScreen for simplicity */
    screen = (struct Screen *)IntuitionBase->ActiveScreen;
    
    /* Set default values first */
    screenWidth = 640;
    
    /* Only try to access screen if it's valid */
    if (screen != NULL && screen->Width > 0) {
        screenWidth = screen->Width;
    }
    
    /* Calculate maximum text width - smart sizing for different screen sizes */
    if (screenWidth <= 320) {
        /* Small screens (PAL, NTSC) - use most of the width */
        maxTextWidth = (screenWidth * 90) / 100;
    } else if (screenWidth <= 640) {
        /* Medium screens - use 80% */
        maxTextWidth = (screenWidth * 80) / 100;
    } else if (screenWidth <= 1024) {
        /* Large screens - use 70% */
        maxTextWidth = (screenWidth * 70) / 100;
    } else {
        /* Very large screens - use 60% for readability */
        maxTextWidth = (screenWidth * 60) / 100;
    }
    
    /* Ensure minimum readable width */
    if (maxTextWidth < 200) {
        maxTextWidth = 200;
    }
    
    /* Format the title and message */
    sprintf(title, "Guru Meditation Insight");
    
    if (testMode) {
        /* Test mode - show that this is a random selected Insight */
        sprintf(message, 
                "\n\n"
                "Error Code: 0x%08X \n\n"
                "Task ID: (Test Mode) \n\n"
                "Error: %s \n\n"
                "\33i%s\33n \n\n",
                errorCode, description, explanation);
    } else {
        /* Normal mode - show actual task ID from LastAlert */
        sprintf(message, 
                "\n\n"
                "Error Code: 0x%08X \n\n"
                "Task ID: 0x%08X \n\n"
                "Error: %s \n\n"
                "\33i%s\33n \n\n",
                errorCode, taskID, description, explanation);
    }
    
    /* Word wrap the message if we have a valid screen */
    if (screen != NULL && screen->Width > 0) {
        strcpy(wrappedMessage, WordWrapText(message, maxTextWidth, screen));
    } else {
        strcpy(wrappedMessage, message);
    }
    
    /* Create the requester object with all attributes */
    reqobj = NewObject(RequesterClass, NULL,
                       REQ_TitleText, title,
                       REQ_BodyText, wrappedMessage,
                       REQ_Type, REQTYPE_INFO,
                       REQ_GadgetText, "OK",
                       REQ_Image, REQIMAGE_ERROR,
                       TAG_END);
    
    if (reqobj != NULL) {
        /* Show the requester and wait for user response */
        DoMethod(reqobj, RM_OPENREQ, NULL, 0L, NULL, TAG_DONE);
        
        /* Clean up the requester object */
        DisposeObject(reqobj);
    }
}

/*
 * Initialize required libraries
 */
BOOL InitializeLibraries(VOID)
{
    /* Open intuition.library - use conservative version requirement */
    IntuitionBase = (struct IntuitionBase *)OpenLibrary("intuition.library", 47);
    if (IntuitionBase == NULL) {
        return FALSE;
    }
    
    /* Open utility.library - use conservative version requirement */
    UtilityBase = OpenLibrary("utility.library", 47);
    if (UtilityBase == NULL) {
        CloseLibrary((struct Library *)IntuitionBase);
        IntuitionBase = NULL;
        return FALSE;
    }
    
    /* Open graphics.library - use conservative version requirement */
    GfxBase = (struct GfxBase *) OpenLibrary("graphics.library", 47);
    if (GfxBase == NULL) {
        CloseLibrary(UtilityBase);
        UtilityBase = NULL;
        CloseLibrary((struct Library *)IntuitionBase);
        IntuitionBase = NULL;
        return FALSE;
    }
    
    return TRUE;
}

/*
 * Initialize Reaction classes
 */
BOOL InitializeApplication(VOID)
{
    /* Open requester.class */
    RequesterBase = (struct ClassLibrary *)OpenLibrary("requester.class", 47);
    if (RequesterBase == NULL) {
        return FALSE;
    }
    
    /* Get the requester class */
    RequesterClass = REQUESTER_GetClass();
    if (RequesterClass == NULL) {
        CloseLibrary(RequesterBase);
        RequesterBase = NULL;
        return FALSE;
    }
    
    return TRUE;
}

/*
 * Clean up resources
 */
VOID Cleanup(VOID)
{
    /* Close Reaction classes first - be extra careful with order */
    if (RequesterClass != NULL) {
        /* For Reaction classes, we don't call FreeClass - just clear the pointer */
        /* The class will be cleaned up when the library is closed */
        RequesterClass = NULL;
    }
    
    if (RequesterBase != NULL) {
        CloseLibrary(RequesterBase);
        RequesterBase = NULL;
    }
    
    /* Close libraries in reverse order of opening */
    if (GfxBase != NULL) {
        CloseLibrary((struct Library *)GfxBase);
        GfxBase = NULL;
    }
    
    if (UtilityBase != NULL) {
        CloseLibrary(UtilityBase);
        UtilityBase = NULL;
    }
    
    if (IntuitionBase != NULL) {
        CloseLibrary((struct Library *)IntuitionBase);
        IntuitionBase = NULL;
    }
}

/*
 * Safe exit function that ensures proper cleanup
 */
VOID SafeExit(int status)
{
    /* Final cleanup pass */
    Cleanup();
    
    /* Exit with proper status */
    exit(status);
}

/*
 * Check if running from Workbench
 */
BOOL CheckWorkbenchStartup(VOID)
{
    /* When Workbench starts a program, it sets argc to 0 and puts the WBStartup message in argv[0] */
    /* We need to check if we're in main() with argc == 0, which indicates Workbench startup */
    /* This function will be called from main() where we can check the argc parameter */
    
    /* For now, return FALSE - we'll handle this in main() by checking argc */
    return FALSE;
}

/**
 * @brief Converts a hexadecimal string to a ULONG.
 * @param hexString The null-terminated string containing the hex value.
 * @return The converted ULONG value.
 * 
 * This function parses hex strings the same way ParseLastAlert reads ULONG values
 * from memory - as a big-endian 32-bit value. The string is processed left-to-right,
 * with each character representing 4 bits, building the value from most significant
 * to least significant digit.
 */
ULONG HexStringToULong(STRPTR hexString) {
    ULONG result;
    char localBuffer[32];
    int startIdx;
    int idx;
    int digitCount;
    unsigned char charVal;
    ULONG digitVal;
    int i;
    
    /* Initialize all variables - C89 compliance */
    result = 0;
    startIdx = 0;
    idx = 0;
    digitCount = 0;
    charVal = 0;
    digitVal = 0;
    
    /* Check for NULL pointer or empty string */
    if (hexString == NULL || hexString[0] == '\0') {
        return (ULONG)-1;
    }

    /* Copy string to local buffer */
    i = 0;
    while (i < 31 && hexString[i] != '\0') {
        localBuffer[i] = hexString[i];
        i++;
    }
    localBuffer[i] = '\0';

    /* Skip "0x" prefix if present */
    if (localBuffer[0] == '0' && (localBuffer[1] == 'x' || localBuffer[1] == 'X')) {
        startIdx = 2;
    }

    /* Check if we have at least one character after prefix */
    if (localBuffer[startIdx] == '\0') {
        return (ULONG)-1;
    }

    /* Process each character from left to right using while loop */
    i = startIdx;
    
    while (i < 32 && localBuffer[i] != '\0' && digitCount < 8) {
        charVal = (unsigned char)localBuffer[i];
        
        /* Convert to uppercase */
        if (charVal >= 'a' && charVal <= 'z') {
            charVal = charVal - 'a' + 'A';
        }
        
        /* Convert hex char to digit value */
        if (charVal >= '0' && charVal <= '9') {
            digitVal = charVal - '0';
        } else if (charVal >= 'A' && charVal <= 'F') {
            digitVal = charVal - 'A' + 10;
        } else {
            return (ULONG)-1;
        }
        
        /* Shift result left by 4 bits and add new digit */
        result = (result << 4) | digitVal;
        
        digitCount = digitCount + 1;
        i = i + 1;
    }
    
    idx = i;  /* Set idx for later checks */
    
    /* Check for too many digits */
    if (localBuffer[idx] != '\0') {
        return (ULONG)-1;
    }
    
    /* Must have exactly 8 digits */
    if (digitCount != 8) {
        return (ULONG)-1;
    }
    
    return result;
}

/**
 * @brief Checks if a string looks like a hexadecimal number.
 * @param str The null-terminated string to check.
 * @return TRUE if the string looks like a hex number, FALSE otherwise.
 * 
 * A string looks like a hex number if:
 * - It starts with "0x" or "0X" followed by one or more hex digits, OR
 * - It consists entirely of hex digits (0-9, A-F, a-f)
 */
BOOL LooksLikeHexNumber(STRPTR str) {
    STRPTR current;
    unsigned char tempChar;
    
    /* Check for NULL pointer or empty string */
    if (str == NULL || *str == '\0') {
        return FALSE;
    }
    
    current = str;
    
    /* Skip "0x" or "0X" prefix if present */
    if (current[0] == '0' && (current[1] == 'x' || current[1] == 'X')) {
        current += 2;
        /* After 0x prefix, we must have at least one hex digit */
        if (*current == '\0') {
            return FALSE;
        }
    }
    
    /* Check if remaining characters are all hex digits */
    while (*current != '\0') {
        tempChar = (unsigned char)*current;
        
        /* Convert to uppercase for case-insensitive processing */
        if (tempChar >= 'a' && tempChar <= 'z') {
            tempChar = tempChar - 'a' + 'A';
        }
        
        /* Check if it's a valid hex digit */
        if (!((tempChar >= '0' && tempChar <= '9') || 
              (tempChar >= 'A' && tempChar <= 'F'))) {
            return FALSE;  /* Invalid character found */
        }
        
        current++;
    }
    
    /* All characters are valid hex digits */
    return TRUE;
}

/**
 * @brief Validates that a string is exactly 8 hexadecimal digits.
 * @param hexString The null-terminated string to validate.
 * @return TRUE if the string is exactly 8 hex digits (with or without 0x prefix), FALSE otherwise.
 * 
 * This function ensures the error code is exactly 8 hex digits, which represents a 32-bit value.
 * The string may optionally start with "0x" or "0X" prefix.
 */
BOOL ValidateHexErrorCode(STRPTR hexString) {
    STRPTR current;
    int digitCount = 0;
    unsigned char tempChar;
    
    /* Check for NULL pointer or empty string */
    if (hexString == NULL || *hexString == '\0') {
        return FALSE;
    }
    
    current = hexString;
    
    /* Skip "0x" or "0X" prefix if present */
    if (current[0] == '0' && (current[1] == 'x' || current[1] == 'X')) {
        current += 2;
    }
    
    /* Count hex digits - must be exactly 8 */
    while (*current != '\0') {
        tempChar = (unsigned char)*current;
        
        /* Convert to uppercase for case-insensitive processing */
        if (tempChar >= 'a' && tempChar <= 'z') {
            tempChar = tempChar - 'a' + 'A';
        }
        
        /* Check if it's a valid hex digit */
        if ((tempChar >= '0' && tempChar <= '9') || 
            (tempChar >= 'A' && tempChar <= 'F')) {
            digitCount++;
        } else {
            /* Invalid character found */
            return FALSE;
        }
        
        current++;
    }
    
    /* Must be exactly 8 hex digits */
    return (digitCount == 8);
}

/*
 * Parse the LastAlert array from SysBase to get the complete error code and task ID.
 * Returns (ULONG)-1 if no error, otherwise the error code.
 * 
 * According to Amiga documentation:
 * - LastAlert[0] contains the most recent alert code
 * - LastAlert[1] contains additional error information
 * - LastAlert[2] contains more error context
 * - LastAlert[3] contains the Task ID that caused the error
 */
ULONG ParseLastAlert(ULONG *taskID) {
    ULONG errorCode = (ULONG)-1;
    struct ExecBase *sysBase;
    ULONG *guruData, guruFromMemory;

    // Try to get SysBase from the system - this is the robust way
    // On Amiga, SysBase is at address 4
    sysBase = *(struct ExecBase **)4;
    
    // Fallback: if direct access fails, try the global SysBase variable
    if (sysBase == NULL && SysBase != NULL) {
        sysBase = SysBase;
    }
    
    // Check if running on AmigaOS 4 (version 50 or above)
    // LastAlert structure doesn't exist on OS4, so fail silently
    if (sysBase != NULL && sysBase->LibNode.lib_Version >= 50) {
        *taskID = 0;
        return (ULONG)-1;  // Return "no error" silently
    }
    
    if (sysBase != NULL) {
        guruData = (ULONG *) 0x0100;
        guruFromMemory = guruData[0];
        
        if (guruFromMemory != 0) {
            // If GuruData[0] is NOT zero, use it directly
            errorCode = guruFromMemory;
            *taskID = guruData[1];
        } else {
            ULONG la0 = sysBase->LastAlert[0];
            ULONG la1 = sysBase->LastAlert[1];
            ULONG la2 = sysBase->LastAlert[2];
            ULONG la3 = sysBase->LastAlert[3];
            
            // Check if there's actually an error code (not -1 or 0)
            if (la0 != (ULONG)-1 && la0 != 0) {
                errorCode = la0;
                *taskID = la3;
            } else {
                *taskID = 0;
            }
        }
    } else {
        *taskID = 0;
    }
    
    return errorCode;
}
