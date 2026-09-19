# AGE OF AETHER — Coding Standards

## Core rules
1. Prefer small, cohesive modules over giant manager classes.
2. Reuse existing abstractions before creating new systems.
3. Keep gameplay rules out of presentation-only Blueprints.
4. Keep persistent state behind authoritative services/repositories.
5. Prefer data-driven content over ID-specific hard-coded rules.
6. Validate all client-originated requests on the server.
7. Critical multi-state mutations require explicit transaction boundaries.
8. Errors must be detectable, classifiable and observable.
9. Public APIs document ownership, authority and failure behavior.
10. Do not introduce a dependency without documenting why it exists.

## C++ rules
- Use strong types/enums for domain concepts where practical.
- Prefer const-correctness and immutable inputs.
- Avoid global mutable state.
- Keep ownership/lifetime explicit.
- Avoid hidden side effects in validation functions.
- Separate validation from mutation when useful.
- Do not put database-specific code directly into gameplay classes.
- Do not hard-code item, quest, monster or event content into core engines.

## Data rules
Definitions describe static content; instances describe runtime/persistent state.
Every runtime definition needs a stable identifier, schema/version strategy, validation rules, clear ownership and documented references.

## Blueprint rules
Blueprints may compose actors/components, drive UI, present data, trigger safe public APIs and orchestrate non-authoritative visual behavior.
Blueprints must not directly mutate authoritative persistent state.

## Testing rules
New critical logic requires tests for valid input, invalid input, boundaries, ownership/authority, replay/duplicate cases where applicable and regressions.
A green build without behavioral tests is not sufficient evidence for critical gameplay systems.
