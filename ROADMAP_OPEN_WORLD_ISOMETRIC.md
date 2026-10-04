# AGE OF AETHER — ROADMAP OFICIAL 2D ISOMÉTRICO

## Objetivo

Transformar o AGE OF AETHER existente em um RPG de exploração isométrica inspirado na linguagem de apresentação de RPGs como Lords of Xulima, porém com identidade própria e qualidade visual premium.

**A principal representação visual será 2D.**

O projeto não exige personagens, criaturas, mapas ou ambientes 3D. A apresentação poderá utilizar imagens, sprites, sprite sheets, Flipbooks, camadas, paralaxe, iluminação, sombras, VFX, partículas e outros recursos da Unreal para criar profundidade visual convincente.

Elementos 3D somente serão utilizados quando trouxerem benefício comprovado. Não são requisito do projeto.

## DIRETRIZ VISUAL NÃO NEGOCIÁVEL — QUALIDADE MÁXIMA

Esta diretriz vale para **todas as fases visuais do projeto** e não deve ser perdida em nenhuma atualização do roadmap.

- **ANIMAÇÕES:** buscar animações 2D extremamente fluidas, naturais e responsivas, com frames, timing, antecipação, follow-through e transições adequados. Fluidez é requisito de qualidade, não acabamento opcional.
- **IMAGENS E ARTE:** todo conteúdo visual deve ser produzido na **melhor qualidade possível**, priorizando resolução, detalhe, coerência artística, legibilidade, acabamento e aparência premium.
- **PERSONAGENS E CRIATURAS:** movimentos, ataques, habilidades, reações e estados devem possuir transições convincentes.
- **AMBIENTES E MAPAS:** arquitetura, vegetação, props, efeitos, iluminação e composição devem manter o mesmo padrão elevado.
- **VFX E PROFUNDIDADE:** luz, sombra, partículas, paralaxe, camadas e oclusão devem criar profundidade rica sem exigir 3D.
- **REGRA DE VERDADE:** imagem ou animação criada fora do Unreal só é considerada integrada após materialização legítima na pipeline e validação real quando a fase exigir.

**Prioridade permanente:** qualidade visual máxima → animação extremamente fluida → consistência artística → integração técnica → performance baseada em evidência.


## PRINCÍPIO CENTRAL

O AGE OF AETHER **já possui a raiz sistêmica do jogo**.

Não recriar:

- skills;
- progression/XP/levels;
- attributes/state;
- inventory;
- items/equipment;
- loot/reward/respawn;
- combat;
- quests/events/dialogue;
- creatures/NPC/AI contracts;
- economy/crafting/shops;
- persistence;
- networking/server authority;
- multiplayer;
- UI framework;
- world/streaming contracts;
- registries e contratos de conteúdo existentes.

O objetivo deste roadmap é **adaptar a apresentação e produzir o conteúdo visual/jogável 2D sobre essa base**.

Regra obrigatória:

**preservar -> adaptar -> integrar -> validar -> expandir**

Nenhuma nova arquitetura deve substituir uma existente sem auditoria e justificativa técnica.

---

# FASE 0 — AUDITORIA E PRESERVAÇÃO DA RAIZ

### Objetivo
Confirmar exatamente quais sistemas já existem e definir o que será apenas adaptado para a apresentação 2D.

### Escopo
- gameplay existente;
- progression;
- combat;
- inventory/equipment;
- skills/effects;
- quests/events;
- creatures/NPC/bosses;
- economy/crafting;
- persistence;
- networking;
- multiplayer;
- world/streaming;
- UI;
- presentation;
- registries;
- asset pipeline.

### Gate
Nenhum sistema já existente deve ser reconstruído.

**Estado: CONCLUÍDA em nível repository/source.**

---

# FASE 1 — FUNDAÇÃO REAL DO UNREAL

### Objetivo
Comprovar que a raiz do projeto funciona no Unreal real antes da produção visual.

