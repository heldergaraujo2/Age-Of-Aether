# AGE OF AETHER — FASE 11 — PRIMEIRO INIMIGO E LOOP DE COMBATE

## Objetivo

Estabelecer no nível repository/source/architecture o primeiro inimigo da primeira região permanente e a ponte que permite ao combate PvE existente operar contra criaturas sem criar um segundo combat system.

Fluxo-alvo: exploração → encontro → alvo → ataque/skill → dano → hit/defeat → XP → loot → continuidade.

Unreal runtime, materialização de assets e validação jogável permanecem deliberadamente deferidos nesta fase.

## 1. Auditoria da raiz

O projeto já possui FAetherCombatService, UAetherCombatSubsystem, resolução autoritativa de basic attack, range/cooldown/accuracy/critical/dano/defesa/resistência/shield, estados Alive/Dead, FAetherCreatureDefinition, FAetherCreatureSpawnDefinition, UAetherCreatureSubsystem, AAetherCreatureActor, registry/catalog de criaturas, progression/XP, loot/reward/respawn/drop/spawn-group registries, skills/effects e os hooks da primeira região.

O gap real encontrado foi de adaptação: o combat service trabalha sobre FAetherCharacterRecord, enquanto a criatura mantém seu próprio estado de vida. A solução é uma ponte explícita e reutilizável.

## 2. Ponte Creature → Combat

Arquivos:
- Source/AgeOfAether/Public/Combat/AetherCreatureCombatAdapter.h
- Source/AgeOfAether/Private/Combat/AetherCreatureCombatAdapter.cpp

FAetherCreatureCombatAdapter:
1. converte FAetherCreatureDefinition + actor em FAetherCharacterRecord;
2. reutiliza ResolveBasicAttack para jogador → criatura;
3. reutiliza o mesmo serviço para criatura → jogador;
4. preserva range, cooldown, accuracy, critical, dano, defesa, resistência e autoridade;
5. aplica o resultado aceito ao estado da criatura;
6. não cria regras paralelas de dano.

A identidade derivada é creature:<CreatureID>, permitindo reutilizar o mapa de cooldown existente.

## 3. Dano e derrota

AAetherCreatureActor recebeu ApplyCombatDamage(float Damage).

O método rejeita dano inválido, impede dano em criatura morta, reduz a vida existente, limita a vida inferior a zero e marca bAlive=false ao chegar a zero. Ele é o ponto único para futura materialização de hit/death, XP, loot e respawn.

## 4. Integração 2D

FAetherCreatureDefinition agora aceita Visual2DProfile opcional.

AAetherCreatureActor cria UAether2DLivingVisualComponent e aplica o perfil quando configurado.

Estados de combate ligados à apresentação:
- dano com criatura viva → Hit;
- derrota → Death;
- reset de vida → Idle.

O perfil 2D é opcional; ausência dele não invalida a criatura nem altera a autoridade do gameplay.

## 5. Primeiro inimigo canônico

Primeira criatura de produção: Wild Hound — Cão Selvagem da Fronteira.

IDs canônicos:
- CreatureID: AOA.Creature.FirstRegion.WildHound
- VisualProfileID: AOA.Creature.WildHound
- LootTableID: AOA.Loot.FirstRegion.WildHound
- RewardDefinitionID: AOA.Reward.FirstRegion.WildHound
- SpawnID: AOA.Spawn.FirstRegion.WildHound.Frontier

Esses IDs formam o contrato estável de produção. Eles não significam que assets Unreal binários ou um spawn real já existam no repository.

O Wild Hound é a referência inicial para encontro, target, basic attack, Hit, Death, XP, loot, respawn e apresentação 2D. Deve ser legível em câmera isométrica, visualmente reconhecível por silhueta e possuir Idle/Walk/Run/Attack/Hit/Death extremamente fluidos quando materializado.

Valores finais de balanceamento e assets concretos ficam para a materialização de conteúdo e não foram falsamente declarados como runtime facts.

