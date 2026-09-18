# AGE OF AETHER — MASTER TECHNICAL BLUEPRINT
## Especificação Técnica, Arquitetural e Funcional Completa do MMORPG

Projeto: AGE OF AETHER  
Repositório: https://github.com/heldergaraujo2/Age-Of-Aether  
Documento: Master Technical Blueprint  
Status: Especificação de arquitetura e implementação  
Fonte de verdade de implementação: repositório Git  
Roadmap de execução: ROADMAP.md  
Continuidade: PROJECT_MEMORY/00_CONTINUITY.md

---

# 0. FINALIDADE DESTE DOCUMENTO

Este documento descreve, em nível técnico e funcional, como AGE OF AETHER deverá ser construído.

Ele não é um cronograma.

Ele não substitui o ROADMAP.md.

Ele define a arquitetura, responsabilidades, contratos, dados, fluxos, validações, persistência, testes, segurança e integração entre os sistemas que formarão o jogo.

A finalidade é permitir que:

1. um novo agente compreenda a arquitetura sem depender de conversas anteriores;
2. o desenvolvimento seja incremental sem perder a visão completa;
3. C++, Blueprint e dados tenham responsabilidades claramente separadas;
4. novos conteúdos possam ser adicionados sem reescrever os sistemas centrais;
5. o servidor permaneça autoridade final sobre o estado persistente;
6. os sistemas possam evoluir sem destruir dados existentes;
7. cada implementação possa ser testada isoladamente;
8. a arquitetura permaneça preparada para servidor dedicado e futura distribuição em múltiplos servidores;
9. o projeto tenha uma referência técnica muito mais detalhada que o roadmap.

Este documento descreve o estado-alvo da arquitetura. Uma seção estar documentada não significa que ela já esteja implementada.

O estado real deve sempre ser confirmado no código, nos testes, no histórico Git e no arquivo de continuidade.

---

# 1. PRINCÍPIOS FUNDAMENTAIS

## 1.1 Server authoritative

AGE OF AETHER será server-authoritative.

O cliente:
- recebe input;
- apresenta o mundo;
- apresenta UI;
- envia solicitações;
- recebe estado autorizado;
- interpola e apresenta movimento e efeitos;
- nunca decide sozinho alterações persistentes.

O servidor:
- valida solicitações;
- executa regras;
- simula estados;
- determina resultados;
- controla inventário;
- controla equipamentos;
- controla atributos;
- controla combate;
- controla recompensas;
- controla economia;
- grava estado persistente;
- registra operações críticas;
- aplica segurança.

Regra fundamental:

O cliente solicita. O servidor decide.

---

## 1.2 C++ como núcleo de regras

C++ será utilizado para sistemas em que correção, segurança, performance, persistência ou autoridade são importantes.

Exemplos:
- runtime;
- serviços;
- networking;
- sessões;
- contas;
- personagens;
- atributos;
- progressão;
- itens;
- ItemInstances;
- inventário;
- equipamento;
- combate;
- skills;
- efeitos;
- drops;
- recompensas;
- quests;
- economia;
- trade;
- persistência;
- segurança;
- auditoria;
- testes.

Blueprint não deverá conter a autoridade final desses sistemas.

---

## 1.3 Blueprint como apresentação e composição

Blueprint será utilizado quando o sistema visual e o fluxo de edição do Unreal forem vantajosos.

Exemplos:
- HUD;
- inventário visual;
- tooltip;
- menus;
- animações;
- VFX;
- SFX;
- apresentação de NPCs;
- apresentação de itens;
- efeitos visuais;
- composição de atores;
- eventos visuais;
- lógica de apresentação;
- ferramentas de designer que consumam APIs seguras de C++.

Blueprint poderá chamar C++.

C++ poderá expor funções para Blueprint.

Porém, a autoridade do servidor não será transferida para Blueprint.

---

## 1.4 Dados como conteúdo

O comportamento genérico fica no código.

O conteúdo fica nos dados.

Exemplos:
- ItemDefinition;
- MonsterDefinition;
- SkillDefinition;
- QuestDefinition;
- RewardDefinition;
- DropTable;
- ClassDefinition;
- MapDefinition;
- NPCDefinition;
- RecipeDefinition;
- EnhancementDefinition;
- EffectDefinition;
- EventDefinition.

O objetivo é que adicionar uma espada nova seja uma operação de conteúdo, não uma alteração do motor de inventário.

---

# 2. VISÃO GERAL DA ARQUITETURA

A arquitetura conceitual é:

    AGE OF AETHER

    CLIENT
    Unreal Engine
    C++ Client Systems
    Blueprint Presentation
    UI / Animation / VFX / Audio
              |
              | Requests / State
              v
    SERVER
    Server Runtime
    Authority / Validation
    Gameplay Services
    World / Combat / Items / Economy
    Security / Audit
              |
         +----+----+
         |         |
         v         v
    Persistence  Observability
    Repository   Logs / Metrics
         |
         v
       Database

A separação lógica deve existir mesmo que, durante o desenvolvimento inicial, vários componentes rodem no mesmo processo ou máquina.

---

# 3. CAMADAS

## 3.1 Presentation

Responsável por:
- UI;
- HUD;
- inventário visual;
- equipamentos visuais;
- animações;
- efeitos;
- áudio;
- feedback;
- apresentação do mundo.

Não é autoridade.

## 3.2 Application / Services

Coordena operações de alto nível.

Exemplos:
- InventoryService.MoveItem();
- EquipmentService.EquipItem();
- TradeService.ConfirmTrade();
- RewardService.GrantReward();
- EnhancementService.Upgrade();
- QuestService.CompleteQuest().

Os serviços coordenam regras e dependências.

## 3.3 Domain / Gameplay

Contém regras de negócio e entidades.

Exemplos:
- Item;
- Character;
- Monster;
- Quest;
- Skill;
- Effect;
- Trade;
- Party;
- Guild.

## 3.4 Repository / Persistence

Abstrai armazenamento.

Exemplos:
- CharacterRepository;
- ItemRepository;
- InventoryRepository;
- GuildRepository;
- TransactionRepository;
- AuditRepository.

Gameplay não deve depender diretamente de detalhes do banco.

## 3.5 Infrastructure

Contém:
- database driver;
- networking infrastructure;
- external services;
- logging backend;
- metrics;
- cache;
- queues;
- filesystem;
- deployment infrastructure.

---

# 4. ORGANIZAÇÃO DO PROJETO UNREAL

Estrutura conceitual:

    Age-Of-Aether/
    ├── AgeOfAether.uproject
    ├── Config/
    ├── Content/
    │   ├── Aether/
    │   │   ├── Characters/
    │   │   ├── Items/
    │   │   ├── World/
    │   │   ├── NPC/
    │   │   ├── Monsters/
    │   │   ├── Skills/
    │   │   ├── UI/
    │   │   ├── VFX/
    │   │   ├── Audio/
    │   │   └── Events/
    │   └── Development/
    ├── Source/
    │   └── AgeOfAether/
    │       ├── Core/
    │       ├── Runtime/
    │       ├── Networking/
    │       ├── Accounts/
    │       ├── Characters/
    │       ├── Progression/
    │       ├── Items/
    │       ├── Inventory/
    │       ├── Equipment/
    │       ├── Combat/
    │       ├── Skills/
    │       ├── Effects/
    │       ├── World/
    │       ├── NPC/
    │       ├── Monsters/
    │       ├── Quests/
    │       ├── Rewards/
    │       ├── Social/
    │       ├── Economy/
    │       ├── Crafting/
    │       ├── Events/
    │       ├── Security/
    │       ├── Persistence/
    │       ├── Administration/
    │       └── Testing/
    ├── Plugins/
    ├── Docs/
    ├── PROJECT_MEMORY/
    └── Tests/