### Validar
- UHT;
- UBT Editor/Game;
- Dedicated Server;
- abertura do projeto;
- mapa real;
- GameMode;
- PlayerStart;
- PIE;
- 2-client PIE;
- Dedicated Server + cliente;
- spawn;
- posse;
- movimento;
- câmera;
- replicação;
- Automation;
- Output Log.

### Gate
Sem evidência local real, a fase permanece pendente.

**Estado: RUNTIME PENDING.**

---

# FASE 2 — DIREÇÃO VISUAL 2D ISOMÉTRICA

### Objetivo
Substituir a premissa anterior de 3D/2.5D painterly por uma direção oficialmente **2D isométrica premium**.

### Definir
- ângulo isométrico;
- framing;
- escala;
- zoom;
- leitura de profundidade;
- silhueta;
- composição;
- paleta;
- linguagem artística;
- proporções;
- contraste;
- iluminação;
- sombras;
- oclusão;
- camadas;
- paralaxe;
- VFX;
- apresentação de personagens;
- apresentação de criaturas;
- apresentação de arquitetura;
- apresentação de mapas.

### Resultado
Um padrão visual que permita produzir todo o conteúdo do jogo sem depender de modelos 3D.

**Documentação:** `Docs/AGE_OF_AETHER_2D_ISOMETRIC_VISUAL_DIRECTION.md`

**Estado: DIREÇÃO DOCUMENTADA / RUNTIME PENDING.**

---

# FASE 3 — PIPELINE DE ARTE 2D

### Objetivo
Criar uma pipeline repetível para transformar arte em conteúdo utilizável no Unreal.

### Fluxo

**Concept/arte -> source -> cleanup -> Texture -> Sprite/Sprite Sheet -> Flipbook quando necessário -> material -> perfil/Asset Registry -> consumidor de gameplay -> runtime**

### Produzir
- sprites;
- sprite sheets;
- animações;
- personagens;
- criaturas;
- NPCs;
- props;
- arquitetura;
- vegetação;
- efeitos;
- ícones;
- elementos de cenário.

### Gate
Cada categoria deve possuir:
- nomenclatura;
- origem/proveniência;
- convenção de escala;
- pivot;
- transparência;
- resolução;
- animação;
- collision quando necessária;
- AssetID;
- fallback;
- validação.

---

# FASE 4 — SISTEMA DE PERSONAGENS 2D

### Objetivo
Adaptar o personagem já existente para receber uma representação 2D sem alterar sua autoridade de gameplay.

### Preservar
- Character;
- PlayerState;
- progression;
- combat;
- inventory;
- equipment;
- skills;
- multiplayer.

### Criar/adaptar
- visual 2D;
- Sprite/Flipbook;
- estados Idle;
- Walk;
- Run;
- Attack;
- Hit;
- Death;
- Cast;
- Interaction;
- sombras;
- efeitos;
- variações por classe/evolução;
- integração com equipamentos visuais quando aplicável.

### Gate
O mesmo personagem gameplay deve continuar funcionando com a nova apresentação.

---

# FASE 5 — CRIATURAS, NPCs E CHEFES 2D

### Objetivo
Aplicar a mesma abordagem aos habitantes do mundo sem reconstruir os sistemas de gameplay existentes.

### Produzir
- criaturas comuns;
- elites;
- bosses;
- animais;
- NPCs;
- comerciantes;
- guardas;
- quest givers;
- personagens sociais;
- perfis visuais 2D data-driven;
- componente visual reutilizável;
- estados Idle/Walk/Run/Attack/Hit/Death/Cast/Interaction.

### Preservar
AI, aggro, combate, loot, quests, navegação, spawning, persistência, networking e autoridade já existentes.

### Estado
🟩 Repository/source/architecture concluído.

🟥 Materialização dos assets Paper2D e validação real no Unreal deferidas.

