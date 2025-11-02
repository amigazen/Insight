/*
 * Error Codes Database Implementation
 *
 * This file contains a consolidated and verified database of Guru Meditation error codes
 * compiled from various Amiga documentation sources.
 *
 * Copyright (c) 2025 amigazen project
 * Licensed under BSD 2-Clause License
 */

 #include <exec/types.h>
 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include <proto/dos.h>
 #include "error_codes.h"
 
  /* Consolidated error lookup table */
  struct ErrorInfo errorTable[] = {
     /* No Error */
     { 0x00000000, "No Error", "No error condition detected." },
     
     /* CPU Exceptions (0x01-0x1F) - Recovery Alerts where applicable*/
     { 0x00000001, "Task held", "Check for improper task synchronization or a missing `Resume()` call." },
     { 0x00000002, "Bus error", "Inspect memory mapped I/O access for invalid addresses or faulty RAM." },
     { 0x00000003, "Address error", "Look for unaligned memory access in pointers and data structures." },
     { 0x00000004, "Illegal instruction", "Check for wild jumps into data or corrupted program code." },
     { 0x00000005, "Zero divide", "Validate all divisors to ensure they can't be zero." },
     { 0x00000006, "CHK instruction", "Verify array indices and loop boundaries for 'off-by-one' errors." },
     { 0x00000007, "TRAPV instruction", "Check signed integer operations for potential arithmetic overflows." },
     { 0x00000008, "Privilege violation", "Ensure the program isn't attempting to run privileged instructions in user mode." },
     { 0x00000009, "Trace", "This is a normal event during a debugging session." },
     { 0x0000000A, "Line 1010 Emulator", "Check for unsupported FPU instructions without a coprocessor." },
     { 0x0000000B, "Line 1111 Emulator", "Check for unsupported FPU instructions without a coprocessor." },
     { 0x0000000C, "Line 1010 Emulator", "Check for unsupported FPU instructions without a coprocessor." },
     { 0x0000000D, "Line 1111 Emulator", "Check for unsupported FPU instructions without a coprocessor." },
     { 0x0000000E, "Stack frame format error", "Inspect interrupt handlers or context switch code for stack corruption." },
     { 0x0000000F, "Format error", "Check for corrupted stack frames or invalid exception handling." },
     { 0x00000010, "Uninitialized interrupt", "Check for missing interrupt vector initialization." },
     { 0x00000011, "Spurious interrupt", "Check for hardware glitches or missing interrupt handlers." },
     { 0x00000012, "Level 1 interrupt", "Check for missing level 1 interrupt handler." },
     { 0x00000013, "Level 2 interrupt", "Check for missing level 2 interrupt handler." },
     { 0x00000014, "Level 3 interrupt", "Check for missing level 3 interrupt handler." },
     { 0x00000015, "Level 4 interrupt", "Check for missing level 4 interrupt handler." },
     { 0x00000016, "Level 5 interrupt", "Check for missing level 5 interrupt handler." },
     { 0x00000017, "Level 6 interrupt", "Check for missing level 6 interrupt handler." },
     { 0x00000018, "Spurious interrupt error", "Review interrupt masking logic and hardware for glitches." },
     { 0x00000019, "Autovector Level 1", "Check the interrupt vector table for missing handlers." },
     { 0x0000001A, "Autovector Level 2", "Check the interrupt vector table for missing handlers." },
     { 0x0000001B, "Autovector Level 3", "Check the interrupt vector table for missing handlers." },
     { 0x0000001C, "Autovector Level 4", "Check the interrupt vector table for missing handlers." },
     { 0x0000001D, "Autovector Level 5", "Check the interrupt vector table for missing handlers." },
     { 0x0000001E, "Autovector Level 6", "Check the interrupt vector table for missing handlers." },
     { 0x0000001F, "Autovector Level 7", "Check the interrupt vector table for missing handlers." },
     
     /* File System Errors (0x20-0x2F) */
     { 0x00000020, "File system error", "Check disk hardware and filesystem integrity for general I/O errors." },
     { 0x00000021, "Disk read error", "Examine the disk for bad sectors or a corrupted filesystem." },
     { 0x00000022, "Disk write error", "Verify disk space, hardware integrity, and write protection status." },
     { 0x00000023, "Disk full", "Delete unnecessary files to free up space." },
     { 0x00000024, "File not found", "Verify that the file path and name are correct." },
     { 0x00000025, "Permission denied", "Check file permissions and user privileges." },
     { 0x00000026, "File system corruption", "Run a disk repair utility to fix inconsistent metadata." },
     { 0x00000027, "Directory error", "Inspect the directory structure for corruption or an incorrect path." },
     { 0x00000028, "File system not mounted", "Check the mount configuration and ensure the volume is inserted." },
     { 0x00000029, "File system read-only", "Check the disk's write-protection or filesystem state." },
     { 0x0000002A, "File system busy", "Look for file locks or other processes using the filesystem." },
     { 0x0000002B, "File system timeout", "Investigate slow hardware or network latency on remote filesystems." },
     { 0x0000002C, "File system invalid", "Reformat the disk or use a repair utility to fix the format." },
     { 0x0000002D, "File system unknown", "Use a debugger to trace the cause." },
     
     /* Floating Point Coprocessor Exceptions (0x30-0x37) */
     { 0x00000030, "FPCP branch/set unordered", "Check for uninitialized floating-point variables, specifically NaNs." },
     { 0x00000031, "FPCP inexact result", "This is a normal event in floating-point calculations and doesn't usually indicate an error." },
     { 0x00000032, "FPCP divide by zero", "Validate all floating-point divisors to prevent division by zero." },
     { 0x00000033, "FPCP underflow", "This indicates the result was too small to be represented; check for repeated multiplication by small fractions." },
     { 0x00000034, "FPCP operand error", "Inspect floating-point data for corruption or invalid data types being used in calculations." },
     { 0x00000035, "FPCP overflow", "Check for operations resulting in values too large for floating-point representation." },
     { 0x00000036, "FPCP signalling NAN", "A special NaN value was used to explicitly signal an error condition." },
     { 0x00000037, "FPCP reserved", "This indicates a defect in a program trying to execute an undefined instruction." },
 
     /* MMU Exceptions (0x38-0x3A) */
     { 0x00000038, "MMU configuration error", "Check the Memory Management Unit registers and page tables for incorrect settings." },
     { 0x00000039, "68851 illegal operation", "Inspect software that targets the MMU coprocessor with invalid instructions." },
     { 0x0000003A, "68851 access level violation", "Look for programs attempting to access memory without the required permissions." },
     
     /* Network Errors (0x40-0x4F) */
     { 0x00000040, "Network error", "Check physical network connectivity and cable integrity." },
     { 0x00000041, "Connection failed", "Verify firewall settings, IP address, and that the remote service is running." },
     { 0x00000042, "Connection lost", "Check for network interruptions or abrupt disconnections from the remote host." },
     { 0x00000043, "Timeout", "Investigate network latency, a busy server, or firewall issues." },
     { 0x00000044, "Network unreachable", "Check local network configuration and routing tables." },
     { 0x00000045, "Host unreachable", "Verify the destination host's power and network connectivity." },
     { 0x00000046, "Protocol error", "Look for malformed packets or a mismatch in protocol versions." },
     { 0x00000047, "Network congestion", "This may require increasing network bandwidth to handle traffic." },
     { 0x00000048, "Network reset", "The remote host forcibly closed the connection; check the remote application's status." },
     { 0x00000049, "Network refused", "The remote service may not be running or is blocked by a firewall." },
     { 0x0000004A, "Network aborted", "Check for protocol violations on the local host." },
     { 0x0000004B, "Network invalid", "An invalid network operation was attempted; check socket type or unsupported commands." },
     { 0x0000004C, "Network busy", "The network interface is overloaded; a retransmission may be necessary." },
     { 0x0000004D, "Network not available", "Check network configuration and driver installation." },
     { 0x0000004E, "Network configuration", "Review IP address, subnet mask, and gateway settings for errors." },
     { 0x0000004F, "Network unknown", "Use a debugger to trace the cause." },
     
     /* Graphics Errors (0x50-0x5F) */
     { 0x00000050, "Graphics error", "Check for display driver defects or hardware issues with graphics components." },
     { 0x00000051, "Screen mode not available", "Verify that the requested resolution and color depth are supported by the hardware and driver." },
     { 0x00000052, "Bitmap error", "Check for corrupted bitmap data, insufficient memory, or an invalid operation." },
     { 0x00000053, "Sprite error", "Inspect sprite data and state for corruption or memory issues." },
     { 0x00000054, "Copper error", "Debug the Copper list for invalid instructions or non-terminating loops." },
     { 0x00000055, "Blitter error", "Ensure the blitter hardware is in a valid state and parameters are correct for the operation." },
     { 0x00000056, "Display error", "Check the video output and monitor connection for issues." },
     { 0x00000057, "Palette error", "Verify color values and palette indices for correctness." },
     { 0x00000058, "Viewport error", "Adjust viewport dimensions and check for conflicts with other viewports." },
     { 0x00000059, "RastPort error", "Inspect the RastPort structure or drawing commands for corruption." },
     { 0x0000005A, "Layer error", "Debug layer creation, deletion, or manipulation logic." },
     { 0x0000005B, "Window error", "Check window handles, dimensions, and memory availability." },
     { 0x0000005C, "Screen error", "Review screen creation and screen mode management code." },
     { 0x0000005D, "Graphics library error", "Inspect library initialization and calls for defects." },
     { 0x0000005E, "Graphics device error", "A faulty video chip or corrupted driver may be the cause." },
     { 0x0000005F, "Graphics unknown", "Use a debugger to trace the cause." },
     
     /* Audio Errors (0x60-0x6F) */
     { 0x00000060, "Audio error", "Check for hardware issues or a corrupted sound driver." },
     { 0x00000061, "Audio device not available", "Confirm that the audio device is installed and not in use by another application." },
     { 0x00000062, "Audio format not supported", "Validate that the audio format (e.g., sample rate) is supported by the hardware and driver." },
     { 0x00000063, "Audio sample rate error", "Ensure the hardware can handle the requested sample rate." },
     { 0x00000064, "Audio buffer error", "This indicates a buffer underrun or overrun; check buffer management logic." },
     { 0x00000065, "Audio channel error", "An invalid channel was specified; check audio channel configuration." },
     { 0x00000066, "Audio volume error", "Verify that the requested volume level is within the valid range." },
     { 0x00000067, "Audio frequency error", "Check if the requested frequency is supported by the hardware." },
     { 0x00000068, "Audio library error", "Inspect library initialization and API calls for defects." },
     { 0x00000069, "Audio hardware error", "A faulty sound chip or bad connection is a likely cause." },
     { 0x0000006A, "Audio driver error", "Check for a corrupted driver file or a defect in the driver's code." },
     { 0x0000006B, "Audio configuration", "Review settings for sample rate, channels, and buffer size for correctness." },
     { 0x0000006C, "Audio initialization", "A missing device or a resource conflict can cause this error." },
     { 0x0000006D, "Audio playback error", "A corrupted audio stream or buffer underrun is likely the cause." },
     { 0x0000006E, "Audio recording error", "A resource conflict or hardware issue is a likely cause." },
     { 0x0000006F, "Audio unknown", "Use a debugger to trace the cause." },
     
     /* Input Device Errors (0x70-0x7F) */
     { 0x00000070, "Input device error", "Check for hardware issues or a corrupted driver." },
     { 0x00000071, "Mouse error", "Check hardware connections and for conflicts with other input devices." },
     { 0x00000072, "Keyboard error", "Check hardware connections and for conflicts with other input devices." },
     { 0x00000073, "Joystick error", "Check hardware connections and for conflicts with other input devices." },
     { 0x00000074, "Tablet error", "Check for a hardware issue or a corrupted driver." },
     { 0x00000075, "Touch screen error", "Check for a hardware issue or a corrupted driver." },
     { 0x00000076, "Input library error", "Inspect library initialization and API calls for defects." },
     { 0x00000077, "Input driver error", "Check for a corrupted driver file or a defect in the driver's code." },
     { 0x00000078, "Input configuration", "Review device settings and calibration data for errors." },
     { 0x00000079, "Input initialization", "A resource conflict or missing device can cause this error." },
     { 0x0000007A, "Input calibration", "A hardware issue or configuration error during calibration can cause this." },
     { 0x0000007B, "Input timeout", "This can be caused by a slow or unresponsive device." },
     { 0x0000007C, "Input buffer overflow", "The application isn't reading input fast enough; optimize the input loop." },
     { 0x0000007D, "Input device busy", "Check for a resource conflict with another process." },
     { 0x0000007E, "Input device not found", "Verify hardware connection and driver installation." },
     { 0x0000007F, "Input unknown", "Use a debugger to trace the cause." },
     
     /* Memory Protection Errors (0x80-0x8F) */
     { 0x00000080, "Memory protection error", "Use a debugger to trace the illegal memory access that was blocked by the MMU." },
     { 0x00000081, "Memory protection fault", "This is a general page fault; use a debugger to find the access violation." },
     { 0x00000082, "Memory access denied", "Check for an attempt to access a memory region without sufficient permissions." },
     { 0x00000083, "Memory write protection", "A write to a read-only memory region was attempted; debug for buffer overflows into code space." },
     { 0x00000084, "Memory read protection", "A read from a no-read memory region was attempted." },
     { 0x00000085, "Memory execute protection", "An attempt to execute code from a non-executable memory region occurred; this is a common security violation." },
     { 0x00000086, "Memory supervisor protection", "A user-mode program tried to access a memory region reserved for the OS kernel." },
     { 0x00000087, "Memory user protection", "A kernel-mode program tried to access a memory region reserved for user-mode programs." },
     { 0x00000088, "Memory stack protection", "This can be caused by a stack overflow or an attempt to execute code on the stack." },
     { 0x00000089, "Memory heap protection", "This can be caused by a heap buffer overflow or a `use-after-free` defect." },
     { 0x0000008A, "Memory code protection", "An attempt to modify executable code in memory was blocked." },
     { 0x0000008B, "Memory data protection", "An attempt to access or modify static data without permission occurred." },
     { 0x0000008C, "Memory shared protection", "This indicates a race condition or improper synchronization in shared memory." },
     { 0x0000008D, "Memory private protection", "This indicates a memory protection defect within a single process." },
     { 0x0000008E, "Memory cache protection", "This is a rare hardware-related error; check cache coherency in multi-processor systems." },
     { 0x0000008F, "Memory unknown protection", "Use a debugger to trace the cause." },
     
     /* Timer Errors (0x90-0x9F) */
     { 0x00000090, "Timer error", "Debug the timer library for defects or check for timer chip hardware faults." },
     { 0x00000091, "Timer not available", "A resource conflict or a hardware issue likely prevented timer allocation." },
     { 0x00000092, "Timer overflow", "Review timer configuration and logic for defects that cause the timer value to exceed its maximum." },
     { 0x00000093, "Timer underflow", "This is often caused by a configuration error where the timer value goes below its minimum." },
     { 0x00000094, "Timer resolution", "The requested timer resolution is not supported; check the hardware documentation." },
     { 0x00000095, "Timer accuracy", "The timer's accuracy is a hardware limitation." },
     { 0x00000096, "Timer library error", "Inspect the timer library's initialization and API calls for defects." },
     { 0x00000097, "Timer hardware error", "This indicates a physical fault with the timer chip." },
     { 0x00000098, "Timer driver error", "Check for a corrupted driver file or a defect in the driver's code." },
     { 0x00000099, "Timer configuration", "Review settings for period, mode, and frequency." },
     { 0x0000009A, "Timer initialization", "A resource conflict or a missing device prevented the timer from initializing." },
     { 0x0000009B, "Timer calibration", "A hardware issue or a configuration error occurred during timer setup." },
     { 0x0000009C, "Timer frequency", "The requested timer frequency is not supported by the hardware." },
     { 0x0000009D, "Timer period", "Verify the timer's documentation for valid period ranges." },
     { 0x0000009E, "Timer mode", "Verify the timer's documentation for valid modes." },
     { 0x0000009F, "Timer unknown", "Use a debugger to trace the cause." },
     
     /* Interrupt Errors (0xA0-0xAF) */
     { 0x000000A0, "Interrupt error", "Check for a hardware fault or a defect in the interrupt handling code." },
     { 0x000000A1, "Interrupt not available", "A resource conflict or a hardware issue likely prevented the interrupt from being available." },
     { 0x000000A2, "Interrupt conflict", "Reconfigure the hardware or drivers to resolve conflicts on the same interrupt request line." },
     { 0x000000A3, "Interrupt overflow", "A faulty device or a defect in an interrupt handler is causing interrupts to be received too quickly." },
     { 0x000000A4, "Interrupt underflow", "This indicates a defect in the interrupt handling logic where an unpending interrupt was acknowledged." },
     { 0x000000A5, "Interrupt priority", "Check the priority levels of registered interrupt handlers for conflicts." },
     { 0x000000A6, "Interrupt vector", "The interrupt vector table is corrupted; look for buffer overflows or wild pointers." },
     { 0x000000A7, "Interrupt handler", "A crash occurred within an interrupt handler; use a debugger to find the source." },
     { 0x000000A8, "Interrupt library", "Inspect the interrupt library's initialization and calls for defects." },
     { 0x000000A9, "Interrupt hardware", "This indicates a physical fault in the interrupt controller." },
     { 0x000000AA, "Interrupt driver", "Check for a corrupted driver file or a defect in the driver's code." },
     { 0x000000AB, "Interrupt configuration", "Review interrupt request numbers and priority levels for errors." },
     { 0x000000AC, "Interrupt initialization", "A resource conflict or a missing device prevented the interrupt system from initializing." },
     { 0x000000AD, "Interrupt enable", "Check for an invalid interrupt number or a hardware fault." },
     { 0x000000AE, "Interrupt disable", "Check for an invalid interrupt number or a hardware fault." },
     { 0x000000AF, "Interrupt unknown", "Use a debugger to trace the cause." },
     
     /* DMA Errors (0xB0-0xBF) */
     { 0x000000B0, "DMA error", "Check for DMA controller hardware faults or a defect in the DMA handling code." },
     { 0x000000B1, "DMA not available", "A resource conflict or hardware issue prevented the DMA channel from being available." },
     { 0x000000B2, "DMA conflict", "Reconfigure hardware or drivers to resolve conflicts on the same DMA channel." },
     { 0x000000B3, "DMA overflow", "A DMA transfer wrote more data than the buffer could hold; this is a buffer overflow." },
     { 0x000000B4, "DMA underflow", "A DMA transfer read more data than the buffer contained; this is a buffer underrun." },
     { 0x000000B5, "DMA priority", "Check the priority levels of pending DMA transfers for conflicts." },
     { 0x000000B6, "DMA channel", "Verify the DMA channel number for validity." },
     { 0x000000B7, "DMA transfer", "A DMA transfer failed; check memory addresses, hardware, and transfer size." },
     { 0x000000B8, "DMA library", "Inspect library initialization and calls for defects." },
     { 0x000000B9, "DMA hardware", "This indicates a physical fault with the DMA controller." },
     { 0x000000BA, "DMA driver", "Check for a corrupted driver file or a defect in the driver's code." },
     { 0x000000BB, "DMA configuration", "Review addresses, transfer sizes, and channel numbers for correctness." },
     { 0x000000BC, "DMA initialization", "A resource conflict or a missing device prevented the DMA system from initializing." },
     { 0x000000BD, "DMA enable", "A configuration error or a hardware fault occurred while enabling a DMA transfer." },
     { 0x000000BE, "DMA disable", "A configuration error or a hardware fault occurred while disabling a DMA transfer." },
     { 0x000000BF, "DMA unknown", "Use a debugger to trace the cause." },
     
     /* Serial/Parallel Port Errors (0xC0-0xCF) */
     { 0x000000C0, "Serial port error", "Check for a hardware issue or a defect in the serial port driver." },
     { 0x000000C1, "Parallel port error", "Check for a hardware issue or a defect in the parallel port driver." },
     { 0x000000C2, "Serial baud rate", "Verify the requested baud rate is supported by the hardware and configuration." },
     { 0x000000C3, "Serial data bits", "Verify the requested data bits are supported by the hardware and configuration." },
     { 0x000000C4, "Serial stop bits", "Verify the requested stop bits are supported by the hardware and configuration." },
     { 0x000000C5, "Serial parity", "Verify the requested parity setting is supported by the hardware and configuration." },
     { 0x000000C6, "Serial flow control", "Check for a mismatch in flow control settings between devices." },
     { 0x000000C7, "Serial buffer", "This is a buffer overflow or underrun; inspect buffer management logic." },
     { 0x000000C8, "Serial timeout", "A slow or unresponsive device caused the operation to time out." },
     { 0x000000C9, "Serial framing", "This indicates a data transmission error; verify communication settings." },
     { 0x000000CA, "Serial overrun", "The CPU could not read data from the serial receiver buffer fast enough." },
     { 0x000000CB, "Serial underrun", "The CPU could not write data to the serial transmitter buffer fast enough." },
     { 0x000000CC, "Serial break", "A break signal was detected on the serial line." },
     { 0x000000CD, "Serial library", "Inspect library initialization and API calls for defects." },
     { 0x000000CE, "Serial hardware", "This indicates a physical fault with the serial port." },
     { 0x000000CF, "Serial unknown", "Use a debugger to trace the cause." },
     
     /* Expansion Errors (0xD0-0xDF) */
     { 0x000000D0, "Expansion error", "Check the physical connections and configuration of the expansion hardware." },
     { 0x000000D1, "Expansion card not found", "Ensure the card is properly seated and powered." },
     { 0x000000D2, "Expansion conflict", "Reconfigure cards to resolve conflicts in memory addresses, I/O ports, or interrupts." },
     { 0x000000D3, "Expansion memory", "A hardware fault on the expansion card is a likely cause." },
     { 0x000000D4, "Expansion I/O", "A hardware fault or a driver defect caused an I/O operation to fail." },
     { 0x000000D5, "Expansion interrupt", "This can be a driver defect or resource conflict; the interrupt was not handled correctly." },
     { 0x000000D6, "Expansion DMA", "Check for DMA conflicts or an invalid memory address involving the card." },
     { 0x000000D7, "Expansion ROM", "A hardware fault on the expansion card's ROM is a likely cause." },
     { 0x000000D8, "Expansion driver", "Check for a corrupted driver file or a defect in the driver's code." },
     { 0x000000D9, "Expansion library", "Inspect library initialization and calls for defects." },
     { 0x000000DA, "Expansion configuration", "Review the expansion card's configuration settings for errors." },
     { 0x000000DB, "Expansion initialization", "A resource conflict or a hardware fault prevented the card from initializing." },
     { 0x000000DC, "Expansion detection", "Check the physical connection and the card's health." },
     { 0x000000DD, "Expansion compatibility", "A version mismatch or hardware limitation caused the card to be incompatible with the system." },
     { 0x000000DE, "Expansion power", "The card is not receiving sufficient power; check the power supply and card's power draw." },
     { 0x000000DF, "Expansion unknown", "Use a debugger to trace the cause." },
     
     /* ROM Errors (0xE0-0xEF) */
     { 0x000000E0, "ROM error", "Check for a hardware issue or a corrupted ROM image." },
     { 0x000000E1, "ROM checksum error", "This indicates a corrupted ROM image; check for a hardware fault or a bad flash." },
     { 0x000000E2, "ROM corruption", "A hardware fault on the ROM chip or a bad flash is a likely cause." },
     { 0x000000E3, "ROM not found", "Ensure the ROM chip is properly seated in its socket." },
     { 0x000000E4, "ROM invalid", "The ROM image format is invalid; verify the ROM is for the correct hardware." },
     { 0x000000E5, "ROM version", "The ROM version is incompatible with the system; check for a version mismatch." },
     { 0x000000E6, "ROM format", "An incorrect ROM image is a likely cause." },
     { 0x000000E7, "ROM access", "A hardware fault or a defect in the access logic caused the ROM access to fail." },
     { 0x000000E8, "ROM library", "A corrupted library or a defect within the library is a likely cause." },
     { 0x000000E9, "ROM hardware", "This indicates a physical fault with the ROM chip." },
     { 0x000000EA, "ROM driver", "Check for a defect in the driver's code." },
     { 0x000000EB, "ROM configuration", "Review ROM configuration settings for errors." },
     { 0x000000EC, "ROM initialization", "A hardware fault prevented the ROM system from initializing." },
     { 0x000000ED, "ROM detection", "Check the physical connection of the ROM chip." },
     { 0x000000EE, "ROM compatibility", "A hardware mismatch caused the ROM to be incompatible with the system." },
     { 0x000000EF, "ROM unknown", "Use a debugger to trace the cause." },
     
     /* Kickstart Errors (0xF0-0xFF) */
     { 0x000000F0, "Kickstart error", "This is a critical system error; check for general system instability." },
     { 0x000000F1, "Kickstart version mismatch", "The Kickstart version is incompatible with installed libraries or hardware; upgrade or downgrade as needed." },
     { 0x000000F2, "Kickstart corruption", "A hardware fault or bad flash is a likely cause of this critical system error." },
     { 0x000000F3, "Kickstart not found", "The Kickstart ROM chip was not detected; the system cannot boot without it." },
     { 0x000000F4, "Kickstart invalid", "The Kickstart ROM image is in an invalid format; ensure the correct ROM is installed." },
     { 0x000000F5, "Kickstart format", "An incorrect ROM image is a likely cause." },
     { 0x000000F6, "Kickstart access", "A critical hardware fault prevented access to the Kickstart ROM." },
     { 0x000000F7, "Kickstart library", "A corrupted library or a defect in a Kickstart-based library is a likely cause." },
     { 0x000000F8, "Kickstart hardware", "This indicates a physical fault with the Kickstart ROM chip." },
     { 0x000000F9, "Kickstart driver", "Check for a defect in the driver's code." },
     { 0x000000FA, "Kickstart configuration", "Review boot sequence settings for errors." },
     { 0x000000FB, "Kickstart initialization", "The Kickstart ROM failed to initialize; the system cannot boot." },
     { 0x000000FC, "Kickstart detection", "Check the physical connection of the Kickstart ROM chip." },
     { 0x000000FD, "Kickstart compatibility", "The Kickstart ROM is incompatible with the system hardware; check for a mismatch." },
     { 0x000000FE, "Kickstart power", "A rare hardware fault where the Kickstart ROM is not receiving sufficient power." },
     { 0x000000FF, "Kickstart unknown", "This is a critical system failure with an unspecified cause." },
     
     /* System Errors (0x100-0x10F) */
     { 0x00000100, "System error", "This can be a defect in the operating system or a critical resource failure." },
     { 0x00000101, "System resource exhausted", "Check for resource leaks, such as memory, file handles, or semaphores." },
     { 0x00000102, "System configuration error", "A corrupted configuration file or incorrect boot settings are a likely cause." },
     { 0x00000103, "System initialization failed", "A hardware fault or a defect in the boot sequence prevented the system from initializing." },
     { 0x00000104, "System shutdown error", "A task is not terminating correctly, causing an error during shutdown." },
     { 0x00000105, "System recovery failed", "This indicates a cascading failure or a persistent defect; reboot the system." },
     { 0x00000106, "System panic", "This is a critical, unrecoverable error; the system will halt or reboot." },
     { 0x00000107, "System crash", "This is a severe, unrecoverable error; use a debugger." },
     { 0x00000108, "System hang", "An infinite loop, a deadlock, or resource starvation is the likely cause." },
     { 0x00000109, "System freeze", "A severe hang caused by a hardware fault or a critical software defect." },
     { 0x0000010A, "System deadlock", "Two or more tasks are blocked, each waiting for a resource held by the other." },
     { 0x0000010B, "System livelock", "Tasks are repeatedly changing state in response to each other without making progress." },
     { 0x0000010C, "System race condition", "The outcome of an operation depends on unpredictable timing; debug for concurrency defects." },
     { 0x0000010D, "System corruption", "A severe defect, such as a buffer overflow or wild pointer, corrupted the OS's internal data structures." },
     { 0x0000010E, "System instability", "The system is behaving erratically; this can precede a crash or hang." },
     { 0x0000010F, "System unknown", "This is a catch-all for severe, unclassified failures; use a debugger." },
     
     /* Exec Library Specific Errors (0x01000000-0x010000FF) - RECOVERY ALERTS */
     { 0x01000001, "68000 exception vector checksum (obs.)", "The exception vector table checksum is invalid; a wild pointer may have corrupted the table." },
     { 0x01000002, "Execbase checksum bad (obs.)", "The checksum of `exec.library` is invalid; the core operating system is corrupted." },
     { 0x01000003, "Library checksum failure", "The checksum of a loaded library is invalid; check for a corrupted file or memory corruption." },
     { 0x01000004, "No memory to make library", "The system could not allocate memory to create a new library; a severe memory shortage is the cause." },
     { 0x01000005, "Corrupt memory list detected in FreeMem()", "The memory allocator's linked list is corrupted; this is a severe heap corruption issue." },
     { 0x01000006, "No memory for interrupt servers", "The system could not allocate memory for a new interrupt handler; this is a critical resource exhaustion error." },
     { 0x01000007, "InitStruct of an APTR source error", "An `InitStruct()` call failed due to an invalid or corrupted source pointer; a defect in the calling application is likely." },
     { 0x01000008, "Semaphore in illegal state at RemSemaphore()", "A semaphore was in an invalid state when `RemSemaphore()` was called; inspect semaphore handling logic." },
     { 0x01000009, "Freeing memory that is already free", "A `FreeMem()` call was made on an already deallocated block; this is a `double-free` defect." },
     { 0x0100000A, "Illegal 680x0 exception taken", "An unhandled exception occurred; a `NULL` pointer dereference, illegal instruction, or bus error is likely." },
     { 0x0100000B, "Attempt to reuse an active IORequest", "An I/O request was reused before the operation completed; check for a missing `WaitIO()` or `AbortIO()` call." },
     { 0x0100000C, "Sanity check on memory list failed during AvailMem()", "The memory list is corrupted; a buffer overflow or wild pointer write is a likely cause." },
     { 0x0100000D, "IO attempted on closed IORequest", "An I/O operation was attempted on a closed device or file handle; this is a `use-after-close` defect." },
     { 0x0100000E, "Stack appears to extend out of range", "A stack overflow or underflow was detected; check for infinite recursion or stack corruption." },
     { 0x0100000F, "Memory header not located", "An attempt was made to free a memory block with an invalid pointer; this is a `wild free` defect." },
     { 0x01000010, "Attempt to use old message semaphores", "An obsolete semaphore API was used; this is a compatibility issue." },
     { 0x010000FF, "Quick interrupt to uninitialized vector", "A quick interrupt occurred on an uninitialized vector; similar to a spurious interrupt." },
 
     /* Exec Library Specific Errors (0x81000000-0x810000FF) - DEADEND ALERTS */
     { 0x81000001, "68000 exception vector checksum", "A fatal checksum error on the exception vector table. Look for wild pointer writes or buffer overflows in interrupt handlers. Check exception vector installation code." },
     { 0x81000002, "Execbase checksum bad", "The core operating system library is corrupted. Look for buffer overflows in memory allocation routines, stack overflows, or heap corruption near exec.library." },
     { 0x81000003, "Library checksum failure", "A loaded library's checksum is invalid. Check for corrupted library files, memory corruption during loading, or buffer overflows in library loading code." },
     { 0x81000004, "No memory to make library", "Severe memory shortage prevented library creation. Look for memory leaks - unfreed memory blocks, unreleased resources, or infinite loops consuming memory." },
     { 0x81000005, "Corrupt memory list", "The memory allocator's list is corrupted. Look for buffer overflows, use-after-free defects, or double-free defects. Check all memory allocation and deallocation calls and array bounds." },
     { 0x81000006, "No memory for interrupt servers", "Critical resource exhaustion for interrupt handlers. Check for memory leaks in interrupt handlers or excessive interrupt registration without cleanup." },
     { 0x81000007, "InitAPtr error", "A fatal InitStruct() failure. Check the source pointer passed to InitStruct() - ensure it's not NULL, corrupted, or uninitialized." },
     { 0x81000008, "Semaphore corrupt", "A semaphore was in a fatal, illegal state. Look for missing ObtainSemaphore() calls, improper ReleaseSemaphore() calls, or uninitialized semaphores." },
     { 0x81000009, "Free twice", "A double-free defect caused fatal memory management error. Look for code calling FreeMem() on the same pointer multiple times or use-after-free defects." },
     { 0x8100000A, "Bogus exception", "An unhandled, critical exception occurred. Look for NULL pointer dereferences, array bounds violations, or corrupted function pointers." },
     { 0x8100000B, "IO used twice", "A reuse of an active I/O request caused fatal error. Look for missing WaitIO() or AbortIO() calls before reusing IORequest structures." },
     { 0x8100000C, "Memory insane", "The memory list is corrupted, causing fatal allocation failure. Look for severe heap corruption from buffer overflows, wild pointer writes, or array bounds violations." },
     { 0x8100000D, "IO after close", "An I/O operation on a closed handle caused fatal error. Look for I/O operations after CloseDevice() calls or improper I/O request lifecycle management." },
     { 0x8100000E, "Stack probe", "A fatal stack overflow or underflow was detected. Look for infinite recursion, excessive stack usage, or large local arrays causing stack overflow." },
     { 0x8100000F, "Bad free address", "A attempt to free an invalid pointer caused unrecoverable error. Look for FreeMem() calls with corrupted pointers, uninitialized pointers, or pointer arithmetic errors." },
     { 0x81000010, "Bad semaphore", "Use of obsolete semaphore API caused fatal compatibility issue. Look for outdated semaphore API usage. Update to current semaphore functions." },
 
     /* Graphics Library Specific Errors (0x02000000-0x0200FFFF) - RECOVERY ALERTS */
     { 0x02010000, "Graphics out of memory", "Check for memory leaks in graphics-heavy applications or general system memory shortage." },
     { 0x02010001, "Monitorspec allocation failed", "A lack of memory or an excessive number of monitors caused this allocation failure." },
     { 0x02010002, "Copperlist memory", "An overly complex Copper list or a memory leak caused insufficient memory for the list." },
     { 0x02010003, "Copperlist full", "A defect in the drawing logic or a hardware limitation caused the Copper list to exceed its maximum size." },
     { 0x02010004, "Copperlist corrupt", "A buffer overflow or a defect in the Copper list generation code caused the corruption." },
     { 0x02010005, "Copperlist header", "This is a memory allocation failure for the Copper list header." },
     { 0x02010006, "Long frame, no memory", "A memory shortage or a request for a very high resolution caused this failure." },
     { 0x02010007, "Short frame, no memory", "This is a simple memory allocation failure for a short frame buffer." },
     { 0x02010008, "Fill TmpRas", "A memory leak in a drawing operation caused insufficient memory for a temporary raster." },
     { 0x02010009, "Text operation failed", "A lack of memory for font glyphs or an invalid font file is the likely cause." },
     { 0x0201000A, "BltBitMap operation failed", "An invalid bitmap handle or insufficient memory caused the blitting operation to fail." },
     { 0x0201000B, "Regions operation failed", "A lack of memory for the region data structure caused this failure." },
     { 0x0201000C, "Graphics new operation failed", "A memory allocation failure or a defect in the graphics library caused this object allocation to fail." },
     { 0x0201000D, "Could not free graphics", "A corrupted object or a `double-free` defect is the likely cause." },
     { 0x0201000E, "MonitorSpec allocation failed", "A lack of memory or an excessive number of monitors caused this allocation failure." },
     { 0x0201000F, "Long frame copper list memory", "A memory shortage or a request for a very high resolution caused this failure." },
     { 0x02010010, "Short frame copper list memory", "This is a simple memory allocation failure for a short frame buffer." },
     { 0x02010011, "Text temporary raster memory", "A memory leak in a drawing operation caused insufficient memory for a temporary raster." },
     { 0x02010012, "BltBitMap memory allocation", "An invalid bitmap handle or insufficient memory caused the blitting operation to fail." },
     { 0x02010013, "Clip regions memory", "A lack of memory for the region data structure caused this failure." },
     { 0x02010014, "MakeVPort memory allocation", "A lack of memory or an invalid viewport configuration caused the `MakeVPort()` call to fail." },
     { 0x02010015, "Graphics memory allocation error", "A memory allocation failure or a defect in the graphics library caused this object allocation to fail." },
     { 0x02010016, "Graphics memory deallocation error", "A corrupted object or a `double-free` defect is the likely cause." },
     { 0x02010017, "Emergency memory not available", "The graphics library could not allocate emergency memory, indicating a severe, system-wide memory shortage." },
     { 0x02010018, "Unsupported font description used", "The font file format is outdated or the file is corrupted." },
     { 0x02010030, "MakeVPort operation failed", "A lack of memory or an invalid viewport configuration caused the `MakeVPort()` call to fail." },
     { 0x0200000C, "Graphics new operation failed", "A memory allocation failure or a defect in the graphics library caused this object allocation to fail." },
     { 0x0200000D, "Could not free graphics", "A corrupted object or a `double-free` defect is the likely cause." },
     { 0x02011234, "Emergency memory not available", "The graphics library could not allocate emergency memory, indicating a severe, system-wide memory shortage." },
     { 0x02000401, "Unsupported font description used", "The font file format is outdated or the file is corrupted." },
     
     /* Graphics Library Specific Errors (0x82000000-0x8200FFFF) - DEADEND ALERTS */
     { 0x82010000, "Graphics out of memory", "A fatal graphics memory allocation failure occurred. Look for memory leaks in graphics operations - check for unfreed BitMaps, RastPorts, or ViewPorts." },
     { 0x82010001, "MonitorSpec alloc", "A fatal error occurred while allocating memory for a monitor specification. Check for memory leaks in monitor handling code or excessive monitor creation without cleanup." },
     { 0x82010006, "Long frame, no memory", "The system could not allocate a long frame buffer due to fatal memory allocation failure. Check for memory leaks in screen creation code or excessive screen resolution requests." },
     { 0x82010007, "Short frame, no memory", "The system could not allocate a short frame buffer due to fatal memory allocation failure. Check for memory leaks in screen creation code or excessive screen creation." },
     { 0x8201000A, "BltBitMap, no memory", "A bitmap blitting operation failed with fatal memory error. Check for memory leaks in bitmap operations or excessive bitmap creation without cleanup." },
     { 0x8201000B, "Region memory", "A fatal region-related operation failed due to lack of memory. Check for memory leaks in region operations or excessive region creation without cleanup." },
     { 0x82010030, "MakeVPort, no memory", "A MakeVPort() call failed fatally due to lack of memory or invalid viewport. Check for memory leaks in viewport creation code or excessive viewport creation." },
     { 0x82011234, "Emergency memory not available", "A fatal system-wide memory shortage prevented graphics library from allocating emergency memory. Check for unfreed resources or infinite loops consuming memory." },
 
     /* Layers Library Errors (0x03000000-0x0300FFFF) */
     { 0x03010000, "Layers out of memory", "Check for memory leaks, or a system-wide memory shortage." },
     { 0x83010000, "Layers out of memory", "A fatal memory shortage occurred when allocating a new layer. Look for memory leaks in layer creation code or excessive layer creation without cleanup." },
 
     /* Intuition Library Errors (0x04000000-0x0400FFFF) - RECOVERY ALERTS */
     { 0x04000001, "Unknown gadget type", "Check for an incorrect gadget type constant or a corrupted gadget structure." },
     { 0x04010002, "Create port operation failed", "This is likely caused by an application creating too many ports without freeing them." },
     { 0x04010003, "Item plane allocation failed", "A lack of memory or an excessive number of menu items caused this failure." },
     { 0x04010004, "Sub allocation failed", "This is a generic memory allocation failure within Intuition." },
     { 0x04010005, "Plane allocation failed", "Insufficient graphics memory is the likely cause." },
     { 0x04010006, "Item box top < RelZero", "A menu item's top coordinate is invalid; check the item's bounding box coordinates." },
     { 0x04010007, "OpenScreen operation failed", "A memory allocation failure or an invalid screen mode caused this to fail." },
     { 0x04010008, "OpenScreen raster allocation failed", "This is a memory allocation failure for the screen's main drawing area." },
     { 0x04000009, "Open system screen failed", "An attempt was made to open an invalid system screen; check screen mode settings." },
     { 0x0401000A, "Add SW gadgets failed", "A memory allocation failure or a defect in the gadget creation code is the likely cause." },
     { 0x0401000B, "Open window operation failed", "A memory allocation failure or an invalid window configuration caused this failure." },
     { 0x0400000C, "Bad State Return entering Intuition", "An internal defect or an improper API call is the likely cause." },
     { 0x0400000D, "Bad Message received by IDCMP", "A defect in the sending application caused a corrupted message to be received." },
     { 0x0400000E, "Weird echo causing incomprehension", "A defect in the message handling code is the likely cause." },
     { 0x0400000F, "Couldn't open the console.device", "A missing driver or a resource conflict is the likely cause." },
     { 0x04000010, "Intuition skipped obtaining a semaphore", "This indicates a race condition defect; Intuition accessed a resource without a required semaphore." },
     { 0x04000011, "Intuition obtained a semaphore in bad order", "Intuition obtained semaphores in an order that could lead to a deadlock; this is a concurrency defect." },
     { 0x04010012, "Bad gadget type", "Unknown gadget type encountered during gadget creation or manipulation." },
     { 0x04010013, "MsgPort creation failed", "This is likely caused by an application creating too many ports without freeing them." },
     { 0x04010014, "MenuItem plane allocation failed", "A lack of memory or an excessive number of menu items caused this failure." },
     { 0x04010015, "MenuSubItem allocation failed", "This is a generic memory allocation failure within Intuition." },
     { 0x04010016, "Plane allocation failed", "Insufficient graphics memory is the likely cause." },
     { 0x04010017, "Item box top < RelZero", "A menu item's top coordinate is invalid; check the item's bounding box coordinates." },
     { 0x04010018, "OpenScreen raster allocation failed", "This is a memory allocation failure for the screen's main drawing area." },
     { 0x04010019, "Unknown system screen type", "An attempt was made to open an invalid system screen; check screen mode settings." },
     { 0x0401001A, "Add software gadgets failed", "A memory allocation failure or a defect in the gadget creation code is the likely cause." },
     { 0x0401001B, "Bad state return entering Intuition", "An internal defect or an improper API call is the likely cause." },
     { 0x0401001C, "Bad message received by IDCMP", "A defect in the sending application caused a corrupted message to be received." },
     { 0x0401001D, "Weird echo causing incomprehension", "A defect in the message handling code is the likely cause." },
     { 0x0401001E, "Console device open failed", "A missing driver or a resource conflict is the likely cause." },
 
     /* Intuition Library Errors (0x84000000-0x8400FFFF) - DEADEND ALERTS */
     { 0x84000001, "Unknown gadget type", "An unrecoverable error occurred when trying to create a gadget of an unknown type. Check for invalid gadget type constants or corrupted gadget structures." },
     { 0x84010002, "Create port failed", "A critical memory allocation failure occurred when trying to create a port. Look for memory leaks in port creation code or excessive port creation without cleanup." },
     { 0x84010003, "Item plane allocation failed", "A fatal memory error occurred while allocating memory for a menu item plane. Check for memory leaks in menu creation code or excessive menu items." },
     { 0x84010004, "Sub allocation failed", "A critical sub-allocation operation within Intuition failed. Look for memory leaks in Intuition operations or excessive resource creation." },
     { 0x84010005, "Plane allocation failed", "A fatal memory allocation error occurred for a graphics plane. Check for insufficient graphics memory or memory leaks in graphics operations." },
     { 0x84010006, "Item box top < RelZero", "A menu item's top coordinate was critically invalid. Check menu item bounding box coordinates and layout calculations." },
     { 0x84010007, "OpenScreen failed", "A fatal error occurred while trying to open a screen due to memory or invalid screen mode. Check for memory leaks in screen creation or invalid screen mode requests." },
     { 0x84010008, "OpenScreen raster allocation failed", "A critical memory allocation failure occurred for the screen's raster. Check for memory leaks in screen creation or excessive screen resolution requests." },
     { 0x84000009, "Open system screen failed", "A fatal error occurred when trying to open an invalid system screen. Check screen mode settings and system screen configuration." },
     { 0x8401000A, "Add SW gadgets failed", "An unrecoverable error occurred when adding a software gadget. Check for memory leaks in gadget creation or corrupted gadget structures." },
     { 0x8401000B, "Open window failed", "A fatal error occurred while trying to open a window. Check for memory leaks in window creation or invalid window configuration." },
     { 0x8400000C, "Bad state return entering Intuition", "The Intuition library was entered in a fatal, invalid state. Check for improper API call sequences or corrupted library state." },
     { 0x8400000D, "Bad message received by IDCMP", "A corrupted message was received by the IDCMP, causing an unrecoverable error. Check for defects in the sending application or corrupted message structures." },
     { 0x8400000E, "Weird echo causing incomprehension", "A fatal defect in message handling code caused an invalid echo message. Check message handling logic for defects or corrupted message processing." },
     { 0x8400000F, "Couldn't open console device", "A fatal error occurred when trying to open the console device. Check for missing drivers, resource conflicts, or console device configuration issues." },
     
     /* Math Library Errors (0x05000000-0x0500FFFF) */
     { 0x05000001, "Math library out of memory", "Check for a memory leak or a large calculation causing a memory shortage." },
     { 0x05000002, "Math overflow error", "Check for large input values that cause the result to exceed representable limits." },
     { 0x05000003, "Math underflow error", "Check for very small input values that cause the result to be too small to be represented." },
     { 0x05000004, "Math division by zero", "Validate all divisors to ensure they are not zero." },
     { 0x05000005, "Math invalid operation", "Debug for invalid operands or a program defect." },
     { 0x05000006, "Math library", "A critical, unrecoverable error occurred in the math library." },
 
     /* Clist Library Errors (0x06000000-0x0600FFFF) */
     { 0x06000001, "Clist library out of memory", "A memory leak or a large number of clist operations caused a memory shortage." },
     { 0x06000002, "Invalid clist operation", "Check the function call for a defect." },
     { 0x86000001, "Clist library out of memory", "A fatal memory shortage occurred when allocating clist memory. Look for memory leaks in clist operations or excessive clist creation without cleanup." },
     { 0x86000002, "Invalid clist operation", "An unrecoverable operation was attempted on the clist library. Check the function call for defects or invalid parameters." },
 
     /* DOS Library Errors (0x07000000-0x0700FFFF) */
     { 0x07010001, "No memory at startup", "A severe memory shortage prevented the DOS library from starting up." },
     { 0x07000002, "EndTask didn't end", "An infinite loop or an unreleased resource caused a task to fail termination." },
     { 0x07000003, "Qpkt failure", "A memory allocation failure or a defect in the packet handling code is the likely cause." },
     { 0x07000004, "Unexpected packet received", "A defect in the message handling code or a corrupted packet is the likely cause." },
     { 0x07000005, "Freevec operation failed", "A corrupted vector table or a defect in the vector handling code is the likely cause." },
     { 0x07000006, "Disk block sequence error", "The logical block order on the disk is inconsistent; this indicates file system corruption." },
     { 0x07000007, "Bitmap corrupt", "The file system's free block bitmap is corrupted; this is a serious file system error." },
     { 0x07000008, "Key already free", "A file system key was found to be free when it should have been in use, indicating corruption." },
     { 0x07000009, "Invalid checksum", "A checksum verification failed on a data block, indicating data corruption." },
     { 0x0700000A, "Disk error", "A hardware failure, a bad sector, or file system corruption is a likely cause." },
     { 0x0700000B, "Key out of range", "A file system key was found to be outside the valid range, indicating corruption." },
     { 0x0700000C, "Bad overlay", "An overlay file is corrupted; this can be a file system error or a defect in the loading code." },
     { 0x0700000D, "Invalid init packet for cli/shell", "A corrupted startup packet for the command line interface is the likely cause." },
     { 0x0700000E, "Filehandle closed more than once", "A file handle was closed more than once; this is a `double-close` defect." },
     { 0x0700000F, "QueuePacket failed", "A memory allocation failure or a defect in the packet handling code is the likely cause." },
     { 0x07000010, "Async packet received", "A defect in the message handling code or a corrupted packet is the likely cause." },
     { 0x07000011, "FreeVec failed", "A corrupted vector table or a defect in the vector handling code is the likely cause." },
     { 0x07000012, "Disk block sequence error", "The logical block order on the disk is inconsistent; this indicates file system corruption." },
     { 0x07000013, "Bitmap corrupt", "The file system's free block bitmap is corrupted; this is a serious file system error." },
     { 0x07000014, "Key already free", "A file system key was found to be free when it should have been in use, indicating corruption." },
     { 0x07000015, "Invalid checksum", "A checksum verification failed on a data block, indicating data corruption." },
     { 0x07000016, "Disk error", "A hardware failure, a bad sector, or file system corruption is a likely cause." },
     { 0x07000017, "Key out of range", "A file system key was found to be outside the valid range, indicating corruption." },
     { 0x07000018, "Bad overlay", "An overlay file is corrupted; this can be a file system error or a defect in the loading code." },
     { 0x07000019, "Invalid init packet for cli/shell", "A corrupted startup packet for the command line interface is the likely cause." },
     { 0x0700001A, "Filehandle closed more than once", "A file handle was closed more than once; this is a `double-close` defect." },
     { 0x87000002, "EndTask didn't end", "A task failed to terminate correctly, causing a fatal error. Look for infinite loops, unreleased resources, or improper task cleanup." },
     { 0x87000003, "Qpkt failure", "A fatal packet queuing failure occurred. Check for memory leaks in packet handling or corrupted packet structures." },
     { 0x87000004, "Unexpected packet received", "A corrupted or unexpected packet caused a fatal error. Check for defects in message handling code or corrupted packet data." },
     { 0x87000005, "Freevec failed", "A fatal attempt to free an exception vector failed. Check for corrupted vector tables or invalid vector addresses." },
     { 0x87000006, "Disk block sequence error", "The logical block order on the disk is inconsistent, indicating fatal file system corruption. Check for hardware issues, corrupted filesystems, or improper disk operations." },
     { 0x87000007, "Bitmap corrupt", "The file system's free block bitmap is corrupted, leading to a fatal error. Check for hardware issues, corrupted filesystems, or improper disk operations." },
     { 0x87000008, "Key already free", "A file system key was in a fatal, illegal state. Check for filesystem corruption or defects in file system operations." },
     { 0x87000009, "Invalid checksum", "A checksum verification failed on a data block, indicating a fatal data corruption error. Check for hardware issues, corrupted data, or improper data handling." },
     { 0x8700000A, "Disk error", "A general disk I/O error occurred, leading to a fatal crash. Check for hardware issues, corrupted filesystems, or improper disk operations." },
     { 0x8700000B, "Key out of range", "A file system key was found to be fatally outside the valid range. Check for filesystem corruption or defects in file system operations." },
     { 0x8700000C, "Bad overlay", "An overlay file is corrupted, causing an unrecoverable error. Check for corrupted overlay files or defects in overlay loading code." },
     { 0x8700000D, "Invalid init packet for cli/shell", "A corrupted startup packet for the command line interface caused an unrecoverable error. Check for corrupted startup configuration or defects in CLI initialization." },
     { 0x8700000E, "Filehandle closed more than once", "A double-close defect on a file handle caused an unrecoverable error. Look for code that closes file handles multiple times or improper file handle lifecycle management." },
 
     /* RAM Library Errors (0x08000000-0x0800FFFF) */
     { 0x08000001, "Overlays are illegal for library segments", "An attempt was made to use an overlay with a library segment; this is an unsupported operation." },
     { 0x08000002, "Bad segment list", "Overlay detected in library seglist; this is an unsupported operation." },
     
     /* Icon Library Errors (0x09000000-0x0900FFFF) */
     { 0x09000001, "Icon library out of memory", "A memory leak or a large number of icons caused a memory shortage." },
     { 0x09000002, "Invalid icon format", "The icon file is in an invalid format; check for a corrupted file or an unsupported format." },
     { 0x09000003, "Icon data corrupted", "A defect in the icon loading code or a file system error is a likely cause." },
     { 0x09000004, "Icon library", "A critical, unrecoverable error occurred in the icon library." },
     
     /* Expansion Library Errors (0x0A000000-0x0A00FFFF) */
     { 0x0A000001, "Freed free region", "An attempt was made to free a region of expansion memory that was already deallocated; this is a `double-free` defect." },
     
     /* DiskFont Library Errors (0x0B000000-0x0B00FFFF) */
     { 0x0B000001, "DiskFont library out of memory", "A memory leak or a large number of fonts caused a memory shortage." },
     { 0x0B000002, "Invalid font format", "The font file is in an invalid format; check for a corrupted file or an unsupported format." },
     { 0x0B000003, "Font data corrupted", "A defect in the font loading code or a file system error is a likely cause." },
     
     /* Utility Library Errors (0x0C000000-0x0C00FFFF) */
     { 0x0C000001, "Utility library out of memory", "A memory leak or a large number of utility operations caused a memory shortage." },
     { 0x0C000002, "Invalid utility operation", "An invalid operation was attempted on the utility library. Check the function call." },
     
     /* Audio Library Errors (0x10000000-0x1000FFFF) */
     { 0x10000001, "Audio library out of memory", "A memory leak or a large audio file caused a memory shortage." },
     { 0x10000002, "Invalid audio operation", "Check the function call for a defect." },
     { 0x10000003, "Audio device error", "A hardware fault or a defect in the driver is a likely cause." },
     
     /* Console Device Errors (0x11000000-0x1100FFFF) */
     { 0x11000001, "Console can't open initial window", "A graphics memory issue or an Intuition library defect is preventing the console from opening." },
     { 0x11000002, "Console can't open window", "A graphics memory issue or an Intuition library defect is preventing the console from opening." },
     
     /* GamePort Library Errors (0x12000000-0x1200FFFF) */
     { 0x12000001, "GamePort library out of memory", "A memory leak or a large number of GamePort operations caused a memory shortage." },
     { 0x12000002, "Invalid GamePort operation", "An invalid operation was attempted on the GamePort library. Check the function call." },
     
     /* Keyboard Library Errors (0x13000000-0x1300FFFF) */
     { 0x13000001, "Keyboard library out of memory", "A memory leak is a likely cause." },
     { 0x13000002, "Invalid keyboard operation", "An invalid operation was attempted on the keyboard library. Check the function call." },
     
     /* Trackdisk Device Errors (0x14000000-0x1400FFFF) */
     { 0x14000001, "Calibrate seek error occurred", "A hardware issue with the floppy disk drive mechanism is a likely cause." },
     { 0x14000002, "Delay error on timer wait", "A defect in the timer device or a hardware issue is a likely cause." },
     { 0x14000003, "Seek error during calibration", "A hardware issue with the floppy disk drive mechanism is a likely cause." },
     { 0x14000004, "Error on timer wait", "A defect in the timer device or a hardware issue is a likely cause." },
     
     /* Timer Device Errors (0x15000000-0x1500FFFF) */
     { 0x15000001, "Bad request", "Check the I/O request structure and parameters for errors." },
     { 0x15000002, "Power supply error", "A hardware fault or a power quality issue is preventing proper 50/60Hz ticks." },
     { 0x15000003, "Bad timerequest", "Check the I/O request structure and parameters for errors." },
     { 0x15000004, "Bad power supply", "A hardware fault or a power quality issue is preventing proper 50/60Hz ticks." },
     
     /* CIA Library Errors (0x20000000-0x2000FFFF) */
     { 0x20000001, "CIA library out of memory", "A memory leak is a likely cause." },
     { 0x20000002, "Invalid CIA operation", "Check the function call for a defect." },
     
     /* Disk Resource Errors (0x21000000-0x2100FFFF) */
     { 0x21000001, "Get unit failed", "A resource conflict or a defect in the resource management code is the likely cause." },
     { 0x21000002, "Interrupt error", "A driver defect or an interrupt conflict is preventing the interrupt from being handled correctly." },
     { 0x21000003, "GetUnit already has disk", "A resource conflict or a defect in the resource management code is the likely cause." },
     { 0x21000004, "No active unit for interrupt", "A driver defect or an interrupt conflict is preventing the interrupt from being handled correctly." },
     
     /* Misc Library Errors (0x22000000-0x2200FFFF) */
     { 0x22000001, "Misc library out of memory", "A memory leak is a likely cause." },
     { 0x22000002, "Invalid misc operation", "Check the function call for a defect." },
     
     /* Expansion Library Errors (0x0A000000-0x0A00FFFF) */
     { 0x0A000001, "Freed free region", "An attempt was made to free a region of expansion memory that was already deallocated; this is a `double-free` defect." },
     { 0x0A000002, "Expansion memory error", "A hardware fault on the expansion card is a likely cause." },
     { 0x0A000003, "Expansion I/O error", "A hardware fault or a driver defect caused an I/O operation to fail." },
     { 0x0A000004, "Expansion interrupt error", "This can be a driver defect or resource conflict; the interrupt was not handled correctly." },
     { 0x0A000005, "Expansion DMA error", "Check for DMA conflicts or an invalid memory address involving the card." },
     { 0x0A000006, "Expansion ROM error", "A hardware fault on the expansion card's ROM is a likely cause." },
     { 0x0A000007, "Expansion driver error", "Check for a corrupted driver file or a defect in the driver's code." },
     { 0x0A000008, "Expansion library error", "Inspect library initialization and calls for defects." },
     { 0x0A000009, "Expansion configuration error", "Review the expansion card's configuration settings for errors." },
     { 0x0A00000A, "Expansion initialization error", "A resource conflict or a hardware fault prevented the card from initializing." },
     { 0x0A00000B, "Expansion detection error", "Check the physical connection and the card's health." },
     { 0x0A00000C, "Expansion compatibility error", "A version mismatch or hardware limitation caused the card to be incompatible with the system." },
     { 0x0A00000D, "Expansion power error", "The card is not receiving sufficient power; check the power supply and card's power draw." },
     
     /* Bootstrap Errors (0x30000000-0x3000FFFF) */
     { 0x30000001, "Boot code returned an error", "This is a critical, unrecoverable error; check for a corrupted bootblock." },
     { 0x30000002, "Bootstrap error", "A critical, unrecoverable error occurred during system bootstrap; check for a corrupted bootblock." },
     
     /* Workbench Errors (0x31000000-0x3100FFFF) */
     { 0x31000001, "Workbench bad startup message 1", "A defect in the application sending the message is a likely cause." },
     { 0x31000002, "Workbench bad startup message 2", "A defect in the application sending the message is a likely cause." },
     { 0x31000003, "Workbench bad IO message", "A defect in the sending application is the likely cause." },
     { 0x31010004, "Workbench init potion alloc drawer failed", "This is a memory allocation failure." },
     { 0x31010005, "Workbench create menus 1 failed", "A memory allocation failure or a defect in the menu creation code is the likely cause." },
     { 0x31010006, "Workbench create menus 2 failed", "A memory allocation failure or a defect in the menu creation code is the likely cause." },
     { 0x31010007, "Workbench layout menus failed", "A defect in the layout code or a corrupted menu structure is the likely cause." },
     { 0x31010008, "Workbench add tool menu item failed", "A memory allocation failure is the likely cause." },
     { 0x31010009, "Workbench re-layout tool menu failed", "A defect in the layout code is the likely cause." },
     { 0x3101000A, "Workbench init timer failed", "A hardware or driver issue is a likely cause." },
     { 0x3101000B, "Workbench init layer demon failed", "This is an internal defect." },
     { 0x3101000C, "Workbench init WB gels failed", "A memory allocation failure is the likely cause." },
     { 0x3101000D, "Workbench init screen and windows 1 failed", "A critical graphics initialization error occurred." },
     { 0x3101000E, "Workbench init screen and windows 2 failed", "A critical graphics initialization error occurred." },
     { 0x3101000F, "Workbench init screen and windows 3 failed", "A critical graphics initialization error occurred." },
     { 0x31010010, "Workbench memory allocation failed", "A severe memory shortage prevented Workbench from allocating a critical resource." },
     { 0x31010011, "Workbench bootstrap", "A critical, unrecoverable error occurred during Workbench bootstrap." },
     { 0x31010012, "Workbench", "A critical, unrecoverable error occurred in Workbench." },
     { 0x31010013, "DiskCopy", "A critical, unrecoverable error occurred in DiskCopy." },
     { 0x31010014, "GadTools library", "A critical, unrecoverable error occurred in the GadTools library." },
     { 0x31010015, "Utility library", "A critical, unrecoverable error occurred in the utility library." },
     { 0x31010016, "Unknown", "An unknown, unrecoverable error occurred." },
     { 0xB1000001, "Workbench no fonts", "Workbench failed to load required fonts, causing an unrecoverable error. Check for missing font files, corrupted fonts, or font loading configuration issues." },
     { 0xB1010004, "Workbench init potion alloc drawer failed", "Workbench failed to allocate memory for a drawer, causing a fatal memory allocation failure. Check for memory leaks in Workbench or system-wide memory shortage." },
     { 0xB1010009, "Workbench re-layout tool menu failed", "A fatal defect in the layout code caused Workbench to fail re-laying out a menu. Check for corrupted menu structures or layout calculation defects." },
     { 0xB101000A, "Workbench init timer failed", "Workbench failed to initialize a timer with an unrecoverable hardware or driver issue. Check for timer device conflicts or corrupted timer drivers." },
     { 0xB101000B, "Workbench init layer demon failed", "A fatal internal defect occurred when Workbench tried to initialize a layer demon. Check for graphics library corruption or layer system defects." },
     { 0xB101000C, "Workbench init WB gels failed", "Workbench failed to initialize its gels (graphical elements) with a fatal memory allocation failure. Check for memory leaks in Workbench or graphics memory shortage." },
     { 0xB101000D, "Workbench init screen and windows 1 failed", "Workbench failed to create its screen and windows due to a critical graphics initialization error. Check for graphics memory shortage or invalid screen mode requests." },
     { 0xB101000E, "Workbench init screen and windows 2 failed", "Workbench failed to create its screen and windows due to a critical graphics initialization error. Check for graphics memory shortage or invalid screen mode requests." },
     { 0xB101000F, "Workbench init screen and windows 3 failed", "Workbench failed to create its screen and windows due to a critical graphics initialization error. Check for graphics memory shortage or invalid screen mode requests." },
     { 0xB1010010, "Workbench memory allocation failed", "Workbench could not allocate memory for a critical resource, causing a severe memory shortage. Check for memory leaks across the system or resource exhaustion." },
 
     /* DiskCopy Library Errors (0x32000000-0x3200FFFF) */
     { 0x32000001, "DiskCopy library out of memory", "A memory leak or a large number of DiskCopy operations caused a memory shortage." },
     { 0x32000002, "Invalid DiskCopy operation", "Check the function call for a defect." },
     
     /* GadTools Library Errors (0x33000000-0x3300FFFF) */
     { 0x33000001, "GadTools library out of memory", "A memory leak or a large number of gadgets caused a memory shortage." },
     { 0x33000002, "Invalid GadTools operation", "Check the function call for a defect." },
     
     /* CPU Exceptions with Deadend Bit Set (0x80000000-0x8000FFFF) */
     { 0x80000002, "Bus Error", "A fatal hardware bus fault occurred. Look for memory access violations, corrupted pointers, or hardware memory faults." },
     { 0x80000003, "Address Error", "An illegal address access occurred. Look for unaligned memory access, corrupted pointers, or invalid memory addresses." },
     { 0x80000004, "Illegal Instruction", "An illegal CPU instruction was encountered. Look for corrupted code, wild jumps into data, or corrupted function pointers." },
     { 0x80000005, "Division by Zero", "An integer division by zero was attempted. Look for uninitialized variables, missing validation, or corrupted divisor values." },
     { 0x80000006, "CHK Instruction", "A bounds check failed. Look for array bounds violations, loop boundary errors, or corrupted array indices." },
     { 0x80000007, "TRAPV Instruction", "An overflow was detected. Look for arithmetic operations that exceed integer limits or missing overflow checks." },
     { 0x80000008, "Privilege Violation", "A program attempted to execute a privileged instruction. Look for code running in wrong privilege mode or corrupted instruction pointers." },
     { 0x80000009, "Trace", "A trace exception occurred. Look for debugging code left in production or corrupted trace flags." },
    { 0x8000000A, "Line 1010 Emulator", "An FPU instruction was run without coprocessor, most likely to be caused by executing data as code." },
    { 0x8000000B, "Line 1111 Emulator", "An FPU instruction was run without coprocessor, most likely to be caused by executing data as code." },
     { 0x8000000E, "Stack frame format error", "A corrupted stack frame caused a fatal error. Look for stack corruption, buffer overflows, or corrupted return addresses." },
     { 0x80000018, "Spurious interrupt error", "An unexpected and unhandled interrupt occurred. Look for missing interrupt handlers, corrupted interrupt vectors, or hardware glitches." },
     { 0x80000019, "Autovector Level 1", "An interrupt was received, but no handler was installed. Look for missing interrupt handler installation or corrupted interrupt vector table." },
     { 0x8000001A, "Autovector Level 2", "An interrupt was received, but no handler was installed. Look for missing interrupt handler installation or corrupted interrupt vector table." },
     { 0x8000001B, "Autovector Level 3", "An interrupt was received, but no handler was installed. Look for missing interrupt handler installation or corrupted interrupt vector table." },
     { 0x8000001C, "Autovector Level 4", "An interrupt was received, but no handler was installed. Look for missing interrupt handler installation or corrupted interrupt vector table." },
     { 0x8000001D, "Autovector Level 5", "An interrupt was received, but no handler was installed. Look for missing interrupt handler installation or corrupted interrupt vector table." },
     { 0x8000001E, "Autovector Level 6", "An interrupt was received, but no handler was installed. Look for missing interrupt handler installation or corrupted interrupt vector table." },
     { 0x8000001F, "Autovector Level 7", "An interrupt was received, but no handler was installed. Look for missing interrupt handler installation or corrupted interrupt vector table." },
     
     /* End marker */
     { 0xFFFFFFFF, "End of table", "End marker for error table." }
  };
  
  /* Number of entries in the error table */
  const ULONG ERROR_TABLE_SIZE = sizeof(errorTable) / sizeof(struct ErrorInfo);
  
   /*
   * Look up error code in the table
   */
  struct ErrorInfo* GainInsight(ULONG errorCode)
  {
      ULONG i;
      struct ErrorInfo *expandedError;
      STRPTR expandedExplanation;

      for (i = 0; i < ERROR_TABLE_SIZE - 1; i++) {
          if (errorTable[i].code == errorCode) {
              /* Allocate fresh memory for the expanded error info */
              expandedError = (struct ErrorInfo *)malloc(sizeof(struct ErrorInfo));
              if (expandedError == NULL) {
                  /* Memory allocation failed - return NULL */
                  return NULL;
              }
              
              /* Expand the explanation */
              expandedExplanation = ExpandExplanation(errorTable[i].insight);
              if (expandedExplanation == NULL) {
                  /* Expansion failed - free allocated memory and return NULL */
                  free(expandedError);
                  return NULL;
              }
              
              /* Allocate fresh memory for the expanded explanation string */
              expandedError->insight = strdup(expandedExplanation);
              if (expandedError->insight == NULL) {
                  /* String allocation failed - free allocated memory and return NULL */
                  free(expandedError);
                  return NULL;
              }
              
              /* Copy the other fields (these are static strings, so just copy pointers) */
              expandedError->code = errorTable[i].code;
              expandedError->description = errorTable[i].description;
              
              return expandedError;
          }
      }
      
      /* Return NULL if not found */
      return NULL;
  }
  
  /*
   * Free error info structure allocated by LookupError
   * Call this when you're done with the error info to prevent memory leaks
   */
  VOID FreeErrorInfo(struct ErrorInfo *errorInfo)
  {
      if (errorInfo != NULL) {
          /* Free the dynamically allocated explanation string */
          if (errorInfo->insight != NULL) {
              free((void *)errorInfo->insight);
          }
          
          /* Free the error info structure itself */
          free(errorInfo);
      }
  }
  
 /*
   * Expand RLE tokens in explanations
   */
   STRPTR ExpandExplanation(STRPTR explanation) {
    static char expandedBuffer[2048];
    char *src = explanation;
    char *dst = expandedBuffer;
    char token_name[8];
    int token_len;
    STRPTR expanded;
    const char *start_of_name;

    memset(expandedBuffer, 0, sizeof(expandedBuffer));

    while (*src && (dst - expandedBuffer) < sizeof(expandedBuffer) - 1) {
        if (strncmp(src, "TOK_", 4) == 0) {
            token_len = 0;
            start_of_name = src + 4;
            
            // C89-compliant check for digits and underscore
            while (token_len < 7 && start_of_name[token_len] != '\0' && 
                   ((start_of_name[token_len] >= '0' && start_of_name[token_len] <= '9') || 
                    start_of_name[token_len] == '_')) {
                token_name[token_len] = start_of_name[token_len];
                token_len++;
            }
            token_name[token_len] = '\0';
            
            // Advance the source pointer by the full token length, regardless of validity
            src += 4 + token_len;

            // Handle the case where a token name was not successfully extracted.
            if (token_len == 0) {
                // Not a valid token, just copy the "TOK_" prefix and continue
                if ((dst - expandedBuffer) + 4 < sizeof(expandedBuffer) - 1) {
                    memcpy(dst, "TOK_", 4);
                    dst += 4;
                }
                continue;
            }

            // Look up and expand token
            expanded = LookupToken(token_name);
            if (expanded) {
                // Copy expanded string and update destination pointer
                size_t exp_len = strlen(expanded);
                if ((dst - expandedBuffer) + exp_len < sizeof(expandedBuffer) - 1) {
                    memcpy(dst, expanded, exp_len);
                    dst += exp_len;
                }
            } else {
                // Token not found, copy original token string
                if ((dst - expandedBuffer) + 4 + token_len < sizeof(expandedBuffer) - 1) {
                    memcpy(dst, "TOK_", 4);
                    dst += 4;
                    memcpy(dst, token_name, token_len);
                    dst += token_len;
                }
            }
        } else {
            // Copy regular character
            *dst++ = *src++;
        }
    }
    
    *dst = '\0';
    
    return expandedBuffer;
  }
  
  /*
   * Token lookup table for RLE expansion
   * This table must match the tokens generated by compress_error_codes.py
   */
  static struct {
      const char *name;
      const char *value;
  } tokenTable[] = {
     { "TOK_01", "Check for memory leaks" },
     { "TOK_02", "Check for memory leaks in" },
     { "TOK_03", "memory allocation failure" },
     { "TOK_04", "is the likely cause. A" },
     { "TOK_05", "debugger to trace the" },
     { "TOK_06", "Use a debugger to trace" },
     { "TOK_07", "a debugger to trace the" },
     { "TOK_08", "Use a debugger to trace the" },
     { "TOK_09", "debugger to trace the cause." },
     { "TOK_10", "a debugger to trace the cause." },
     { "TOK_11", "Use a debugger to trace the cause." },
     { "TOK_12", "A resource conflict or" },
     { "TOK_13", "for missing interrupt" },
     { "TOK_14", "trace the cause. Check" },
     { "TOK_15", "to trace the cause. Check" },
     { "TOK_16", "memory leak or a large" },
     { "TOK_17", "debugger to trace the cause. Check" },
     { "TOK_18", "a debugger to trace the cause. Check" },
     { "TOK_19", "Use a debugger to trace the cause. Check" },
     { "TOK_20", "interrupt vector table" },
     { "TOK_21", "defect in the driver's" },
     { "TOK_22", "in the driver's code." },
     { "TOK_23", "A memory allocation failure" },
     { "TOK_24", "caused a memory shortage." },
     { "TOK_25", "Look for missing interrupt" },
     { NULL, NULL }
  };
  
  /*
   * Look up token value by name
   */
  STRPTR LookupToken(const char *tokenName)
  {
      int i;
      
      for (i = 0; tokenTable[i].name != NULL; i++) {
          if (strcmp(tokenTable[i].name, tokenName) == 0) {
              return tokenTable[i].value;
          }
      }
      
      return NULL;
  }