## 6. Loop de combate

A. Exploração: primeira região usando movimento, câmera, world/streaming e interação existentes.

B. Encontro: spawn do Wild Hound pelo sistema de criaturas existente.

C. Target: seleção usando o targeting existente, sem sistema paralelo.

D. Ataque: combat service existente através do Creature Combat Adapter.

E. Dano: resultado aplicado via ApplyCombatDamage.

F. Apresentação: Attack no atacante, Hit no alvo, Death no alvo derrotado.

G. Derrota: bAlive=false e outcome Defeated; criatura fica elegível para recompensa/drop/respawn existentes.

H. XP e loot: continuam nos sistemas existentes. Nenhum reward system novo foi criado.

## 7. Multiplayer e servidor

A resolução continua autoritativa. O estado da criatura pertence ao gameplay. A apresentação 2D é apenas visual e Dedicated Server não deve carregar apresentação 2D. A prova real de replicação, servidor, cliente e apresentação permanece pendente de Unreal.

## 8. XP, loot e persistência

A fase não recria progression, XP, inventory, item registry, loot, rewards, respawn ou persistence.

Cadeia de integração:
Creature.Defeated → reward/XP existente → loot/drop existente → inventory/item existente → persistence existente → respawn existente.

A orquestração real dessa cadeia será validada posteriormente.

## 9. Primeira região

O inimigo pertence à zona lógica CombatFrontier da Region.FirstPermanent. A materialização posterior deverá respeitar streaming cells, navegação, rotas de exploração e retorno ao assentamento. Nenhuma coordenada .umap foi fabricada nesta fase.

## 10. Qualidade visual

Diretriz permanente: qualidade visual máxima → animação extremamente fluida → consistência artística → integração técnica → performance baseada em evidência.

Na materialização, o Wild Hound deverá receber arte 2D de alta qualidade, silhueta clara, animação fluida, antecipação/follow-through, Hit/Death convincentes, sombra, profundidade por camadas e VFX quando necessário.

Imagem externa não é asset Unreal integrado até passar pela pipeline legítima.

## 11. Arquivos desta fase

- Source/AgeOfAether/Public/Combat/AetherCreatureCombatAdapter.h
- Source/AgeOfAether/Private/Combat/AetherCreatureCombatAdapter.cpp
- Source/AgeOfAether/Public/Creatures/AetherCreatureActor.h
- Source/AgeOfAether/Private/Creatures/AetherCreatureActor.cpp
- Source/AgeOfAether/Public/Creatures/AetherCreatureTypes.h
- Docs/PHASE_11_FIRST_ENEMY_COMBAT_LOOP.md
- PROJECT_MEMORY/00_CONTINUITY.md
- ROADMAP_OPEN_WORLD_ISOMETRIC.md

## 12. Validação

Repository/source: auditoria direta da arquitetura existente e implementação da ponte no repository oficial.

Unreal: DEFERIDO. Não foram declarados UHT, UBT, PIE, 2-client, Dedicated Server, spawn real, combate real, XP real, loot real, exploração real ou qualidade visual como PASS.

## 13. Definição de conclusão

No nível declarado desta etapa, a Fase 11 está concluída quando existe contrato canônico do primeiro inimigo; ponte explícita criatura/combat service; atualização segura do estado de vida; ligação Hit/Death/Idle à apresentação 2D; integração apontando para XP/loot/respawn existentes; preservação da arquitetura central; compatibilidade com CombatFrontier; documentação e continuidade atualizadas; e runtime explicitamente pendente.

Resultado: 🟩 SOURCE / REPOSITORY / ARCHITECTURE
Runtime: 🟥 DEFERIDO

## Próxima fase

FASE 12 — PRIMEIRA DUNGEON.

A dungeon deverá reutilizar câmera isométrica, apresentação 2D, profundidade/camadas, criaturas, combate, loot, quests/events, streaming, persistence e networking, sem reconstruir sistemas centrais.