Os nomes podem mudar durante a implementação se uma decisão técnica posterior justificar isso.

Não serão criadas centenas de classes vazias apenas para preencher a árvore.

---

# 5. CORE RUNTIME

O Core Runtime é a fundação do servidor.

Deve controlar:
- inicialização;
- shutdown;
- estado do runtime;
- relógio;
- scheduler;
- timers;
- configuração;
- logging;
- métricas;
- serviços;
- eventos internos;
- tratamento de erro;
- health state;
- recuperação.

## 5.1 Lifecycle

Estados conceituais:

    Created
      ↓
    Initializing
      ↓
    Ready
      ↓
    Running
      ↓
    Stopping
      ↓
    Stopped

Falhas críticas devem produzir estado de erro observável.

## 5.2 Service Registry

Serviços podem depender de outros serviços.

Exemplo:

    Runtime
     ├── ConfigService
     ├── LoggingService
     ├── SessionService
     ├── CharacterService
     ├── ItemService
     ├── InventoryService
     ├── PersistenceService
     └── SecurityService

A ordem de inicialização deve respeitar dependências.

## 5.3 Server Clock

Tempo do servidor deve ser centralizado.

Usos:
- cooldown;
- respawn;
- duração de efeitos;
- eventos;
- expiração de mail;
- timeout;
- duração de buffs;
- persistência periódica.

Não confiar no relógio enviado pelo cliente.

---

# 6. CONFIGURAÇÃO

Regras alteráveis devem ser data-driven.

Exemplos:

    level_max = 400
    points_per_level = 5
    xp_rate = 1.0
    drop_rate = 1.0

A implementação final pode utilizar:
- Unreal Config;
- Data Assets;
- Data Tables;
- arquivos externos;
- JSON;
- banco;
- combinação desses mecanismos.

A escolha será feita conforme o tipo de dado.

Regra:

Configuração de servidor não deve exigir recompilar C++ quando o valor puder ser alterado com segurança como dado.

---

# 7. CONTAS E SESSÕES

## 7.1 Account

Dados conceituais:
- AccountID;
- username;
- authentication reference;
- status;
- creation date;
- last login;
- permissions;
- characters;
- flags;
- ban state.

Estados:
- Active;
- Suspended;
- Banned;
- Disabled.

Senha nunca será armazenada em texto puro.

## 7.2 Session

Fluxo:

    Connect
    → Authenticate
    → Create Session
    → Load Account
    → Load Character
    → Enter World

Saída:

    Logout
    → Save Critical State
    → Persist
    → Close Session

Deve existir suporte para:
- timeout;
- heartbeat;
- reconnect;
- disconnect;
- expiration;
- invalid session;
- duplicate login policy.

---

# 8. PERSONAGEM

Character é uma entidade persistente.

Dados conceituais:
- CharacterID;
- AccountID;
- Name;
- Class;
- Level;
- XP;
- MasterLevel;
- BaseStats;
- DerivedStats;
- Skills;
- Inventory;
- Equipment;
- Warehouse;
- Currency;
- QuestProgress;
- Achievements;
- Titles;
- Guild;
- Party;
- Location;
- Status.

A representação real será modular para evitar um objeto monolítico.

---

# 9. ATRIBUTOS

Atributos base previstos:
- Strength;
- Agility;
- Vitality;
- Energy;
- Command.

Derived Stats incluem:
- HP;
- MP;
- Stamina;
- Shield;
- Attack;
- MagicAttack;
- Defense;
- AttackSpeed;
- DefenseRate;
- CriticalChance;
- CriticalDamage;
- Accuracy;
- Evasion;
- Resistances;
- MovementSpeed.

O cálculo deve ser centralizado.

Pipeline:

    Base Stats
        ↓
    Class Modifiers
        ↓
    Level Modifiers
        ↓
    Equipment
        ↓
    Item Options
        ↓
    Buffs/Debuffs
        ↓
    Temporary Effects
        ↓
    Derived Stats

Nenhum sistema deve inventar seu próprio cálculo de HP ou ataque.

---

# 10. PROGRESSÃO

## 10.1 XP

Um XP Calculator centralizado deverá receber:
- nível atual;
- XP atual;
- XP recebido;
- modificadores.

Resultado:
- novo XP;
- level up;
- níveis ganhos;
- pontos;
- recompensas.

## 10.2 Rewards

Usar:

    RewardDefinition + RewardService

Uma recompensa pode conter:
- XP;
- currency;
- item;
- item bundle;
- skill;
- title;
- achievement;
- custom reward.

Isso permite que quests, eventos, monstros e achievements reutilizem o mesmo sistema.

---

# 11. ITEM SYSTEM

Este é um dos sistemas centrais de AGE OF AETHER.

A arquitetura foi influenciada pela análise estrutural do Item.txt fornecido pelo usuário e por pesquisa pública de configurações de MU Online.

O objetivo não é copiar MU.

O objetivo é utilizar a lição arquitetural:

conteúdo de item deve ser configurável, categorizado e extensível.

---

# 12. ITEM DEFINITION VS ITEM INSTANCE

## 12.1 ItemDefinition

Define o tipo.

Exemplo conceitual:

    ItemDefinition:
        ID = 100001
        Name = "Aether Sword"
        Category = Weapon
        Width = 1
        Height = 4
        Slot = MainHand
        DamageMin = 20
        DamageMax = 35
        RequiredLevel = 10
        RequiredStrength = 30

Isso não representa uma espada específica pertencente a um jogador.

## 12.2 ItemInstance

Representa uma unidade persistente concreta.

Exemplo:

    ItemInstance:
        InstanceID = ...
        DefinitionID = 100001
        OwnerCharacterID = ...
        EnhancementLevel = 7
        Durability = 83
        Rarity = Rare
        GeneratedOptions = [...]

Duas instâncias do mesmo ItemDefinition podem ser diferentes.

Essa separação é obrigatória.

---

# 13. ITEM DEFINITION COMPLETA

Campos conceituais:
- ItemDefinitionID;
- InternalName;
- DisplayName;
- Description;
- Category;
- SubCategory;
- EquipmentSlot;
- Width;
- Height;
- ItemClass;
- Tags;
- MaxStack;
- BaseLevel;
- BaseDurability;
- BaseStats;
- Requirements;
- AllowedClasses;
- BindingRules;
- TradeRules;
- DropRules;
- EconomyRules;
- EnhancementRules;
- VisualDefinitionID;
- IconDefinitionID;
- EffectDefinitionID;
- OptionDefinitionIDs;
- SocketDefinition;
- GameplayFlags;
- SchemaVersion.

---

# 14. ITEM INSTANCE COMPLETA

Campos conceituais:
- ItemInstanceID;
- DefinitionID;
- OwnerCharacterID;
- CurrentContainer;
- CurrentLocation;
- Quantity;
- ItemLevel;
- Durability;
- Quality;
- Rarity;
- EnhancementLevel;
- Luck;
- BindingState;
- GeneratedOptions;
- Sockets;
- CustomData;
- CreationTimestamp;
- ModificationTimestamp;
- SchemaVersion.

---

# 15. CATEGORIAS DE ITEM

Taxonomia inicial.

## Equipment
- Weapon;
- OffHand;
- Head;
- Chest;
- Legs;
- Hands;
- Feet;
- Back;
- Ring;
- Necklace;
- Mount;
- Pet/Companion.

## Consumables
- HP;
- MP;
- Stamina;
- Recovery;
- Buff;
- Temporary.

