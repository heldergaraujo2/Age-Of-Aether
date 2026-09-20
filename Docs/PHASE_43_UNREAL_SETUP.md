# PHASE 43 — MINIMUM UNREAL SETUP

Only one reusable catalog is needed; no 25 Blueprint setup is required.

One-time:
- Create Content/Aether/Classes/Presentation.
- Create one UAetherClassEvolutionPresentationCatalog Data Asset.
- Keep its generated 25 entries.
- Import the first real FBX.
- Create one visual profile and one animation profile.
- Assign them to warrior.01.

For each later evolution, only assign:
- VisualProfile
- AnimationProfile
- Icon
- PresentationVFX
- PresentationSFX

Runtime:
Use ApplyClassEvolution(ClassID, EvolutionID) on the presentation component after authoritative class/evolution state has been accepted/replicated.

The component is presentation-only.