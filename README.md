# Insight

After meditation comes... Insight

Insight is a comprehensive Guru Meditation error checker and analyzer for Amiga systems. It provides detailed explanations and troubleshooting guidance for system errors, helping developers and users understand and resolve system crashes.

## About Insight

Insight automatically detects and analyzes Guru Meditation errors from the Amiga system. When launched from Workbench, it automatically checks the system's LastAlert array for any recent errors and displays them in a user-friendly dialog.

### Key Features

- **Automatic Error Detection** - Checks system LastAlert array for recent errors
- **Comprehensive Error Database** - 200+ predefined error codes with detailed explanations
- **User-Friendly Interface** - Reaction-based requester.class GUI with clear error dialogs
- **Command-Line Support** - Manual error code analysis and testing
- **Guru Mode** - Become a _guru_ by studying random errors from the database

## Operating Modes

### Workbench Mode (Automatic)
When you double-click the Insight icon in Workbench, the application automatically:
- Checks the system's LastAlert array for recent errors
- Displays any reason found for the last session to Alert, in a requester
- Provides detailed insight on the possible causes of the Alert
- Exits after showing the error information, or silently if nothing to show

### Command Line Mode (Manual)
You can also run Insight from the command line with various options:

```bash
# Basic Usage
Insight [ERROR/K=code] [GURU/S] [VERBOSE/S]

# Parameters
ERROR=0xXXXXXXXX  # Analyze a specific error code
GURU              # Show a random error
VERBOSE           # Enable detailed debug output

# Examples
Insight ERROR=0x80000004
Insight GURU
Insight VERBOSE
```

## Error Categories

Insight provides comprehensive coverage of Amiga system errors:

- **CPU Exceptions (0x01-0x1F)** - Task held, bus error, illegal instruction, etc.
- **File System Errors (0x20-0x2F)** - Disk read/write, file not found, permissions
- **Floating Point Errors (0x30-0x37)** - FPU exceptions, divide by zero, overflow
- **Memory Protection (0x80-0x8F)** - Access violations, protection faults
- **System Errors (0x100-0x10F)** - Resource exhaustion, configuration errors
- **Library-Specific Errors** - Exec, Graphics, Intuition, DOS, Workbench
- **Deadend Alerts (0x80000000+)** - Fatal, unrecoverable errors

## Building from Source

### Requirements
- SAS/C compiler 6.58
- NDK 3.2
- ToolKit

### Build Commands
```bash
# Using SMakefile
cd Source/
smake Insight
smake install ;Will copy Insight to the SDK/Tools drawer in the project directory
```

## Installation

1. Find the Insight executable and matching icon in SDK/Tools/ in this distribution
2. Place in `SYS:WBStartup/` or your preferred location. If installed in WBStartup, it will run on boot, doing nothing unless there was a LastAlert from the previous session crashing

## ChangeLog

### Version 47.3 (27.08.2025)

- Fixed reading of LastAlert finally and properly 
- Removed debugging output

### Version 47.2 (26.08.2025)

- Fixed what could be considered a fairly serious bug in that reading LastAlert from SysBase was corrupted
- Added additional error codes for some 

### Version 47.1 (17.08.2025)
- Initial release
- Comprehensive error database (200+ codes)
- Workbench and command-line modes
- Reaction-based GUI

## About [amigazen project](http://www.amigazen.com)

*A web, suddenly*

*Forty years meditation*

*Minds awaken, free*

**amigazen project** uses modern software development tools and methods to update and rerelease classic Amiga open source software. Our upcoming releases include a new AWeb, and a new Amiga Python 2.

Key to our approach is ensuring every project can be built with the same common set of development tools and configurations, so we created the ToolKit project to provide a standard configuration for Amiga development. All *amigazen project* releases will be guaranteed to build against the ToolKit standard so that anyone can download and begin contributing straightaway without having to tailor the toolchain for their own setup.

Our philosophy is based on openness:

*Open* to anyone and everyone	- *Open* source and free for all	- *Open* your mind and create!

PRs for all of our projects are gratefully received at [GitHub](https://github.com/amigazen/). While our focus now is on classic 68k software, we do intend that all amigazen project releases can be ported to other Amiga-like systems including AROS and MorphOS where feasible.

## About ToolKit

**ToolKit** exists to solve the problem that most Amiga software was written in the 1980s and 90s, by individuals working alone, each with their own preferred setup for where their dev tools are run from, where their include files, static libs and other toolchain artifacts could be found, which versions they used and which custom modifications they made. Open source collaboration did not exist then as we know it now. 

**ToolKit** from amigazen project is a work in progress to make a standardised installation of not just the Native Developer Kit, but the compilers, build tools and third party components needed to be able to consistently build projects in collaboration with others, without each contributor having to change build files to work with their particular toolchain configuration. 

All *amigazen project* releases will release in a ready to build configuration according to the ToolKit standard.

Each component of **ToolKit** is open source and like *Insight* here will have it's own github repo, while ToolKit itself will be released at https://github.com/amigazen/ToolKit as an easy to install package containing the redistributable components, as well as scripts to easily install the parts that are not freely redistributable from archive.

## Contact 

- At GitHub https://github.com/amigazen/Insight
- on the web at http://www.amigazen.com/toolkit/ (Amiga browser compatible)
- or email toolkit@amigazen.com

## Acknowledgements

*Amiga* is a trademark of **Amiga Inc**. 

Inspired by the original LastAlert2 