## Materials
- Crafting;
- Enhancement;
- Event;
- Quest.

## Skill Items
- Scroll;
- Book;
- Unlock Item.

## Quest

Itens vinculados a objetivos.

## Cosmetic

Itens sem ou com baixo impacto de gameplay.

## Event

Itens temporários ou específicos de eventos.

## Containers

Itens que podem gerar ou armazenar conteúdo.

## Special/System

Itens com comportamentos especiais.

A taxonomia poderá ser expandida sem alterar o núcleo do inventário.

---

# 16. GRID INVENTORY

A análise do Item.txt mostrou que Width e Height são propriedades importantes para determinadas categorias.

AGE OF AETHER adotará uma abordagem de grid inventory.

Exemplos:

    Potion = 1x1
    Sword = 1x4
    Armor = 2x3
    Large Weapon = 2x4

O servidor calculará:
- posição;
- células ocupadas;
- colisão;
- limites;
- disponibilidade.

O cliente apenas apresenta o resultado.

---

# 17. INVENTORY SERVICE

Operações:
- Add;
- Remove;
- Move;
- Swap;
- Split;
- Merge;
- Stack;
- Unstack;
- Use;
- Equip;
- Unequip;
- Drop;
- Pickup;
- Destroy;
- Trade;
- Sell;
- Buy;
- QuestConsume;
- CraftConsume;
- EventConsume.

Cada operação terá:
1. request;
2. authentication/session validation;
3. ownership validation;
4. state validation;
5. rule validation;
6. mutation;
7. persistence/transaction policy;
8. audit when required;
9. authoritative response.

---

# 18. INVENTORY VALIDATION

Para mover um item:

    Request
    → Validate Session
    → Validate Character
    → Validate Item
    → Validate Ownership
    → Validate Source
    → Validate Destination
    → Validate Dimensions
    → Validate Collision
    → Apply Move
    → Persist
    → Audit if required
    → Respond

Não permitir:
- item inexistente;
- item de outro personagem;
- quantidade inválida;
- posição negativa;
- overlap;
- duplicação;
- movimento impossível;
- bypass de container.

---

# 19. EQUIPMENT

Slots previstos:
- Head;
- Chest;
- Legs;
- Hands;
- Feet;
- MainHand;
- OffHand;
- Back;
- Ring;
- Necklace;
- Mount;
- Pet.

O sistema deverá suportar:
- slot compatibility;
- class restrictions;
- level requirements;
- stat requirements;
- quest restrictions;
- event restrictions;
- binding;
- two-handed rules;
- conflict resolution;
- derived stat recalculation;
- visual update.

---

# 20. EQUIP FLOW

    Request Equip
    → Validate Session
    → Validate Character
    → Validate Item Ownership
    → Validate Item Location
    → Validate Slot
    → Validate Class
    → Validate Level
    → Validate Stats
    → Validate Restrictions
    → Resolve Existing Equipment
    → Apply Equipment
    → Recalculate Stats
    → Persist
    → Audit
    → Replicate Result

A operação deve ser atômica.

---

# 21. ITEM OPTIONS

A arquitetura deve evitar um enum gigantesco contendo todas as possibilidades de item.

Modelo:

    ItemDefinition
        +
    OptionDefinition
        +
    ItemInstance.GeneratedOptions

Exemplos:
- +Strength;
- +Agility;
- +Vitality;
- +Energy;
- +HP;
- +MP;
- +Attack;
- +Defense;
- +AttackSpeed;
- +CriticalChance;
- +CriticalDamage;
- +Accuracy;
- +Evasion;
- +Resistance;
- +ElementalDamage;
- +LifeSteal;
- +ManaSteal;
- +SkillModifier.

---

# 22. OPTION GENERATION

Uma opção gerada deve possuir:
- OptionDefinitionID;
- value;
- roll information;
- source;
- restrictions;
- optional expiration.

Exemplo:

    OptionDefinition = BonusStrength
    GeneratedValue = 12

A definição diz o que a opção significa.

A instância guarda o valor daquela ocorrência.

---

# 23. SOCKET SYSTEM

Itens podem possuir sockets.

Modelo:

    ItemInstance
      └── Sockets[]
            ├── Empty
            ├── Filled
            └── Locked

Operações:
- add socket;
- remove socket where allowed;
- insert material;
- extract material;
- validate compatibility;
- apply effect;
- recalculate stats.

Tudo deve ser server-authoritative.

---

# 24. ENHANCEMENT

Enhancement é uma operação transacional.

    Item + Material + Currency
              ↓
          Validation
              ↓
             Roll
              ↓
        Success / Failure
              ↓
          Apply Result
              ↓
           Persist
              ↓
            Audit

Configuração:
- maximum level;
- success rate;
- failure result;
- downgrade;
- destruction;
- preservation;
- required material;
- currency cost;
- pity/guarantee;
- event modifier.

Nunca confiar na taxa calculada pelo cliente.

---

# 25. DROP ENGINE

Arquitetura:

    Monster
      → DropTable
      → DropRule
      → Selection
      → Roll
      → ItemGeneration
      → Ownership
      → Inventory or WorldLoot

Tipos:
- guaranteed;
- weighted;
- random;
- rare;
- conditional;
- class-specific;
- level-specific;
- event-modified.

O resultado deve produzir ItemInstance real.

---

# 26. DUPLICATION PREVENTION

Itens persistentes devem ter identidade única.

Operações que criam ItemInstance devem passar por uma autoridade de geração.

Nunca permitir:

    Client → "Create Item ID 100001"

O servidor deve executar:

    Authorized Source
    → ItemDefinition lookup
    → Generate InstanceID
    → Generate State
    → Register Instance
    → Persist

Fontes autorizadas:
- loot;
- quest reward;
- crafting;
- GM command;
- purchase;
- event;
- approved system action.

---

# 27. WAREHOUSE

Warehouse é um container persistente separado do inventário.

Deve suportar:
- compartments;
- grid;
- item placement;
- expansion;
- locking;
- transactional movement;
- permissions where applicable;
- audit.

Fluxo:

    Inventory ↔ Warehouse

Sempre pelo servidor.

---

# 28. WORLD

World é composto por:
- Maps;
- Zones;
- SpawnPoints;
- NPCs;
- Monsters;
- Portals;
- SafeZones;
- PvPZones;
- EventZones;
- environmental rules.

---

# 29. MAP DEFINITION

Campos conceituais:
- MapID;
- Name;
- LevelRequirement;
- SpawnDefinitions;
- NPCDefinitions;
- MonsterDefinitions;
- PortalDefinitions;
- SafeZones;
- PvPZones;
- EventZones;
- Weather;
- WorldRules.

---

# 30. MOVEMENT

O cliente pode solicitar movimento.

O servidor deve validar:
- velocidade;
- distância;
- delta de tempo;
- colisão relevante;
- mapa;
- zona;
- teleport restrictions;
- stun/root/debuff;
- mount rules;
- impossible movement.

Movimentos impossíveis devem gerar:
- correction;
- warning;
- security signal;
- audit quando necessário.

---

# 31. NPC

NPC terá definição de capacidades.

Exemplos:
- Shop;
- Quest;
- Teleport;
- Craft;
- Upgrade;
- Banking;
- Dialogue;
- Event;
- Service.

Um NPC pode possuir várias capacidades.

---

# 32. MONSTERS

MonsterDefinition:
- ID;
- Name;
- Level;
- HP;
- Attack;
- Defense;
- MagicDefense;
- Speed;
- Resistances;
- Elements;
- AI;
- DropTable;
- Rewards;
- Respawn.

---

# 33. MONSTER AI

