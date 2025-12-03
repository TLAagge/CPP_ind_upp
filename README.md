# Door Access Card System
Individual assignment for C and c++ course, G level

## Description
A command line administration system for managing access cards.
Users can remotely open doors, view all registered cards, deny/grant access for cards and simulate card scanning

## Files
`main.c` - Main program with all functionality
`SafeInput.h` - Header for safe input functions  
`SafeInput.c` - Implementation of buffer-safe input handling
`Makefile` - Build automation
`README.md` - Build information 

## Requirements Implemented
No global variables (state stored in SystemState struct)
Dynamic memory allocation (malloc/realloc for unlimited cards)
Buffer overflow protection (SafeInput functions)
Stack overflow protection (proper function design)
Error handling (validates all input, no crashes)
Split into functions
User friendly interface


Tested online at: https://www.onlinegdb.com/

## Features
1. **Remote open door** - Opens door remotely (green lamp for 3 seconds)
2. **List all cards** - Shows all cards with their access status and date added
3. **Add/remove access** - Create new cards or modify the access of an existing card
4. **Scan card** - Simulate card scanning (RED=denied, GREEN=granted)
5. **Exit** - Clean shutdown with proper memory cleanup


## Author
Tuguldur Gurragchaa  
December 2025
