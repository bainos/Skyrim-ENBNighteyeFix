# BainosNighteyeFix - SKSE Plugin Project

## Overview
SKSE plugin v13 for Skyrim that integrates Night Eye spell with ENB's Khajiit Night Vision effect, with state persistence across saves.

## Architecture

### C++ Plugin (`plugin.cpp`)
- **Entry Point**: `SKSEPluginLoad()` - Initializes SKSE and registers for API init events
- **Initialization**: `InitializePlugin()` - Sets up logging, serialization, and messaging
- **ENB Integration**:
  - `g_ENB` - Global ENB SDK pointer (v1001)
  - `ENBCallback()` - Updates ENB parameter `ENBEFFECT.FX::KNActive` on PostLoad/EndFrame
  - Sets ENB parameter based on Night Eye equip state
- **Event Handling**:
  - `EquipHandler` - Monitors `TESEquipEvent` for Night Eye spell (FormID 0x000AA01D)
  - On equip: Sets `KNActive=true`, shows "Night Eye: ON"
  - On unequip: Sets `KNActive=false`, shows "Night Eye: OFF"
- **State Persistence**:
  - `NightEyeState` - Singleton managing `isActive` bool
  - SKSE serialization (Save/Load/Revert callbacks)
  - Restores ENB state on game load
- **Message Handler**: Responds to `kPostLoad` (ENB API), `kDataLoaded` (register events)

### Papyrus Script (`Scripts/BainosNighteyeFix.psc`)
```papyrus
Scriptname BainosNighteyeFix extends ActiveMagicEffect
Function toggle() Global Native
Event OnEffectStart(Actor akTarget, Actor akCaster)
```
- Extends `ActiveMagicEffect` for magic effect attachment
- `OnEffectStart` event with debug messages and UI notifications
- Global native `toggle()` function (currently unused in new architecture)

### Supporting Files
- `patch_includes.h` - Fixes CommonLibSSE/STL namespace issues, includes SKSE PCH
- `extern/enb-api/` - ENB SDK headers (v1001)
- `deploy.ps1` - Deployment script
- `compile-papyrus.ps1` - Papyrus compiler script
- `check-plugin-status.ps1` - Plugin status checker
- `find-pex-folders.ps1` - PEX folder finder

## Dependencies
- SKSE (Skyrim Script Extender)
- CommonLibSSE (RE/Skyrim.h)
- spdlog (logging)
- ENB SDK v1001
- Windows API (Psapi.h)

## Key Features
- ENB Night Vision integration via `KNActive` parameter
- Automatic detection of Night Eye spell equip/unequip
- State persistence across save/load cycles
- Event-driven architecture
- Debug notifications in-game
- Comprehensive logging (`BainosNighteyeFix.log`)

## Build System
- CMake-based
- vcpkg for dependencies
- Target: x64 Windows

## Configuration
- **File**: `Data/SKSE/Plugins/BainosNighteyeFix.ini`
- **Settings**:
  - `NightEyeSpellFormID` - FormID of spell to monitor (default: 0x000AA01D)
- Config loaded on plugin initialization with fallback to defaults

## Night Eye Spell
- Default FormID: `0x000AA01D` (vanilla Night Eye power)
- Configurable via INI file
- Triggers on equip/unequip (not cast)

## Development Resources
- **CommonLibSSE-NG**: `/mnt/c/dev/CommonLibSSE-NG/include/` - Reference for all RE/SKSE definitions
- **MANDATORY**: Do not guess API usage. Use provided resources and throw alert if something is missing.

## Communication with Claude
**IMPORTANT**: Keep chat responses super concise. No unnecessary explanations or preambles.