Estados previstos:

    Idle
    → Patrol
    → Detect
    → Aggro
    → Chase
    → Attack
    → Hit
    → Dead
    → Respawn

A implementação deve separar:
- state;
- perception;
- target selection;
- movement;
- attack;
- threat;
- timers.

---

# 34. COMBAT

Pipeline autoritativo:

    Request
    → Validate
    → Resolve Attacker
    → Resolve Target
    → Range
    → Cooldown
    → Resource
    → Accuracy
    → Base Damage
    → Defense
    → Resistance
    → Critical
    → Modifiers
    → Effects
    → HP/Shield
    → Death
    → Rewards
    → Audit/Combat Log

O cliente não determina o dano.

---

# 35. DAMAGE MODEL

O sistema deve ser modular.

Possível modelo:

    Base Damage
    + Weapon
    + Skill
    + Item Options
    + Buffs
    - Defense
    - Resistance
    + Critical
    + Situational Modifiers
    = Final Damage

A fórmula final será definida por dados e decisões de design.

Ela não deve ficar espalhada por dezenas de classes.

---

# 36. PVE

PvE deve controlar:
- aggro;
- threat;
- damage contribution;
- kill attribution;
- assists;
- XP;
- loot ownership;
- rewards.

Party pode modificar distribuição conforme regras configuráveis.

---

# 37. PVP

PvP deve possuir:
- zone rules;
- target restrictions;
- damage modifiers;
- penalties;
- safe zones;
- guild wars;
- event PvP;
- anti-abuse rules.

As regras serão configuráveis.

---

# 38. SKILLS

SkillDefinition:
- SkillID;
- Name;
- Class restrictions;
- Level requirement;
- Resource cost;
- Cooldown;
- Cast time;
- Range;
- Targeting;
- Effects;
- Damage profile;
- Animation reference;
- VFX reference;
- SFX reference.

A parte visual pode ser Blueprint.

A regra é C++/server authoritative.

---

# 39. EFFECT SYSTEM

Sistema genérico de efeitos:
- Buff;
- Debuff;
- Damage over Time;
- Heal over Time;
- Stun;
- Root;
- Silence;
- Slow;
- Shield;
- Immunity;
- Dispel;
- Attribute modifier.

Cada efeito deve possuir:
- definition;
- source;
- target;
- duration;
- stack policy;
- periodic interval;
- magnitude;
- start time;
- expiration;
- removal reason.

---

# 40. EFFECT STACKING

Deve existir uma política explícita.

Exemplos:
- Replace;
- Refresh;
- Stack;
- HighestWins;
- LowestWins;
- Exclusive;
- Ignore.

Isso evita que cada skill invente sua própria lógica.

---

# 41. QUEST SYSTEM

QuestDefinition:
- QuestID;
- Name;
- Description;
- prerequisites;
- objectives;
- branches;
- rewards;
- repeatability;
- level requirements;
- class requirements;
- NPC references.

Quest state:

    Locked
    → Available
    → Active
    → ObjectiveProgress
    → ReadyToComplete
    → Completed

---

# 42. REWARD SYSTEM

Rewards devem ser reutilizáveis.

Uma RewardDefinition pode conceder:
- XP;
- currency;
- ItemDefinition;
- ItemInstance;
- title;
- achievement;
- skill;
- unlock;
- reputation.

O RewardService valida duplicações e limites.

---

# 43. PARTY

Party:
- PartyID;
- Leader;
- Members;
- Roles;
- LootRules;
- XPDistribution;
- shared state.

Operações:
- invite;
- accept;
- decline;
- leave;
- kick;
- transfer leadership;
- loot configuration.

---

# 44. GUILD

Guild:
- GuildID;
- Name;
- Leader;
- Members;
- Ranks;
- Permissions;
- GuildStorage;
- GuildEvents;
- Wars.

Permissões devem ser granulares.

---

# 45. TRADE

Trade é transação multi-etapas.

    Request
    → Accept
    → Lock
    → Validate
    → Confirm
    → Commit
    → Audit

Ao entrar em Lock:
- itens ficam bloqueados;
- currency fica reservada;
- alterações incompatíveis são rejeitadas.

Após Commit:
- troca é aplicada atomicamente;
- ambos recebem estado novo;
- transação é registrada.

---

# 46. ECONOMY

Toda operação econômica relevante deve gerar identificação transacional.

Campos:
- TransactionID;
- Source;
- Destination;
- Currency;
- Amount;
- CharacterID;
- AccountID;
- Timestamp;
- Result;
- Reason.

Sinks:
- NPC purchase;
- enhancement;
- crafting;
- taxes;
- marketplace fees.

Sources:
- monster reward;
- quest;
- sale;
- event;
- admin grant.

Isso permite análise econômica posterior.

---

# 47. SHOPS

NPC shops devem consumir ItemDefinitions existentes.

Um shop não deve duplicar a definição completa do item.

ShopEntry referencia:
- ItemDefinitionID;
- price;
- stock;
- restrictions;
- availability.

---

# 48. CRAFTING

RecipeDefinition:
- RecipeID;
- inputs;
- quantities;
- requirements;
- station;
- success rate;
- outputs;
- failure behavior;
- modifiers.

Fluxo:

    Request
    → Validate Recipe
    → Validate Character
    → Validate Inputs
    → Consume Inputs
    → Execute Roll
    → Generate Output
    → Persist
    → Audit

A operação deve ser atômica.

---

# 49. EVENTS

EventDefinition:
- EventID;
- schedule;
- duration;
- phases;
- map;
- objectives;
- participation;
- contribution;
- rewards;
- bosses;
- loot;
- modifiers.

Exemplo:

    Scheduled
    → Starting
    → Active
    → PhaseTransition
    → Ending
    → Rewarding
    → Finished

---

# 50. BOSSES

Bosses podem utilizar:
- phases;
- thresholds;
- special attacks;
- enrage;
- adds;
- event objectives;
- contribution tracking;
- unique loot.

O boss não deve precisar de um novo sistema de loot para cada evento.

---

# 51. INSTANCES

Arquitetura preparada para instâncias:
- InstanceID;
- MapDefinitionID;
- participants;
- owner/group;
- difficulty;
- lifecycle;
- timer;
- objectives;
- rewards.

Lifecycle:

    Create
    → Prepare
    → Active
    → Completed/Failed
    → Reward
    → Cleanup

---

# 52. ACHIEVEMENTS

AchievementDefinition:
- AchievementID;
- criteria;
- progress;
- reward;
- title;
- visibility.

O progresso deve ser persistente.

---

# 53. TITLES

Titles podem possuir:
- ID;
- display name;
- unlock criteria;
- visual reference;
- stat modifiers if desired;
- rarity;
- expiration if temporary.

---

# 54. RANKINGS

Rankings devem ser desacoplados do gameplay.

Podem incluir:
- level;
- PvP;
- guild;
- achievement;
- economy;
- event;
- other metrics.

Não devem permitir alteração direta pelo cliente.

---

# 55. PETS, MOUNTS, COMPANIONS

Devem reutilizar conceitos de:
- Definition;
- Instance;
- Progression;
- Stats;
- Skills;
- Equipment where applicable;
- Ownership;
- Persistence.

Não criar uma arquitetura totalmente diferente para cada tipo de companheiro.

---

# 56. SOCIAL

Sistemas:
- Friends;
- Ignore;
- Chat;
- Notifications;
- Mail.

Chat deve possuir:
- channel;
- sender;
- timestamp;
- permissions;
- rate limit;
- moderation hooks.

---

# 57. MAIL

Mail pode conter:
- sender;
- receiver;
- subject;
- body;
- attachments;
- currency;
- creation time;
- expiration;
- read state;
- claimed state.

