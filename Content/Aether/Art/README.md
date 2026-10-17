# Age of Aether — arte 2D e estado de animação

A direção atual é **2D isométrica painterly**. Assets 3D não são obrigatórios para esta fatia.

## Arte e animação disponível

- Mage Aelwyn: frames Idle, Walk e Cast empacotados em `Preview/assets/spritesheets/player-mage.png`.
- Vigia Elara: frames Idle e Talk, incluindo gesto de quest-giver, em `Preview/assets/spritesheets/elara.png`.
- Cão Selvagem: frames Idle, Run e Attack em `Preview/assets/spritesheets/wild-hound.png`.
- Wisp de Éter: frames Idle, Glide e Pulse para o segundo encontro da região em `Preview/assets/spritesheets/aether-wisp.png`.
- Casa da primeira região permanece como sprite estático de ambiente.

As quatro folhas usam células alinhadas com alpha real e metadados de linha/FPS em `Preview/assets/spritesheets/manifest.json`. Os PNGs de folha também estão em `Content/Aether/Art/ImportReady/Animations/SpriteSheets/`. Fontes AI individuais, imagens preparadas e prompts ficam rastreáveis em `ArtSource/Generated/Animations/`, `Content/Aether/Art/ImportReady/Animations/` e [`GENERATED_ART_PROVENANCE.md`](GENERATED_ART_PROVENANCE.md).

A primeira passada contém key poses distintas para movimento/ação e frames de hold com pequenas variações em alguns ciclos de Idle. O navegador já anima essas células no Canvas. Isso ainda não equivale a um conjunto final de animação (não há todas as oito direções, Hit/Death, nem redesenho quadro a quadro completo).

## Ferramentas e validação

```bash
python -m venv .venv
. .venv/bin/activate                 # Windows: .venv\\Scripts\\activate
python -m pip install -r Tools/Art/requirements.txt
python Tools/Art/prepare_generated_sprite.py <fonte-gerada.png> <quadro-preparado.png>
python Tools/Art/build_sprite_sheets.py
python Tools/Art/validate_manifest.py
python -m unittest Tools.Art.test_prepare_generated_sprite Tools.Art.test_build_sprite_sheets
```

`prepare_generated_sprite.py` remove somente o matte neutro ligado à borda e escreve PNG RGBA. `build_sprite_sheets.py` alinha os quadros, aplica a âncora inferior, cria os atlas/manifestos e atualiza os arquivos locais da prévia. Nenhum desses scripts cria `.uasset` ou `.umap`.

## Aceite Unreal ainda pendente

1. Importar as folhas e/ou imagens-fonte em Unreal Engine 5.8 com configurações de alpha e textura revisadas.
2. Criar PaperSprites e Flipbooks reais, mapeando cada linha conforme o manifesto.
3. Associar animações ao personagem, NPC e criatura por meio dos perfis de apresentação do jogo.
4. Validar escala, pivôs, sorting, blending, direção, transições, PIE e multiplayer no editor real.
5. Registrar a versão do Unreal, os testes executados e evidência real antes de declarar runtime PASS.

Os atlas PNG são **fontes preparadas**, não ativos de runtime Unreal. Editor/import/PIE não foram executados neste ambiente.
