# AGE OF AETHER — Test Strategy

## Test pyramid
### Repository / static validation
Check required documentation, forbidden generated directories, policy documents, continuity state and absence of secrets.

### Unit tests
C++ domain rules and pure logic: identifiers, validation, item definitions, inventory geometry, stats, economy, combat and progression.

### Integration tests
Cross-service behavior: session to character, registry to instance, inventory to equipment, reward to inventory, transaction to persistence and request to authoritative mutation.

### Functional / Unreal tests
When Unreal exists: editor startup, map loading, PIE, multiplayer, dedicated server, asset loading and UI workflows.

### Regression tests
Every fixed defect should gain a regression test when practical.

### Performance / load
Later phases measure tick cost, memory, network bandwidth, concurrent sessions, persistence latency and transaction throughput.

## Phase 0 validation
Phase 0 is repository/documentation validation. Unreal compilation is not expected because the Unreal project does not exist yet.
Evidence: required source-of-truth documents exist; repository tree and recent history were inspected; architecture and IP rules are explicit; Git/asset policy is explicit; naming/coding standards are explicit; test strategy is explicit; Phase 1 boundary is explicit.

## Phase 1 entry gate
1. Verify Unreal 5.8.1 locally.
2. Verify C++ toolchain.
3. Verify Git and Git LFS.
4. Create/open .uproject.
5. Compile the baseline C++ project.
6. Check clean-clone reproducibility.
