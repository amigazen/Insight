## Building from Source

### Requirements
- SAS/C compiler 6.58
- NDK 3.2
- ToolKit

### Build Commands
```bash
; Using SMakefile
cd Source/
smake Insight

smake install ; Will copy Insight to the SDK/Tools drawer in the project directory

smake clean ; Will clean the local project folder of build artifacts
```

## Installation

1. Find the Insight executable and matching icon in SDK/Tools/ in this distribution
2. Place in `SYS:WBStartup/` or your preferred location. If installed in WBStartup, it will run on boot, doing nothing unless there was a LastAlert from the previous session crashing