### Gate de runtime
Criatura/NPC/boss real deve aparecer no runtime com o comportamento existente e representação visual 2D, incluindo validação de estados, qualidade visual, multiplayer e Dedicated Server.


---

# FASE 6 — AMBIENTES E PROPS 2D

### Objetivo
Criar a biblioteca visual reutilizável do mundo e sua fundação data-driven.

### Produzir
- casas;
- muralhas;
- portas;
- torres;
- templos;
- pontes;
- estradas;
- árvores;
- rochas;
- vegetação;
- água;
- ruínas;
- dungeons;
- fogueiras;
- móveis;
- objetos interativos;
- elementos decorativos;
- perfis visuais por família;
- componentes Paper2D reutilizáveis;
- ordenação de renderização e configuração de sombra.

### Estado
🟩 Repository/source/architecture concluído.

🟥 Materialização dos assets e validação real no Unreal deferidas.

### Regra
Os assets devem ser reutilizáveis e combináveis. Evitar desenhar cada região como uma imagem descartável e impossível de expandir.
---

# FASE 7 — PROFUNDIDADE 2D, LUZ E SOMBRA

### Objetivo
Fazer o mundo 2D transmitir profundidade e presença.

### Implementar
- camadas de profundidade;
- ordem de renderização;
- oclusão;
- sombras;
- luz;
- highlights;
- ambientação;
- paralaxe;
- partículas;
- neblina;
- clima;
- efeitos mágicos;
- pós-processamento quando apropriado.

### Gate
A cena deve parecer espacial e legível sem depender de modelos 3D.

---

# FASE 8 — CÂMERA E EXPLORAÇÃO ISOMÉTRICA

### Objetivo
Adaptar a exploração existente à nova apresentação.

### Preservar/adaptar
- movimento;
- input;
- targeting;
- interação;
- seleção;
- câmera;
- zoom;
- collision;
- multiplayer.

### Resultado
Uma experiência de exploração isométrica controlada, confortável e consistente.

---

# FASE 9 — PRIMEIRA REGIÃO PERMANENTE

### Objetivo
Construir a primeira região real do mundo definitivo.

### Composição

**assentamento -> praça -> serviços -> NPCs -> saída -> estrada -> campo -> floresta -> exploração -> combate -> ruínas/landmark -> aproximação da dungeon -> dungeon**

### Regra
Não é um mapa descartável.

É a primeira região da arquitetura do mundo contínuo.

---

# FASE 10 — INTEGRAÇÃO DOS SISTEMAS EXISTENTES

### Objetivo
Conectar a nova apresentação 2D ao gameplay já criado.

### Validar
- progression;
- levels/XP;
- classes;
- skills;
- combat;
- inventory;
- equipment;
- loot;
- quests;
- interactions;
- NPCs;
- creatures;
- economy;
- persistence;
- multiplayer.

### Importante
Esta fase **não implementa esses sistemas novamente**.

Ela verifica a adaptação da apresentação 2D a eles e corrige somente lacunas reais.

---

# FASE 11 — PRIMEIRO INIMIGO E LOOP DE COMBATE

### Objetivo
Comprovar o loop completo com representação 2D.

### Fluxo

**explorar -> encontrar -> atacar -> receber dano -> usar skill -> derrotar -> XP -> loot -> continuar**

### Gate
Gameplay existente + apresentação 2D funcionando juntos no Unreal real.

---

# FASE 12 — PRIMEIRA DUNGEON

### Produzir
- entrada;
- corredores/áreas;
- salas;
- obstáculos;
- criaturas;
- interações;
- loot;
- eventos;
- mini-boss;
- boss;
- retorno.

### Regra
A dungeon deve utilizar a mesma linguagem visual 2D do mundo externo.

---

# FASE 13 — VERTICAL SLICE

### Fluxo obrigatório

**entrar -> explorar assentamento -> conversar -> aceitar quest -> sair -> explorar -> combater -> ganhar XP -> loot -> dungeon -> boss -> retornar**

