# ENB NightEye Fix

A Skyrim Anniversary Edition mod that synchronizes Night Eye spell effects with ENB's Khajiit Night Vision shader, providing a seamless visual experience for all Night Eye variants.

## ⚠️ Disclaimer

**This is my first mod - consider it alpha stage.**

- Testing has been limited to Khajiit Night Eye with [Lorerim](https://www.lorerim.com/) modlist
- Should work with lesser powers in general, but not extensively tested
- Requires `enbeffect.fx` patching - each ENB preset author may or may not choose to implement it
- The included Rudy ENB patch is for testing purposes only and will likely not be maintained
- This mod is open source - see [Open Source](#open-source) section for repository details

## What Does This Mod Do?

ENB Series does not support the way Night Eye spells are triggered in Skyrim. When you activate any Night Eye variant - Khajiit Night Eye, Vampire Hunter Sight, or Werewolf Night Eye - the ENB shader simply doesn't respond, leaving you without the intended night vision effect.

The Phinix Natural ENB patch was an earlier attempt to fix this, but it no longer works reliably in current ENB versions due to its dependency on the Params01 parameter array.

**This mod provides a working solution using a hybrid approach:**

- **SKSE Plugin**: Monitors equip/unequip events for Night Eye spells and uses the ENB API to set a hidden `KNActive` parameter
- **ENB Shader**: Uses the Phinix Night Vision visual effect implementation

The effect turns off automatically when you unequip the spell, and the state persists across saves and game loads.

**Visual Customization**: The shader parameters from the original Phinix patch are preserved and can be adjusted in real-time via the ENB GUI (`Shift+Enter`):

- **Nighteye Balance** - RGB color tint (default: blueish)
- **Nighteye SaturationInterior/Exterior** - Color saturation (0.0-10.0)
- **Nighteye BrightnessInterior/Exterior** - Brightness (-5.0 to 5.0)
- **Nighteye ContrastInterior/Exterior** - Contrast (0.0-5.0)
- **Nighteye Low ClipInterior/Exterior** - Black level clipping (0.0-1.0)

## Supported Night Eye Spells

The mod comes pre-configured to work with:

- **Khajiit Night Eye** (Power Khajiit Night Eye)
- **Vampire Hunter Sight**
- **Werewolf Night Eye**

You can easily add support for any custom Night Eye spells from mods by editing the configuration file.

## Installation Variants

The mod is distributed in two versions to accommodate different ENB presets:

### ENBNighteyeFix-RudyENB-patch

Includes the complete shader patch file pre-configured for **Rudy ENB**. This is a drop-in solution that requires no additional configuration.

### ENBNighteyeFix-NoPatch

Contains only the SKSE plugin without shader files. Use this variant if:
- You're using a different ENB preset and want to manually apply the shader patch
- You want to customize the visual effects yourself
- Your ENB preset already has compatible Night Vision shader code

## Configuration

The mod uses a simple configuration file located at `Data/SKSE/Plugins/ENBNighteyeFix.ini`.

To add support for additional Night Eye spells from mods, simply add a new line with the spell's FormID:

```ini
[General]
PowerKhajiitNightEye = 0x000AA01D
VampireHunterSight = 0x000C4DE1
WerewolfNightEye = 0x0006B10E
YourCustomSpell = 0xYOURFORMID
```

You can add as many spells as you want - there's no limit.

## Requirements

- Skyrim Anniversary Edition
- SKSE64
- ENB Series (any recent version)
- An ENB preset correctly patched

## Credits

Visual effect shader code from Phinix Natural ENB Night Eye patch.

## Open Source

This mod is fully open source. The project consists of multiple repositories:

- **[Skyrim-Modding](https://github.com/bainos/Skyrim-Modding)** - Main project repository with all components as submodules
- **[Skyrim-ENBNighteyeFix](https://github.com/bainos/Skyrim-ENBNighteyeFix)** - SKSE plugin and ENB shader patch
- **[commonlibsse-ng-ae-vcpkg](https://github.com/bainos/commonlibsse-ng-ae-vcpkg)** - Custom vcpkg registry with enb-api port
- **[enb-api](https://github.com/bainos/enb-api)** - ENB Series SDK with CMake configuration
- **[CommonLibSSE-NG](https://github.com/bainos/CommonLibSSE-NG)** - Fork updated to work with current dependency libraries (October 2025)