Itens anexados devem ser transacionados atomicamente.

---

# 58. PERSISTENCE

Camadas:

    Gameplay
    → Services
    → Repositories
    → Persistence
    → Database

Nunca:

    Gameplay Actor
    → SQL

A implementação final do banco será decidida quando a fundação estiver pronta e os requisitos de persistência estiverem concretizados.

---

# 59. SAVE POLICY

Não salvar o personagem inteiro a cada frame.

Estratégias:
- periodic save;
- logout save;
- critical-operation save;
- transactional save;
- checkpoint;
- recovery.

Operações críticas:
- trade;
- currency;
- item creation;
- item destruction;
- enhancement;
- purchase;
- reward.

---

# 60. CRASH RECOVERY

O sistema deve ser preparado para:
- processo cair;
- conexão cair;
- database timeout;
- partial transaction;
- duplicate request;
- server restart.

Operações transacionais devem ser idempotentes quando aplicável.

---

# 61. SCHEMA VERSIONING

Persistent objects devem possuir versão.

Exemplo:

    SchemaVersion = 1

Posteriormente:

    SchemaVersion = 2

Deve existir migration:

    V1 → V2

Nunca reinterpretar silenciosamente dados antigos.

---

# 62. NETWORKING

A rede deve distinguir:
- request;
- response;
- replicated state;
- event notification;
- error.

Exemplo:

    Client:
    MoveItemRequest

    Server:
    MoveItemResult

    Server:
    InventoryStateChanged

O protocolo deverá possuir versionamento futuro.

---

# 63. IDEMPOTÊNCIA

Requests importantes devem possuir request identifiers quando necessário.

Exemplo:

    RequestID = ABC123

Se o mesmo request for recebido novamente, o servidor poderá detectar duplicação.

Importante para:
- purchases;
- trades;
- rewards;
- item generation;
- enhancement.

---

# 64. SECURITY

Princípio:

Tudo que o cliente envia é potencialmente manipulável.

Validar:
- item IDs;
- instance IDs;
- ownership;
- quantities;
- prices;
- damage;
- cooldowns;
- movement;
- teleport;
- skills;
- rewards;
- trade;
- currency.

---

# 65. ANTI-CHEAT

Detecção conceitual:
- impossible movement;
- impossible attack speed;
- impossible cooldown;
- impossible damage;
- impossible item state;
- impossible currency;
- repeated malformed requests;
- packet flooding;
- suspicious trade patterns.

Respostas configuráveis:
- ignore;
- warning;
- correction;
- rate limit;
- disconnect;
- temporary restriction;
- investigation flag.

Penalidades graves devem ser auditáveis.

---

# 66. RATE LIMITING

Aplicar limites a:
- login;
- chat;
- movement requests;
- inventory requests;
- trade;
- marketplace;
- skill actions;
- admin actions.

Limites devem ser configuráveis.

---

# 67. AUDIT

Operações críticas devem produzir audit records.

Campos:
- AuditID;
- Timestamp;
- AccountID;
- CharacterID;
- SessionID;
- Action;
- Source;
- Target;
- Before;
- After;
- Result;
- RequestID;
- ServerInstance;
- Metadata.

Exemplos:
- item created;
- item destroyed;
- item transferred;
- currency changed;
- trade completed;
- GM command;
- enhancement;
- reward.

---

# 68. LOGGING

Categorias:
- Runtime;
- Network;
- Authentication;
- Gameplay;
- Inventory;
- Items;
- Combat;
- Economy;
- Security;
- Persistence;
- Administration.

Logs operacionais não substituem auditoria.

---

# 69. OBSERVABILITY

Métricas previstas:
- connected players;
- active sessions;
- requests/sec;
- tick duration;
- CPU;
- memory;
- database latency;
- save failures;
- network latency;
- combat throughput;
- inventory operations;
- trade operations;
- error rates.

---

# 70. ADMINISTRATION

Papéis conceituais:
- PLAYER;
- VIP;
- MODERATOR;
- GM;
- ADMIN;
- OWNER.

Permissões devem ser granulares.

Comandos possíveis:
- inspect player;
- grant item;
- remove item;
- grant currency;
- teleport;
- spawn monster;
- start event;
- stop event;
- mute;
- kick;
- ban;
- inspect audit;
- inspect health.

Toda ação administrativa deve ser auditada.

---

# 71. ITEM EDITOR — VISÃO FUTURA

Um dos objetivos mais importantes é tornar criação de conteúdo prática.

Fluxo desejado:

    Create Item
    → Select Category
    → Generate ID
    → Configure Dimensions
    → Configure Stats
    → Configure Requirements
    → Configure Classes
    → Configure Options
    → Configure Sockets
    → Configure Enhancement
    → Configure Drop
    → Configure Shop
    → Assign Icon
    → Assign Visual
    → Validate
    → Save
    → Test
    → Commit

---

# 72. ITEM VALIDATOR

Validações:
- duplicate ID;
- missing name;
- invalid category;
- invalid dimensions;
- invalid slot;
- invalid class;
- invalid requirement;
- invalid stat;
- invalid stack;
- invalid option;
- invalid visual reference;
- invalid drop reference;
- invalid shop reference;
- incompatible equipment;
- deprecated fields;
- schema mismatch.

O validator deve distinguir:
- Error;
- Warning;
- Info.

---

# 73. CONTEÚDO E VISUAIS

ItemDefinition não deve conter o modelo 3D diretamente como lógica de gameplay.

Ele referencia uma definição visual.

Modelo:

    ItemDefinition
        ↓
    VisualDefinitionID
        ↓
    Icon / Mesh / Material / VFX

Isso permite alterar visual sem alterar a lógica do item.

---

# 74. BLUEPRINT ITEM PRESENTATION

Blueprint pode receber uma definição visual e:
- apresentar mesh;
- trocar material;
- tocar VFX;
- tocar SFX;
- apresentar tooltip;
- exibir rarity;
- mostrar enhancement;
- animar pickup/equip.

Blueprint não deve inventar atributos.

---

# 75. DATA PIPELINE

Pipeline ideal:

    Content Authoring
    → Schema Validation
    → Content Registry
    → Build Validation
    → Server Load
    → Runtime Registry
    → Gameplay

Conteúdo inválido deve falhar cedo.

---

# 76. REGISTRY

Registries devem permitir lookup eficiente.

Exemplos:
- ItemRegistry;
- SkillRegistry;
- MonsterRegistry;
- QuestRegistry;
- MapRegistry;
- NPCRegistry;
- RecipeRegistry;
- EventRegistry.

Cada registry deve:
- carregar;
- validar;
- indexar;
- consultar;
- reportar erro;
- expor versão.

---

# 77. DATA VS DATABASE

Regra inicial.

Static content:
- Data Assets;
- Data Tables;
- Config;
- content files.

Persistent player state:
- database/persistence.

Não misturar indiscriminadamente os dois.

---

# 78. TESTING

Testing começa antes do primeiro grande sistema.

## Unit tests
- Item validation;
- inventory placement;
- stack;
- split;
- merge;
- equipment requirements;
- stat calculation;
- XP;
- drop probability;
- trade state machine.

## Integration tests
- character + inventory;
- inventory + persistence;
- item + equipment;
- monster + drop;
- combat + rewards;
- trade + economy.

## Security tests
- forged ID;
- forged ownership;
- duplicate requests;
- replay;
- negative quantity;
- impossible movement;
- cooldown bypass;
- trade race condition.

---

# 79. TESTES DE ITEM

Para cada novo item, testes podem verificar:

1. definição carrega;
2. ID é único;
3. categoria é válida;
4. dimensões são válidas;
5. requisitos são válidos;
6. classes são válidas;
7. opções existem;
8. visual existe;
9. stack é válido;
10. item pode ser instanciado;
11. item pode entrar no inventário;
12. item pode ser equipado quando apropriado;
13. item pode ser persistido;
14. item pode ser carregado novamente;
15. tooltip recebe dados corretos.

---

# 80. LOAD TESTING

Depois dos sistemas fundamentais:
- players simultâneos;
- combat;
- AI;
- inventory;
- drops;
- events;
- trades;
- persistence.

Medições:
- CPU;
- memory;
- tick;
- latency;
- network;
- database.

Não otimizar baseado em impressão.

---

# 81. PERFORMANCE

Princípios:
- medir antes;
- evitar trabalho por frame desnecessário;
- evitar alocações excessivas;
- separar ticks;
- usar timers;
- usar eventos;
- limitar replication;
- cachear lookups;
- carregar conteúdo adequadamente;
- evitar consultas ao banco em loops de gameplay.

---

# 82. SERVER TICK

Nem tudo precisa executar a cada tick.

Separar:
- high-frequency simulation;
- movement;
- combat;
- AI;
- timers;
- periodic persistence;
- low-frequency maintenance.

Timers e schedulers devem substituir loops desnecessários quando apropriado.

---

# 83. DEDICATED SERVER

O projeto deverá ser capaz de produzir uma build dedicada.

Servidor não deve depender de:
- HUD;
- widgets;
- input local;
- renderização;
- assets client-only.

O servidor precisa executar regras sem depender da apresentação.

---

# 84. CLIENT/SERVER SHARED CODE

Quando apropriado, estruturas compartilhadas podem conter:
- enums;
- IDs;
- serialized structs;
- protocol definitions;
- immutable data contracts.

Código server-authoritative deve permanecer protegido de manipulação client-side.

---

# 85. FUTURA ARQUITETURA MULTI-SERVER

A arquitetura deverá permitir futuramente:

    Login Server
         ↓
      Gateway
         ↓
    World/Game Servers
         ↓
     Instance Servers
         ↓
    Persistence / Services

Possíveis serviços futuros:
- Login;
- Gateway;
- World;
- Game;
- Chat;
- Instance;
- Matchmaking;
- Marketplace;
- Persistence;
- Analytics.

Não implementar todos antecipadamente.

Criar limites arquiteturais que permitam evolução.

---

# 86. CACHE E SERVIÇOS FUTUROS

Cache pode ser introduzido posteriormente para:
- static definitions;
- sessions;
- frequently accessed state;
- rankings;
- marketplace data.

Cache nunca deve criar uma segunda verdade para estado crítico sem estratégia de consistência.

---

# 87. MIGRATIONS

Toda alteração persistente relevante deve considerar:
- version;
- migration;
- rollback;
- backup;
- compatibility;
- validation.

Antes de migration:
- backup;
- validation;
- test environment;
- migration dry run quando possível.

---

# 88. IP E REFERÊNCIAS EXTERNAS

O Item.txt fornecido é referência de modelagem.

Pesquisa pública de MU Online também é referência.

Não copiar:
- código proprietário;
- client binaries;
- assets;
- maps;
- textures;
- models;
- protocols;
- proprietary databases;
- protected content.

AGE OF AETHER deve possuir implementação e conteúdo próprios.

---

# 89. DESENVOLVIMENTO INCREMENTAL

Cada etapa segue:

    Inspect
    → Design
    → Implement
    → Compile
    → Test
    → Validate
    → Document
    → Commit
    → Continue

Nunca considerar concluído somente porque uma classe foi criada.

---

# 90. DEFINITION OF DONE

Uma fase só pode ser considerada concluída quando:
- implementação existe;
- compila;
- testes executam;
- comportamento esperado foi validado;
- documentação foi atualizada;
- continuidade foi atualizada;
- falhas conhecidas foram registradas;
- próximo passo está definido.

---

# 91. CONTINUIDADE ENTRE AGENTES

O arquivo:

    PROJECT_MEMORY/00_CONTINUITY.md

é obrigatório.

Após grandes alterações deve registrar:
- fase atual;
- implementação real;
- arquivos alterados;
- commits;
- testes;
- resultados;
- falhas;
- pendências;
- riscos;
- decisões;
- próximo passo.

Novo agente deve:
1. ler continuidade;
2. ler roadmap;
3. inspecionar repositório;
4. inspecionar histórico;
5. validar estado real;
6. continuar.

---

# 92. RELAÇÃO ENTRE DOCUMENTOS

AGE OF AETHER possui papéis documentais diferentes.

## ROADMAP.md

Responde:

O que construiremos e em qual ordem?

É o mapa de execução.

## MASTER TECHNICAL BLUEPRINT

Este documento responde:

O que o projeto será, como os sistemas serão estruturados e como eles deverão funcionar?

É a especificação técnica de alto nível e arquitetura funcional.

## PROJECT_MEMORY/00_CONTINUITY.md

Responde:

Onde realmente estamos agora?

É o handoff operacional.

## Docs/ITEM_SYSTEM_REFERENCE.md

Responde:

O que aprendemos especificamente sobre a estrutura de itens e qual modelo adotaremos?

É a referência especializada do sistema de itens.

---

# 93. DIFERENÇA ENTRE ROADMAP E ARQUITETURA

O roadmap pode dizer:

    PHASE 9 — INVENTORY

Este documento explica:
- por que existe inventário;
- quais dados existem;
- quais operações existem;
- como o servidor valida;
- como o grid funciona;
- como ItemInstance participa;
- como persistence participa;
- como Blueprint apresenta;
- como testar;
- como evitar duplicação.

Portanto, os dois documentos não competem.

Eles se complementam.

---

# 94. FLUXO COMPLETO DE UM ITEM NOVO

Exemplo: adicionar uma nova espada.

## Passo 1 — Content

Criar ItemDefinition.

## Passo 2 — Identity

Definir:
- ItemDefinitionID;
- InternalName;
- DisplayName.

## Passo 3 — Classification

Definir:
- Category = Weapon;
- SubCategory = Sword;
- Slot = MainHand.

## Passo 4 — Dimensions

Definir:
- Width = 1;
- Height = 4.

## Passo 5 — Stats

Definir:
- DamageMin;
- DamageMax;
- AttackSpeed;
- Durability.

## Passo 6 — Requirements

Definir:
- Level;
- Strength;
- Agility;
- Class.

## Passo 7 — Options

Referenciar OptionDefinitions.

## Passo 8 — Visual

Referenciar visual/icon.

## Passo 9 — Drops/Shops

Adicionar referências de conteúdo.

## Passo 10 — Validator

Executar validação.

## Passo 11 — Tests

Instanciar e testar.

## Passo 12 — Runtime

Server carrega registry.

## Passo 13 — Spawn/Reward

Alguma fonte autorizada gera ItemInstance.

## Passo 14 — Inventory

Item entra no container.

## Passo 15 — Equipment

Se equipado, regras são validadas.

## Passo 16 — Persistence

Instance é persistida.

## Passo 17 — Client

UI e visual apresentam o item.

Nenhum código central do inventário deve precisar conhecer especificamente a espada.

---

# 95. FLUXO COMPLETO DE UMA TRANSAÇÃO

Modelo genérico:

    Client Request
         ↓
    Session Validation
         ↓
    Permission Validation
         ↓
    Domain Validation
         ↓
    Business Rules
         ↓
    Lock/Transaction
         ↓
    Mutation
         ↓
    Persistence
         ↓
    Audit
         ↓
    Replication
         ↓
    Client Result