### Resultado
Primeiro trecho realmente reconhecível como Age of Aether.

---

# FASE 14 — PRIMEIRO TESTE REAL JOGÁVEL

### Validar no Unreal
- startup;
- câmera;
- personagem 2D;
- movimento;
- exploração;
- interação;
- NPCs;
- criaturas;
- combate;
- skills;
- XP;
- loot;
- dungeon;
- boss;
- retorno;
- HUD;
- áudio;
- VFX;
- 2-client PIE;
- Dedicated Server smoke test;
- logs;
- profiling.

### Gate
**GO:** expandir.

**NO-GO:** corrigir a fundação.

---

# FASE 15 — WORLD MASTER PLAN

### Objetivo
Planejar o único mundo contínuo definitivo.

### Definir aproximadamente 40 grandes cidades
Cada cidade deve possuir:
- identidade;
- jurisdição;
- bioma;
- cultura;
- arquitetura;
- ecologia;
- recursos;
- estradas;
- landmarks;
- dungeons;
- eventos;
- pontos de interesse.

### Regra
Não criar 40 mapas desconectados.

O objetivo é um único mundo contínuo, com streaming interno quando necessário.

---

# FASE 16 — EXPANSÃO DAS JURISDIÇÕES

### Objetivo
Transformar cada cidade em núcleo de uma grande região explorável.

### Estrutura

**cidade -> periferia -> wilderness -> sub-regiões -> descobertas -> fronteira -> próxima jurisdição**

### Conteúdo
- estradas;
- florestas;
- campos;
- montanhas;
- rios;
- cavernas;
- ruínas;
- dungeons;
- criaturas;
- NPCs;
- recursos;
- eventos;
- atalhos;
- landmarks.

Não preencher escala com terreno vazio.

---

# FASE 17 — CIDADES VIVAS

### Integrar conteúdo visual 2D com os sistemas existentes:
- comerciantes;
- ferreiros;
- tavernas;
- guardas;
- quest givers;
- serviços;
- animais;
- diálogos;
- eventos;
- storytelling ambiental.

---

# FASE 18 — MUNDO PvE

### Produzir
- famílias de criaturas;
- elites;
- bosses;
- dungeons;
- world bosses;
- eventos;
- loot;
- respawns;
- quests;
- áreas de descoberta.

Tudo deve usar a pipeline visual 2D.

---

# FASE 19 — MUNDO VIVO E SOCIAL

### Integrar e apresentar visualmente
- party;
- guild;
- eventos;
- invasões;
- bosses;
- ciclos;
- reputação;
- descobertas;
- economia;
- zonas seguras;
- zonas PvP quando previstas pelos sistemas existentes.

---

# FASE 20 — POLIMENTO VISUAL 2D PREMIUM

### Objetivo
Eliminar aparência de protótipo.

### Polir
- personagens;
- criaturas;
- arquitetura;
- ambientes;
- sombras;
- iluminação;
- VFX;
- animações;
- UI;
- composição;
- clima;
- áudio;
- storytelling ambiental.

### Gate
Qualidade consistente entre regiões.

---

# FASE 21 — ESCALA E PERFORMANCE

### Medir com profiling real
- FPS;
- Game Thread;
- Render Thread;
- GPU;
- memória;
- draw calls;
- sprites;
- partículas;
- animações;
- streaming;
- AI;
- replication;
- bandwidth;
- server CPU.

### Regra
Otimizar baseado em evidência, não em suposição.

---

# FASE 22 — CONTEÚDO INICIAL COMPLETO

### Objetivo
Produzir a primeira grande entrega de conteúdo:
- cidades;
- regiões;
- dungeons;
- criaturas;
- bosses;
- NPCs;
- personagens;
- quests;
- equipamentos;
- skills já existentes;
- crafting;
- economia;
- eventos;
- conteúdo social.

