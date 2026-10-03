# PHASE 2 — DIREÇÃO VISUAL ISOMÉTRICA

## Estado

**SOURCE/DOCUMENTATION: COMPLETE**  
**UNREAL RUNTIME: PENDING REAL ENVIRONMENT AUDIT**

## Objetivo

Definir a direção visual isométrica oficial do AGE OF AETHER para que câmera, composição, escala, materiais, iluminação, personagens, criaturas, arquitetura, vegetação e VFX sejam produzidos como um único sistema visual coerente.

A direção oficial é:

**Stylized Painterly Isometric / 2.5D Hand-Painted Diorama / 2.5D Isometric Cutaway**, em qualidade gráfica premium.

O jogo continua sendo 3D real no Unreal. A câmera e a composição produzem a leitura isométrica/2.5D; não se deve substituir o mundo por sprites ou um falso cenário 2D.

## Princípios visuais

1. **Leitura isométrica primeiro:** gameplay, silhuetas, profundidade e navegação devem permanecer claros em visão elevada.
2. **Diorama vivo:** ambientes devem parecer pequenas cenas artesanais densas, com composição, camadas de profundidade e storytelling ambiental.
3. **Painterly premium:** materiais possuem variação de cor/roughness, pinceladas e formas artísticas controladas sem perder volume físico.
4. **3D real:** personagens, criaturas, props, terrenos, colisões, iluminação e VFX continuam sendo elementos reais do Unreal.
5. **Escala legível:** personagens e interações devem permanecer distinguíveis em zoom de gameplay.
6. **Profundidade:** foreground, midground e background devem formar composição contínua, sem transformar o mundo em corredores artificiais.
7. **Consistência:** a mesma linguagem deve funcionar em cidade, wilderness, dungeon, combate e exploração.
8. **Premium antes de escala:** não degradar a linguagem visual para acelerar a criação das ~40 cidades.

## Câmera

A câmera alvo deve oferecer:

- projeção ortográfica ou perspectiva fortemente controlada, conforme validação real no Unreal;
- ângulo elevado consistente;
- zoom contínuo dentro de limites definidos;
- foco confortável no personagem;
- distância suficiente para leitura tática;
- ausência de clipping destrutivo em arquitetura e terreno;
- comportamento estável em multiplayer;
- movimento relativo à câmera;
- seleção/targeting legíveis;
- composição que favoreça silhueta, armas, asas, capas e VFX.

Os parâmetros finais de distância, pitch, FOV/ortho width e limites de zoom devem ser calibrados no Unreal real, não inventados como valores finais somente no repositório.

## Composição do mundo

O cenário deve ser construído em camadas:

**foreground → gameplay space → midground → landmark → atmospheric background**

Cada cena deve ter:

- ponto focal;
- caminho visual;
- área jogável clara;
- landmarks;
- contraste suficiente entre personagem e ambiente;
- oclusão controlada;
- variação de altura;
- vegetação e props com função visual e/ou gameplay.

## Paleta e materiais

A paleta será definida por bioma/jurisdição, mas sempre dentro da mesma identidade de mundo.

Materiais devem priorizar:

- albedo painterly;
- roughness coerente;
- variação de escala;
- desgaste e sujeira contextual;
- bordas legíveis;
- diferenciação de pedra, madeira, metal, tecido, vegetação, gelo, lava e outros materiais.

Não fixar uma paleta definitiva para as 40 cidades nesta fase. A Phase 2 define o sistema; o conteúdo específico será desenvolvido por região no World Master Plan e no pipeline de arte.

## Iluminação

A iluminação deve favorecer:

- separação de planos;
- leitura de silhueta;
- profundidade atmosférica;
- sombras de contato;
- landmarks;
- clima e identidade regional;
- VFX legíveis sem estourar a cena.

A configuração final de iluminação, exposição, pós-processamento e qualidade deverá ser validada por profiling real no Unreal.

## Personagens e criaturas

A leitura isométrica deve priorizar:

- cabeça/corpo;
- arma;
- silhueta;
- asas/capas quando existirem;
- efeitos de habilidade;
- estado de combate;
- direção de movimento.

Animações devem continuar sendo 3D reais e compatíveis com o sistema de combate, locomotion, equipment e multiplayer existente.

## Arquitetura e props

Arquitetura deve ser modular e compatível com composição de diorama, mas sem produzir cidades falsas ou desconectadas.

Props devem possuir escala coerente e contribuir para:

- navegação;
- narrativa ambiental;
- identidade cultural;
- gameplay;
- densidade visual.

## VFX

VFX devem reforçar gameplay e identidade visual:

- ataques;
- skills;
- buffs/debuffs;
- impactos;
- criaturas;
- clima;
- magia;
- ambiente.

A leitura deve permanecer clara em câmera isométrica, evitando excesso de partículas que esconda personagens ou telegráficos de combate.

## Relação com o mundo contínuo

A direção visual deve funcionar em **um único mundo contínuo**, aproximadamente 40 cidades e grandes jurisdições exploráveis.

A transição entre regiões deve ocorrer por geografia e linguagem visual progressiva, e não por mapas desconectados.

## Gate de validação

Esta fase pode ser considerada concluída em nível de source/documentação, mas sua aceitação visual final depende da auditoria do ambiente real do Unreal e de um benchmark jogável.

Ainda faltam no ambiente real:

- abrir o projeto;
- confirmar câmera em execução;
- calibrar zoom/ângulo;
- validar movimento relativo à câmera;
- verificar clipping/oclusão;
- validar iluminação e leitura visual;
- testar em PIE e 2-client PIE;
- confirmar ausência de erros críticos nos logs.

**Não fabricar evidência runtime.**

## Próxima fase

**PHASE 3 — REAL ART PIPELINE.**

A Phase 3 deverá transformar esta linguagem visual em pipeline de assets reais, sem fabricar binários Unreal.
