# AGE OF AETHER — FASE 14 — PRIMEIRO TESTE REAL JOGÁVEL

## Estado da fase

🟨 **PREPARADA PARA VALIDAÇÃO REAL NO UNREAL / PC**  
🟥 **RUNTIME AINDA NÃO VALIDADO**

Esta fase prepara e formaliza a primeira bateria de testes reais da vertical slice concluída na Fase 13.

Nenhum resultado de runtime é declarado sem execução real no PC/Unreal.

---

## 1. Objetivo

Provar a primeira experiência jogável integrada:

**startup → assentamento → quest → exploração → combate → XP/loot → dungeon → boss → retorno**

A validação deve provar o comportamento real e não apenas a existência dos arquivos source.

---

## 2. Escopo herdado

A bateria valida, em conjunto:

- Fundação Unreal;
- personagem;
- câmera isométrica;
- primeira região;
- assentamento;
- Quest Hub;
- primeira quest;
- exploração;
- Wild Hound;
- combat service;
- XP/progression;
- loot/reward;
- primeira dungeon;
- checkpoint;
- Hollowed Watch;
- Guardian;
- Hollowed Watch Warden;
- retorno ao assentamento;
- persistence;
- multiplayer;
- Dedicated Server;
- apresentação 2D;
- depth/parallax;
- iluminação/sombra;
- HUD;
- áudio/VFX.

---

## 3. Regra de evidência

Cada gate deve receber um resultado real:

- 🟩 PASS — evidência suficiente;
- 🟥 FAIL — falhou e precisa correção;
- 🟨 BLOCKED — não foi possível executar por dependência;
- ⬜ NOT RUN — ainda não executado.

Não utilizar "parece funcionar" como evidência de runtime.

Para cada teste registrar:

- data/hora;
- commit testado;
- projeto local;
- versão do Unreal;
- configuração;
- mapa;
- modo de execução;
- resultado;
- log relevante;
- screenshot/video quando necessário;
- erro e reprodução quando houver.

---

# 4. Gate A — sincronização

Antes dos testes:

1. confirmar que o clone local corresponde ao `main` oficial;
2. executar pull fast-forward;
3. confirmar commit HEAD;
4. confirmar que não há alteração local não relacionada;
5. abrir o projeto oficial.

Não criar outro projeto.

---

# 5. Gate B — compilação

Validar no PC:

### UHT

O projeto deve gerar/refletir os tipos sem erros.

### UBT Editor

Compilar:

`AgeOfAetherEditor Win64 Development`

### Game

Compilar:

`AgeOfAether Win64 Development`

### Dedicated Server

Compilar o target/configuração disponível para servidor.

### Critério

Nenhum erro de compilação ou reflection relacionado à Fase 13/14.

---

# 6. Gate C — startup

Abrir o projeto no Unreal.

Validar:

- Editor inicia;
- módulo AgeOfAether carrega;
- não existem erros críticos de startup;
- GameMode é resolvido;
- mapa inicial abre;
- player start é resolvido;
- personagem pode ser criado.

Registrar Output Log.

---

# 7. Gate D — personagem e câmera

No primeiro mapa:

1. personagem aparece;
2. posse funciona;
3. movimento funciona;
4. câmera isométrica funciona;
5. yaw/pitch respeitam o perfil;
6. zoom respeita limites;
7. colisão da câmera não produz comportamento destrutivo;
8. apresentação 2D aparece quando configurada;
9. estados Idle/Walk/Run são observáveis.

Falha de qualquer item mantém o gate vermelho.

---

# 8. Gate E — assentamento

Percorrer fisicamente o assentamento.

Não basta abrir o mapa.

Validar:

- entrada;
- praça;
- Quest Hub;
- serviços;
- ruas;
- saída;
- colisões;
- navegação;
- leitura visual;
- profundidade;
- ordenação 2D;
- ausência de obstáculos críticos;
- performance básica.