Os sistemas continuam sendo os já existentes; esta fase concentra-se principalmente em **conteúdo e apresentação**.

---

# FASE 23 — ALPHA

### Gate
- loop principal;
- mundo explorável;
- conteúdo;
- combate;
- progressão;
- multiplayer;
- persistência;
- estabilidade;
- sem blockers críticos.

---

# FASE 24 — BETA

### Validar
- bugs;
- balanceamento;
- performance;
- UX;
- economia;
- multiplayer;
- segurança;
- conteúdo;
- estabilidade.

---

# FASE 25 — RELEASE CANDIDATE

### Validar
- build limpa;
- cliente;
- servidor;
- persistência;
- recuperação de falhas;
- patch/update;
- regressão;
- segurança;
- performance;
- conteúdo.

---

# FASE 26 — LANÇAMENTO E EVOLUÇÃO CONTÍNUA

Adicionar continuamente:
- novas regiões;
- cidades;
- dungeons;
- criaturas;
- bosses;
- personagens;
- eventos;
- histórias;
- conteúdo visual 2D.

Sem reescrever os sistemas centrais.

---

# DEFINIÇÃO DE PRONTO DO PROJETO VISUAL

O projeto será considerado visualmente coerente quando:

1. o personagem gameplay existente funcionar com representação 2D;
2. classes/evoluções possuírem identidade visual;
3. criaturas/NPCs/bosses forem apresentados em 2D;
4. ambientes puderem ser produzidos com assets 2D reutilizáveis;
5. luz/sombra/camadas criarem profundidade;
6. câmera e exploração forem isométricas;
7. combate e interação utilizarem os sistemas já existentes;
8. a primeira região permanente estiver jogável;
9. o vertical slice estiver validado;
10. o pipeline puder escalar para o mundo inteiro.

---

# O QUE ESTE ROADMAP NÃO FAZ

Não cria novamente:
- skill system;
- level system;
- inventory system;
- combat system;
- quest system;
- progression system;
- economy system;
- persistence system;
- multiplayer system;
- networking architecture.

Esses elementos pertencem à raiz existente do AGE OF AETHER.

O novo projeto é uma **transformação de apresentação + produção de conteúdo + integração + validação**, não uma reconstrução do jogo.

---

# REGRA DE VERDADE

Sempre separar:

### SOURCE/REPOSITORY
Pode ser validado por:
- inspeção;
- compilação;
- testes;
- contratos;
- documentação;
- registries.

### UNREAL RUNTIME
Precisa de:
- Unreal Editor;
- UHT/UBT;
- import real;
- assets reais;
- mapas reais;
- PIE;
- 2-client PIE;
- Dedicated Server;
- gameplay;
- logs;
- profiling.

Nunca declarar runtime PASS sem execução real.

---

# WORKFLOW OBRIGATÓRIO

Para cada fase:

1. ler `PROJECT_MEMORY/00_CONTINUITY.md`;
2. ler este roadmap;
3. auditar o que já existe;
4. preservar a raiz;
5. adaptar apenas o necessário;
6. produzir conteúdo;
7. criar/atualizar testes;
8. executar os testes possíveis;
9. validar Unreal quando a fase exigir;
10. registrar evidências;
11. atualizar documentação;
12. atualizar continuidade;
13. commit no GitHub;
14. somente então avançar.

---

# ESTADO INICIAL DA NOVA DIREÇÃO

**FASE 0 — AUDITORIA E PRESERVAÇÃO DA RAIZ: CONCLUÍDA.**

**FASE 1 — FUNDAÇÃO REAL DO UNREAL: RUNTIME GATE PENDING.**

A documentação oficial da nova direção 2D está em:

`Docs/AGE_OF_AETHER_2D_ISOMETRIC_VISUAL_DIRECTION.md`

O próximo trabalho visual deve seguir este roadmap e **não deve reconstruir os sistemas de gameplay já existentes**.