Em caso de falha:

    Validation Failure
         ↓
    No Mutation
         ↓
    Error Result

Se uma transação já tiver sido aplicada e a confirmação chegar novamente, o sistema deverá detectar duplicidade quando o request for idempotente.

---

# 96. EXEMPLO — COMPRA DE ITEM

    Client → BuyRequest(ItemID, Quantity)

    Server:
      validate session
      validate shop
      validate ItemDefinition
      validate stock
      calculate authoritative price
      validate currency
      reserve/lock where needed
      generate ItemInstance
      remove currency
      persist transaction
      audit
      update inventory
      respond

O cliente nunca define o preço final.

---

# 97. EXEMPLO — QUEST REWARD

    QuestService
    → Validate completion
    → Build RewardDefinition
    → RewardService
    → Validate inventory capacity
    → Generate ItemInstances
    → Grant XP
    → Grant currency
    → Persist
    → Mark Quest Completed
    → Audit
    → Notify Client

Tudo deve ser consistente.

---

# 98. EXEMPLO — MONSTER DEATH

    Combat
    → Monster HP reaches zero
    → Resolve killer/contributors
    → Resolve XP
    → Resolve DropTable
    → Roll drops
    → Generate ItemInstances
    → Resolve ownership
    → Spawn world loot or inventory reward
    → Persist relevant state
    → Audit

---

# 99. EXEMPLO — EQUIPAMENTO

    Client
    → EquipRequest(ItemInstanceID)

    Server
    → Session
    → Ownership
    → Location
    → ItemDefinition
    → Slot
    → Class
    → Level
    → Stats
    → Restrictions
    → Conflict
    → Apply
    → Recalculate
    → Persist
    → Replicate

---

# 100. EXEMPLO — TRADE

    A requests trade with B
    ↓
    B accepts
    ↓
    Both add items/currency
    ↓
    Both lock offer
    ↓
    Server validates both sides
    ↓
    Both confirm
    ↓
    Server atomically exchanges assets
    ↓
    Persistence
    ↓
    Audit
    ↓
    Notify both

Se qualquer condição crítica deixar de ser válida antes do commit, a transação deve falhar ou voltar para estado seguro.

---

# 101. DESIGN RULE — REUSE BEFORE DUPLICATE

Antes de criar um novo sistema:
1. procurar sistema existente;
2. verificar se o comportamento pode ser generalizado;
3. verificar se um Definition/Service/Component existente pode ser reutilizado;
4. somente criar algo novo quando necessário.

Exemplo:

Não criar:
- QuestRewardItemSystem;
- EventRewardItemSystem;
- MonsterRewardItemSystem.

Criar:
- RewardService.

E fazê-lo ser consumido por:
- quests;
- events;
- monsters;
- achievements.

---

# 102. DESIGN RULE — DATA OVER HARD-CODE

Se algo é conteúdo configurável, preferir dados.

Ruim:

    if ItemID == 100001:
        Damage = 50

Melhor:

    ItemDefinition:
        DamageMin = ...
        DamageMax = ...

O C++ interpreta a definição.

---

# 103. DESIGN RULE — AUTHORITY OVER CONVENIENCE

Nunca permitir uma solução rápida que transfira autoridade para o cliente.

Ruim:

    Client sets inventory state.

Correto:

    Client requests inventory operation.
    Server validates and mutates.
    Client receives authoritative state.

---

# 104. DESIGN RULE — TEST BEFORE EXPANSION

Não avançar para o sistema seguinte quando o anterior possui falhas fundamentais não compreendidas.

Cada fundação deve fornecer contratos estáveis para a próxima camada.

---

# 105. DESIGN RULE — DOCUMENT ACTUAL STATE

Documentação futura não pode mascarar implementação ausente.

Se o documento diz:

    Inventory planned

mas o código não existe:

    Status = NOT IMPLEMENTED

Se existe parcialmente:

    Status = PARTIAL

Somente após compilação/teste/validação:

    Status = VERIFIED

---

# 106. FASES DE IMPLEMENTAÇÃO

A ordem de implementação permanece definida pelo ROADMAP.md.

Resumo:

0. Repository/Architecture Foundation  
1. Unreal Project Foundation  
2. Core Runtime  
3. Networking Foundation  
4. Accounts/Sessions  
5. Character  
6. Progression  
7. Item Definition Registry  
8. Item Instance  
9. Inventory  
10. Equipment  
11. Options/Effects  
12. Enhancement  
13. Sockets/Special Items  
14. Drop Engine  
15. World  
16. NPC/Monster  
17. Combat  
18. Skills/Effects  
19. Quests/Rewards  
20. Party/Guild  
21. Economy/Trade  
22. Crafting  
23. Events  
24. Master Progression  
25. Social  
26. Pets/Mounts/Companions  
27. Security/Anti-Cheat  
28. Persistence Hardening  
29. Dedicated Server  
30. Load/Stress  
31. Optimization  
32. Production Readiness

Este documento explica o conteúdo técnico dessas fases; o roadmap continua sendo o instrumento de execução.

---

# 107. PRIMEIRA IMPLEMENTAÇÃO REAL

A próxima etapa concreta não é criar todos os sistemas.

É construir a fundação Unreal.

Primeiro:
1. confirmar versão do Unreal;
2. confirmar toolchain;
3. criar/validar projeto C++;
4. configurar Git;
5. definir Git LFS quando necessário;
6. criar estrutura inicial;
7. criar módulo;
8. compilar;
9. abrir no Unreal;
10. validar baseline;
11. registrar resultado;
12. atualizar continuidade.

---

# 108. CRITÉRIO PARA CRIAÇÃO DE NOVOS SISTEMAS

Um novo sistema deve ter:
- propósito;
- responsabilidade;
- entradas;
- saídas;
- dependências;
- autoridade;
- persistência;
- networking;
- testes;
- observabilidade;
- integração;
- documentação.

Antes de implementá-lo, deve ser identificado:

Qual problema este sistema resolve e qual sistema existente não resolve esse problema?

---

# 109. CRITÉRIO PARA NOVOS ITENS

Novo item normal deve exigir apenas:
- definição;
- dados;
- referências;
- assets;
- validação;
- testes.

Se adicionar um item exigir modificar:
- InventoryService;
- EquipmentService;
- DropEngine;
- Persistence;
- Networking;

sem necessidade real, isso deve ser considerado sinal de arquitetura excessivamente acoplada.

---

# 110. CRITÉRIO PARA NOVAS CLASSES

Classes devem ser data-driven.

Adicionar uma classe futura deverá idealmente significar:
- ClassDefinition;
- stats;
- skills;
- restrictions;
- progression;
- visuals.

O motor de personagem não deve ser reescrito para cada classe.

---

# 111. CRITÉRIO PARA NOVOS MONSTROS

Novo monstro deve exigir:
- MonsterDefinition;
- stats;
- AI profile;
- DropTable;
- visual;
- spawn configuration.

Não criar um novo sistema de combate por monstro.

---

# 112. CRITÉRIO PARA NOVOS EVENTOS

Novo evento deve reutilizar:
- EventDefinition;
- EventService;
- objectives;
- contribution;
- RewardService;
- DropEngine;
- World/Zone.

Não criar uma arquitetura isolada para cada evento.

---

# 113. CRITÉRIO PARA NOVAS QUESTS

Nova quest deve ser conteúdo:
- QuestDefinition;
- objectives;
- prerequisites;
- rewards;
- dialogue references.

O QuestEngine deve permanecer genérico.

---

# 114. VERSIONAMENTO

O projeto deve versionar:
- code;
- schemas;
- data;
- configuration;
- migrations;
- documentation.