O teste deve incluir caminhada/exploração real.

---

# 9. Gate F — primeira quest

No Quest Hub:

1. NPC/quest giver está acessível;
2. quest fica disponível;
3. jogador aceita;
4. estado muda para Active;
5. objetivo é exibido/atualizado;
6. quest permanece ativa durante a exploração.

Validar que o estado não é falsamente concluído.

---

# 10. Gate G — exploração

Partindo do assentamento:

1. sair pela rota correta;
2. atravessar a área externa;
3. entrar na wilderness;
4. alcançar a Combat Frontier;
5. observar streaming/transições;
6. caminhar/explorar livremente;
7. validar colisão e navegação;
8. observar apresentação 2D.

A área deve ser realmente percorrível, não apenas carregável.

---

# 11. Gate H — primeiro combate

Encontrar:

`AOA.Creature.FirstRegion.WildHound`

Validar:

- criatura aparece;
- AI/comportamento existente funciona;
- targeting funciona;
- ataque funciona;
- dano funciona;
- Hit visual ocorre;
- criatura morre;
- Death visual ocorre;
- player recebe consequência correta;
- quest objective avança quando aplicável.

Não aceitar uma implementação paralela de combate.

---

# 12. Gate I — XP / loot / reward

Depois da derrota:

- XP é concedido pelo sistema existente;
- loot é produzido;
- item entra no inventory quando aplicável;
- reward não duplica;
- quest objective é atualizado;
- persistence mantém o estado quando salvo/recarregado.

---

# 13. Gate J — entrada na dungeon

Percorrer fisicamente:

**Combat Frontier → Dungeon Approach → Dungeon Entrance**

Validar:

- portal/interação;
- requisito de nível;
- transição;
- mapa/instance;
- spawn;
- checkpoint;
- carregamento;
- apresentação 2D.

---

# 14. Gate K — dungeon

Explorar a Hollowed Watch.

Validar fisicamente os principais espaços:

1. Entrance;
2. Vestibule;
3. Broken Gallery;
4. Abandoned Barracks;
5. Crypt;
6. Guardian Hall;
7. Lower Tower;
8. Boss Chamber;
9. Exit.

Validar também:

- combate;
- atalhos;
- interações;
- checkpoint;
- navegação;
- streaming/instance;
- iluminação;
- profundidade;
- VFX;
- performance.

---

# 15. Gate L — boss

Encontrar:

`AOA.Creature.FirstRegion.HollowedWatchWarden`

Validar:

- boss aparece;
- comportamento existente funciona;
- targeting;
- ataques;
- skills/effects;
- dano;
- feedback visual;
- Hit;
- Death;
- reward;
- quest completion.

Não criar um sistema especial de boss apenas para passar o teste.

---

# 16. Gate M — retorno

Após a conclusão:

**Boss Chamber → Dungeon Exit → First Permanent Region → Settlement**

Validar:

- transição de volta;
- personagem reaparece corretamente;
- estado da quest;
- reward;
- progression;
- inventory;
- checkpoint;
- persistence;
- retorno ao Quest Hub.

---

# 17. Gate N — reinício e persistência

Executar:

1. concluir parte do fluxo;
2. sair/reiniciar conforme a infraestrutura existente;
3. restaurar sessão/estado;
4. confirmar que o estado persistente permanece coerente.

Também testar recuperação após morte/respawn quando aplicável.

---

# 18. Gate O — 2-client

Executar a vertical slice com dois clientes.

Validar:

- conexão;
- spawn;
- movimento;
- câmera/presentation;
- replicação;
- criaturas;
- combate;
- dano;
- morte;
- loot/reward;
- dungeon;
- boss;
- retorno;
- ausência de duplicação de rewards;
- autoridade do servidor.

---

# 19. Gate P — Dedicated Server

Executar Dedicated Server + cliente.

Validar:

- servidor inicia;
- não carrega apresentação puramente visual desnecessária;
- regras de gameplay continuam funcionando;
- cliente conecta;
- spawn;
- movimento;
- combate;
- dungeon;
- boss;
- rewards;
- persistence.

---

# 20. Gate Q — Output Log

Pesquisar durante os testes:

- crashes;
- asserts;
- ensure failures;
- warnings críticos;
- missing assets;
- missing classes;
- invalid references;
- replication errors;
- map load failures;
- streaming failures;
- Blueprint/runtime errors;
- Paper2D errors;
- AI errors;
- save/persistence errors.

Qualquer erro relevante deve ser classificado antes de fechar a fase.

---

# 21. Gate R — qualidade visual

Avaliar no runtime real:

### Personagem

- nitidez;
- escala;
- pivot;
- animação;
- transições;
- sombra;
- leitura isométrica.

### Criaturas

- silhueta;
- animação;
- ataque;
- Hit;
- Death.

### Ambiente

- arquitetura;
- vegetação;
- props;
- profundidade;
- layers;
- parallax;
- iluminação;
- sombra.

### Dungeon

- identidade visual;
- legibilidade;
- contraste;
- atmosfera.

### Boss

- silhueta;
- escala;
- leitura;
- VFX;
- feedback de ataques.

A diretriz de qualidade máxima continua obrigatória.

---

# 22. Gate S — performance inicial

Esta fase não é a fase definitiva de otimização, mas deve identificar blockers óbvios.

Registrar, quando disponível:

- FPS;
- Game thread;
- Render thread;
- GPU;
- memória;
- hitch perceptível;
- tempo de carregamento;
- comportamento durante streaming;
- custo visual de partículas/VFX.

A Fase 21 continuará responsável pelo profiling de escala.

---

# 23. Matriz final

| Gate | Objetivo | Estado inicial |
|---|---|---|
| A | Sync oficial | ⬜ |
| B | UHT/UBT | ⬜ |
| C | Startup | ⬜ |
| D | Character/Camera | ⬜ |
| E | Settlement | ⬜ |
| F | Quest | ⬜ |
| G | Exploration | ⬜ |
| H | Combat | ⬜ |
| I | XP/Loot | ⬜ |
| J | Dungeon entry | ⬜ |
| K | Dungeon | ⬜ |
| L | Boss | ⬜ |
| M | Return | ⬜ |
| N | Persistence | ⬜ |
| O | 2-client | ⬜ |
| P | Dedicated Server | ⬜ |
| Q | Logs | ⬜ |
| R | Visual quality | ⬜ |
| S | Initial performance | ⬜ |

---

# 24. Critério de fechamento da Fase 14

A fase somente poderá receber:

**🟩 CONCLUÍDA**

quando a vertical slice tiver sido realmente percorrida no Unreal e os gates obrigatórios possuírem evidência.

A existência do source não fecha a fase.

A documentação não fecha a fase.

Um mapa abrir não fecha a fase.

Apenas o conjunto de evidências reais fecha o gate.

---

# 25. Correção de problemas

Se um gate falhar:

1. registrar a falha;
2. identificar a causa;
3. corrigir no GitHub;
4. fazer novo commit;
5. sincronizar o PC;
6. repetir o teste afetado;
7. executar regressão dos gates dependentes;
8. atualizar esta matriz.

Não mascarar erro apenas para transformar o resultado em PASS.

---

# 26. Resultado da preparação

A Fase 14 está **pronta para a janela de validação no PC**.

A vertical slice da Fase 13 é o alvo único do teste.

O próximo trabalho de execução não deve criar outro projeto nem outra versão da experiência.

Deve validar a implementação oficial no Unreal real.

## Próximo marco

**Primeiro teste jogável real do Age of Aether:**

**Startup → Settlement → Quest → Exploration → Wild Hound → XP/Loot → Hollowed Watch → Warden → Return → Persistence → 2-client → Dedicated Server.**
