/*
 * Insight - Guru Meditation Error Checker
 *
 * This is a simplified version of Insights that only:
 * 1. Checks the LastAlert from SysBase on startup
 * 2. Shows an autorequest with the appropriate error message
 * 3. Exits after displaying the error
 *
 * Copyright (c) 2025 amigazen project
 * Licensed under BSD 2-Clause License
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
ULONG ParseLastAlert(ULONG *taskID, BOOL verbose);  /* Parse LastAlert array and return error code */


/* Library base pointers */
extern struct ExecBase *SysBase;
extern struct IntuitionBase *IntuitionBase;
extern struct Library *UtilityBase;
extern struct GfxBase *GfxBase;

/* Reaction class library bases */
struct ClassLibrary *RequesterBase = NULL;

/* Reaction class handles */
Class *RequesterClass = NULL;

static const char *verstag = "$VER: Insight 47.1 (17.08.2025)\n";
static const char *stack_cookie = "$STACK: 8192\n";

/* Application variables - none needed for this simple app */

/* Main entry point */
int main(int argc, char *argv[])
{
    struct ErrorInfo *errorInfo;
    ULONG guruCode;
    ULONG taskID;
    struct RDArgs *rdargs;
    BOOL testMode = FALSE;
    BOOL verbose = FALSE;
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
        if (verbose) {
            Printf("Insight: Parsing LastAlert array from SysBase...\n");
        }
        guruCode = ParseLastAlert(&taskID, verbose);
        
        /* Check if there's actually an error code (not -1) */
        /* According to Amiga documentation, LastAlert[0] = -1 (0xFFFFFFFF) means no error */
        /* Additional safety check: ensure the value is reasonable (not corrupted) */
        if (guruCode == (ULONG)-1 || guruCode == 0) {
            /* No error - exit cleanly */
            if (verbose) {
                Printf("Insight: No error detected, exiting cleanly\n");
            }
            SafeExit(RETURN_OK);
        } else {
            /* Error exists - parse the error code and show requester */
            if (verbose) {
                Printf("Insight: Error detected, initializing libraries...\n");
            }
            if (!InitializeLibraries()) {
                SafeExit(RETURN_FAIL);
            }
            if (!InitializeApplication()) {
                Cleanup();
                SafeExit(RETURN_FAIL);
            }
            
            /* Parse the error code to get description and explanation */
            if (verbose) {
                Printf("Insight: Looking up error code 0x%08X in database...\n", guruCode);
            }
            errorInfo = GainInsight(guruCode);
            if (errorInfo != NULL) {
                /* Found error in database - show with proper description */
                if (verbose) {
                    Printf("Insight: Error found in database, showing dialog...\n");
                }
                ShowErrorDialog(guruCode, errorInfo->description, errorInfo->insight, taskID);
                FreeErrorInfo(errorInfo);  /* Free allocated memory */
            } else {
                /* Error code not in database - show generic error dialog */
                if (verbose) {
                    Printf("Insight: Error code not in database, showing generic error dialog\n");
                }
                ShowErrorDialog(guruCode, "Unknown Error", "This error code is not recognized by the Insight database.", taskID);
            }
            
            Cleanup();
            
            SafeExit(RETURN_FAIL);  /* Exit with failure status since there was an error */
        }
    } else {
        /* Command line mode: parse arguments and handle accordingly */
        STRPTR args[3] = {NULL, NULL, NULL};  /* ERROR/K, GURU/S, VERBOSE/S */
        STRPTR errorArg = NULL;
        int i;  /* C89 compliance - declare at start of block */
        
        /* Parse command line arguments */
        
        rdargs = ReadArgs("ERROR/K,GURU/S,VERBOSE/S", (LONG *)args, NULL);
        if (rdargs != NULL) {
            /* Set flags first so we can use them in debug output */
            testMode = (args[1] != NULL);      /* GURU/S is a switch */
            verbose = (args[2] != NULL);       /* VERBOSE/S is a switch */
            
            /* Copy the string content before freeing the args */
            if (args[0] != NULL) {
                /* Use a local buffer to store the error argument */
                static char errorBuffer[32];  /* Buffer for error code (max 8 hex chars + 0x prefix + null) */
                
                /* Clear the buffer first */
                memset(errorBuffer, 0, sizeof(errorBuffer));
                
                /* Insight: show what we're working with */
                if (verbose) {
                    Printf("Insight: args[0] pointer: 0x%08X\n", (ULONG)args[0]);
                    Printf("Insight: args[0] content: '%s'\n", args[0]);
                    Printf("Insight: args[0] length: %d\n", strlen(args[0]));
                    
                    /* Show raw bytes of args[0] */
                    Printf("Insight: Raw bytes of args[0]: ");
                    for (i = 0; i < 16 && args[0][i] != '\0'; i++) {
                        Printf("0x%02X ", (unsigned char)args[0][i]);
                    }
                    Printf("\n");
                }
                
                /* Try to copy the string manually character by character */
                i = 0;
                while (i < sizeof(errorBuffer) - 1 && args[0][i] != '\0') {
                    errorBuffer[i] = args[0][i];
                    i++;
                }
                errorBuffer[i] = '\0';  /* Ensure null termination */
                
                if (verbose) {
                    Printf("Insight: Manually copied to errorBuffer: '%s'\n", errorBuffer);
                    Printf("Insight: Raw bytes of errorBuffer after copy: ");
                    for (i = 0; i < 16 && errorBuffer[i] != '\0'; i++) {
                        Printf("0x%02X ", (unsigned char)errorBuffer[i]);
                    }
                    Printf("\n");
                }
                
                errorArg = errorBuffer;
            } else {
                errorArg = NULL;
            }
            
            if (verbose) {
                Printf("Insight: ReadArgs results - ERROR: '%s', GURU: %s, VERBOSE: %s\n", 
                       errorArg ? errorArg : (STRPTR)"(NULL)", 
                       testMode ? "TRUE" : "FALSE", 
                       verbose ? "TRUE" : "FALSE");
            }
            
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
                } else if (argv[i][0] == 'V' && argv[i][1] == 'E' && argv[i][2] == 'R' && 
                           argv[i][3] == 'B' && argv[i][4] == 'O' && argv[i][5] == 'S' && 
                           argv[i][6] == 'E' && argv[i][7] == '\0') {
                    verbose = TRUE;
                }
            }
        }
        
        /* If we have arguments but ReadArgs failed and we didn't find any valid parameters, exit */
        if (argc > 1 && errorArg == NULL && !testMode && !verbose) {
            SafeExit(RETURN_OK);
        }
        
        /* Show parameter summary if verbose mode is enabled */
        if (verbose) {
            Printf("Insight: Final parameter summary - testMode: %s, verbose: %s, errorArg: '%s'\n",
                   testMode ? "TRUE" : "FALSE", 
                   verbose ? "TRUE" : "FALSE",
                   errorArg ? errorArg : (STRPTR)"(NULL)");
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

            guruCode = HexStringToULong(errorArg);
            /* Check if parsing was successful - HexStringToULong returns (ULONG)-1 for invalid input */
            if (guruCode != (ULONG)-1) {
                if (verbose) {
                    Printf("Insight: Successfully parsed error code: 0x%08X\n", guruCode);
                }
                
                errorInfo = GainInsight(guruCode);
                if (errorInfo != NULL) {
                    if (testMode) {
                        ShowErrorDialog(guruCode, errorInfo->description, errorInfo->insight, 0);
                    }
                    /* Always show error information when ERROR parameter is specified */
                    Printf("Error Code: 0x%08X\nError: %s\n%s\n", 
                           guruCode, errorInfo->description, errorInfo->insight);
                    success = TRUE;  /* Successfully parsed and displayed error */
                    FreeErrorInfo(errorInfo);  /* Free allocated memory */
                } else {
                    Printf("Unknown error code: 0x%08X\n", guruCode);
                    success = FALSE;  /* Error code not found */
                }
            } else {
                Printf("Invalid error code format. Use ERROR=0xXXXXXXXX\n");
                success = FALSE;  /* Invalid format */
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
                Printf("Failed to lookup error code 0x%08X\n", guruCode);
                success = FALSE;
            }
        } else {
            /* No special parameters - check SysBase LastAlert */
            /* LastAlert[4] is an array containing the last 4 system alert codes */
            /* The 4 LONG values together form the complete error code */
            if (verbose) {
                Printf("Insight: Command line mode - parsing LastAlert array from SysBase...\n");
            }
            guruCode = ParseLastAlert(&taskID, verbose);
            
            /* Check if there's actually an error code (not -1) */
            /* According to Amiga documentation, LastAlert[0] = -1 (0xFFFFFFFF) means no error */
            if (guruCode == (ULONG)-1) {
                /* No error - this is normal, not a failure */
                if (verbose) {
                    Printf("Insight: No error detected in LastAlert\n");
                }
                success = TRUE;  /* Successfully determined no error exists */
            } else {
                /* Error exists - parse the error code and print to console */
                if (verbose) {
                    Printf("Insight: Error detected in LastAlert, looking up in database...\n");
                }
                errorInfo = GainInsight(guruCode);
                if (errorInfo != NULL) {
                    Printf("Error Code: 0x%08X\n\nTask ID: 0x%08X\n\nError: %s\n\n%s\n", 
                           guruCode, taskID, errorInfo->description, errorInfo->insight);
                    FreeErrorInfo(errorInfo);  /* Free allocated memory */
                } else {
                    Printf("Error Code: 0x%08X\n\nTask ID: 0x%08X\n\nError: Unknown Error\n\nNo Insight for this error code.\n", 
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
 */
ULONG HexStringToULong(STRPTR hexString) {
    ULONG result = 0;
    STRPTR current = hexString;
    ULONG tempChar;
    ULONG tempDigit;
    int i;

    Printf("Insight: HexStringToULong called with: '%s'\n", hexString ? (char *)hexString : "NULL");
    if (hexString) {
        Printf("Insight: Raw bytes: ");
        for (i = 0; i < 16 && hexString[i] != '\0'; i++) {
            Printf("0x%02X ", (unsigned char)hexString[i]);
        }
        Printf("\n");
    }

    // Check for NULL pointer or empty string
    if (hexString == NULL || *hexString == '\0') {
        Printf("Insight: NULL or empty string, returning -1\n");
        return (ULONG)-1;
    }

    // Skip "0x" prefix if present
    if (current[0] == '0' && (current[1] == 'x' || current[1] == 'X')) {
        current += 2;
        Printf("Insight: Skipped 0x prefix, current now: '%s'\n", current);
    }

    // Check if we have at least one character after prefix
    if (*current == '\0') {
        Printf("Insight: Empty string after prefix, returning -1\n");
        return (ULONG)-1;  // Empty string after prefix
    }

    // Process each character of the string
    while (*current != '\0') {
        tempChar = *current;
        
        Printf("Insight: Processing char '%c' (0x%02X) at offset %d\n", *current, (unsigned char)*current, (int)(current - hexString));
        
        // Convert to uppercase for case-insensitive processing
        if (tempChar >= 'a' && tempChar <= 'z') {
            tempChar = tempChar - 'a' + 'A';
        }
        
        // Convert the hex character to its integer value
        if (tempChar >= '0' && tempChar <= '9') {
            tempDigit = tempChar - '0';
        } else if (tempChar >= 'A' && tempChar <= 'F') {
            tempDigit = tempChar - 'A' + 10;
        } else {
            Printf("Insight: Invalid hex character: 0x%02X, returning -1\n", tempChar);
            // Invalid character encountered, return (ULONG)-1 as error indicator
            return (ULONG)-1;
        }
        
        // Left-shift the current result by 4 bits and add the new digit.
        // This is the correct method for building a number from a string
        // regardless of machine endianness.
        result = (result << 4) | tempDigit;
        
        Printf("Insight: Char '%c' -> digit %lu, result now 0x%08X\n", *current, tempDigit, result);
        
        current++;
    }
    
    return result;
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
ULONG ParseLastAlert(ULONG *taskID, BOOL verbose) {
    ULONG lastAlert[4];
    ULONG errorCode;
    
    errorCode = (ULONG)-1; // Assume no error initially

    if (SysBase == NULL) {
        return errorCode; // Cannot parse if SysBase is not available
    }

    // Access LastAlert safely - this field should always be accessible
    // Copy the 4 LONG values from SysBase->LastAlert
    memcpy(lastAlert, SysBase->LastAlert, sizeof(lastAlert));

    // Debug output to understand the LastAlert structure
    if (verbose) {
        Printf("Insight: LastAlert[0] = 0x%08X (primary error code)\n", lastAlert[0]);
        Printf("Insight: LastAlert[1] = 0x%08X (additional error info)\n", lastAlert[1]);
        Printf("Insight: LastAlert[2] = 0x%08X (error context)\n", lastAlert[2]);
        Printf("Insight: LastAlert[3] = 0x%08X (task ID)\n", lastAlert[3]);
    }

    // The primary error code is in LastAlert[0]
    // According to Amiga documentation, LastAlert[0] = -1 (0xFFFFFFFF) means no error
    if (lastAlert[0] != (ULONG)-1 && lastAlert[0] != 0) {
        // There is an error - use LastAlert[0] as the primary error code
        errorCode = lastAlert[0];
        
        // The Task ID is in LastAlert[3]
        *taskID = lastAlert[3];
        
        if (verbose) {
            Printf("Insight: Error detected - Code: 0x%08X, Task ID: 0x%08X\n", errorCode, *taskID);
        }
        
        return errorCode;
    }

    // No error detected
    if (verbose) {
        Printf("Insight: No error detected in LastAlert\n");
    }
    *taskID = 0;
    return errorCode; // Indicate no error
}