Assets binários grandes podem utilizar Git LFS quando apropriado.

---

# 115. GIT

GitHub é a fonte de verdade do projeto.

Após cada alteração significativa:
- commit;
- documentação;
- continuidade;
- status.

Commits devem descrever claramente o que mudou.

---

# 116. BRANCHING

Durante desenvolvimento inicial, uma estratégia simples pode ser utilizada.

Quando o projeto crescer, considerar:
- main;
- development;
- feature branches;
- release branches;
- hotfix.

A estratégia final será adotada conforme a necessidade real.

---

# 117. BUILD VALIDATION

A cada grande alteração:
1. compilar;
2. executar testes;
3. abrir Unreal;
4. validar comportamento quando aplicável;
5. verificar logs;
6. verificar Git diff;
7. atualizar documentação.

---

# 118. ERROR HANDLING

Erros devem ser:
- detectáveis;
- classificáveis;
- logáveis;
- retornáveis ao chamador;
- observáveis;
- recuperáveis quando possível.

Não utilizar falhas silenciosas para operações críticas.

---

# 119. INVALID STATE

Sistemas devem possuir invariantes.

Exemplos:
- item sem definição é inválido;
- ItemInstance sem ID é inválida;
- item pertencendo a dois personagens simultaneamente é inválido;
- currency negativa é inválida quando a economia não permitir;
- equipamento incompatível é inválido;
- stack acima do máximo é inválido.

Invariantes devem ser testados.

---

# 120. TRANSACTION BOUNDARIES

Operações que alteram múltiplos estados relacionados devem possuir uma fronteira transacional.

Exemplos:

Trade:
- A inventory;
- B inventory;
- A currency;
- B currency.

Purchase:
- Currency;
- Inventory;
- Transaction.

Enhancement:
- Item;
- Material;
- Currency;
- Result.

Quest reward:
- Quest state;
- XP;
- Items;
- Currency.

---

# 121. DATA OWNERSHIP

Cada dado deve possuir um dono claro.

Exemplo:
- ItemDefinition → Content Registry;
- ItemInstance → Item/Persistence;
- Inventory placement → Inventory;
- Equipment slot → Equipment;
- Character base stats → Character;
- Derived stats → Stats system;
- Currency → Economy;
- Quest progress → Quest system.

Isso reduz conflitos e inconsistências.

---

# 122. EVENTS INTERNOS

Sistemas podem comunicar mudanças por eventos internos.

Exemplos:
- OnItemAdded;
- OnItemRemoved;
- OnItemEquipped;
- OnCharacterLevelUp;
- OnMonsterKilled;
- OnTradeCompleted;
- OnQuestCompleted;
- OnEventStarted.

Eventos não devem substituir transações ou autoridade.

Eles comunicam fatos após ou durante operações controladas.

---

# 123. REPLICATION

Replicar somente o necessário.

Exemplo de estado para UI:
- inventory snapshot;
- equipment state;
- character stats;
- nearby actors.

Dados administrativos ou sensíveis não devem ser enviados ao cliente sem necessidade.

---

# 124. CLIENT CACHE

Cliente pode manter cache de:
- static ItemDefinitions;
- icons;
- visual definitions;
- skill presentation;
- monster presentation.

Mas o servidor permanece autoridade sobre estado mutável.

---

# 125. UI INVENTORY

A UI deve:
- apresentar grid;
- mostrar dimensões;
- mostrar quantity;
- mostrar rarity;
- mostrar enhancement;
- mostrar tooltip;
- permitir drag/drop como solicitação;
- mostrar resultado autoritativo;
- corrigir visualmente caso o servidor rejeite.

O cliente não deve assumir que uma operação visual foi aceita.

---

# 126. TOOLTIP

Tooltip deve ser derivado dos dados autorizados.

Pode apresentar:
- Name;
- Description;
- Category;
- Requirements;
- Base stats;
- Options;
- Enhancement;
- Durability;
- Rarity;
- Binding;
- sockets.

A regra de cálculo deve vir do servidor/dados, não ser reinventada no widget.

---

# 127. BLUEPRINT/C++ CONTRACT

Quando Blueprint precisar de gameplay:

    Blueprint
    → Public C++ API
    → Service
    → Validation
    → Result
    → Blueprint Presentation

Não:

    Blueprint
    → direct mutation of persistent state

---

# 128. EXTENSIBILIDADE

O projeto deve ser extensível em:
- items;
- classes;
- skills;
- monsters;
- maps;
- NPCs;
- quests;
- events;
- recipes;
- options;
- effects;
- rewards.

A extensibilidade vem de:
- definitions;
- registries;
- services;
- components;
- interfaces;
- configuration.

---

# 129. O QUE NÃO FAZER

Não:
- colocar toda lógica em Blueprint;
- confiar no cliente;
- criar uma struct gigante para todos os itens;
- hard-codear centenas de IDs;
- salvar tudo por frame;
- conectar gameplay diretamente ao banco;
- duplicar sistemas de reward;
- duplicar sistemas de options;
- criar um sistema exclusivo para cada item;
- considerar código não compilado como pronto;
- considerar documentação como implementação;
- copiar implementação proprietária de outro jogo.

---

# 130. VISÃO DE LONGO PRAZO

Quando concluído, AGE OF AETHER deverá possuir uma arquitetura na qual:
- novos itens podem ser adicionados por dados;
- novos monstros podem ser adicionados por dados;
- novas quests podem ser adicionadas por dados;
- novas skills podem ser adicionadas por dados;
- novos eventos podem ser adicionados por dados;
- o servidor controla o estado;
- Unreal apresenta o mundo;
- Blueprint acelera conteúdo visual;
- C++ mantém as regras;
- persistence mantém a continuidade;
- audit torna operações críticas rastreáveis;
- testes protegem regressões;
- observability permite operar o servidor;
- dedicated server permite execução sem cliente;
- boundaries permitem futura expansão multi-server.

---

# 131. ESTADO ATUAL DO DOCUMENTO

Este documento descreve a arquitetura-alvo.

No momento de sua criação:
- o repositório existe;
- o roadmap existe;
- o arquivo de continuidade existe;
- a referência do sistema de itens existe;
- o Item.txt fornecido foi analisado;
- pesquisa pública de padrões de configuração de itens foi realizada;
- a arquitetura C++ + Blueprint + Data foi definida conceitualmente;
- a fundação Unreal ainda precisa ser implementada;
- os sistemas de gameplay ainda não devem ser considerados implementados.

---

# 132. PRÓXIMO PASSO OFICIAL

O próximo passo é:

FASE 1 — UNREAL PROJECT FOUNDATION

Objetivo:

transformar a especificação em um projeto Unreal real, compilável, versionado e preparado para receber o Core Runtime e os sistemas server-authoritative.

A partir daí, a implementação seguirá o ROADMAP.md.

---

# 133. REGRA FINAL

AGE OF AETHER não será desenvolvido como uma sequência de protótipos desconectados.

Será desenvolvido como uma plataforma MMORPG coerente.

Cada sistema deverá:
- ter responsabilidade definida;
- possuir autoridade definida;
- possuir dados definidos;
- possuir contratos definidos;
- ser testável;
- ser observável;
- ser persistível quando necessário;
- ser extensível;
- integrar-se aos sistemas existentes;
- ser documentado;
- ser validado.

Objetivo arquitetural:

C++ define como o mundo funciona.  
Dados definem o que existe no mundo.  
Blueprint define como o mundo é apresentado.  
O servidor decide o estado verdadeiro.  
A persistência garante continuidade.  
Os testes garantem confiança.

Esse princípio deve permanecer válido durante toda a evolução do AGE OF AETHER.
