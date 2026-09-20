# Phase 51 — Minimal Unreal Setup
Create four Widget Blueprints:
- WBP_HUD
- WBP_Inventory
- WBP_Character
- WBP_Skills
Use UMG Designer; no C++ widget subclasses.
HUD: HP/MP, level/XP, target, hotbar, quest tracker, notifications.
Inventory: reusable item-slot widget bound to UI snapshot.
Character: class/evolution, stats, equipment.
Skills: icon, name, level, cooldown, hotbar slot.
Create one UAetherUIPresentationCatalog Data Asset and map these four screens.
Blueprint is presentation; server/domain systems remain authoritative.