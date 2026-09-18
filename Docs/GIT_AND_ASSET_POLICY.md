# AGE OF AETHER — Git and Asset Policy

## Versioned
Always version C++ source, Unreal project/configuration files, authored Blueprints and Data Assets/Tables, schemas, migrations, documentation, tests, build/CI definitions and reproducibility scripts.

## Generated / local
Do not version Binaries/, DerivedDataCache/, Intermediate/, Saved/, IDE caches, local logs or local secrets.

## Large binary assets
Unreal authored binaries such as .uasset and .umap, plus source art/audio formats, should use Git LFS when the content pipeline begins.
.gitattributes defines the intended LFS patterns. Phase 1 must verify Git LFS is installed and the repository hosting setup accepts the workflow before importing large assets.

## Secrets
Never commit API keys, database passwords, private certificates, access tokens or production credentials.
Use environment variables or secure server-side secret storage.
OpenAI credentials, when implemented, must remain server-side and never ship in the Unreal client.

## Reproducibility
A clean clone must eventually reproduce the project together with documented external prerequisites.
Generated files must be recreated by documented tooling rather than committed.
