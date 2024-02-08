# PalWorld Internal

Simple internal built with various code snippets and SDK from UC.me

> [!Note]
> Built for Steam release v0.1.4.1 Hotfix #1.

> [!Warning]
> Incompatible with other programs and overlays which also access the game.
> For example:
> - blitz.gg

## Steps to Build

This assumes you have Visual Studio 2022 installed

- Clone or download repo as .zip
- Open DX11-Base.sln
- Set build configuration to Release
- Build solution
  
## Player Features

- Modify Speed
- Modify Attack
- Modify Defense
- Infinite Health
- Infinite Stamina
- Infinite Ammo
- Show Player Position
- Print Position to Console

## Exploits

- Safe Teleport
- Open Entity List
- Give EXP
- Teleport Home
- Toggle Fly
- Revive
- Item Spawner
- Easy Pal Condensation - Requires only 1 Pal for all upgrade levels

## Cage Spawner - Thanks to contributor FriendlyButFire

- Spawn any Pal you want in cage

> [!Note]
> This only works in Single Player/Co-Op Host game modes

## Teleporter

- All Boss Locations
- Custom TP Locations
- Teleport to Waypoint

## Entity List

- Displays list of nearby entities (Players, NPCs and monsters)
- Filter list for players and/or pals
- Teleport to selected entity
- Kill entity or set to 1 HP
- Force Join Guild
- 'Mask It' can be used on your player entity to hide player name.

## Misc

- Kill Aura
- Crash server
- Destroy All Bases
- Unlock All Effigies
- Teleport Pals to XHair/vacuum
- Unlock All Chests
- Destroy All Visible Objects

## SS

![image](https://github.com/NuLLxD/PalWorld-NetCrack-NuLL/assets/965358/e34db15b-dd00-4db6-88b3-f9a9604791b4)
![image](https://github.com/NuLLxD/PalWorld-NetCrack-NuLL/assets/965358/adf3fe60-82d8-4178-b2e1-2079733087c0)
![image](https://github.com/NuLLxD/PalWorld-NetCrack-NuLL/assets/965358/09827949-5b04-47cd-91f5-5f061b031bf3)
![image](https://github.com/NuLLxD/PalWorld-NetCrack-NuLL/assets/965358/f6fa879f-065b-4607-a2a9-9be52f1176f0)
![image](https://github.com/NuLLxD/PalWorld-NetCrack-NuLL/assets/965358/1c18de56-2bba-44bc-96d5-a7e280a7076b)

## AOBS

> GObjects: `48 8B 05 ? ? ? ? 48 8B 0C C8 4C 8D 04 D1 EB 03`<br>
> FNames: `48 8D 05 ? ? ? ? EB 13 48 8D 0D ? ? ? ? E8 ? ? ? ? C6 05 ? ? ? ? ? 0F 10`<br>
> GWorld: `48 8B 1D ?? ?? ?? ?? 48 85 DB 74 33 41 B0`<br>
> Processs Event: `40 55 56 57 41 54 41 55 41 56 41 57 48 81 EC 10 01 ? ? 48 8D`

## External Library Credits

- [Dear ImGui](https://github.com/ocornut/imgui)
- [MinHook](https://github.com/TsudaKageyu/minhook)
- [Dumper7](https://github.com/Encryqed/Dumper-7)
- [DX11-Internal-Base](https://github.com/NightFyre/DX11-ImGui-Internal-Hook)
