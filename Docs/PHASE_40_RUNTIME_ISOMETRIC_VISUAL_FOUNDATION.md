# Runtime Isometric Visual Foundation — F40

## Objetivo

Materializar no runtime a primeira apresentação visual jogável da primeira região sem fabricar assets binários do Unreal e sem reconstruir os sistemas centrais do jogo.

## Problema encontrado na validação real

A execução da Fase 14 mostrou que:

- o projeto compilava e iniciava;
- o personagem possuía movimentação;
- a câmera ainda estava em comportamento livre/perspectiva porque nenhum perfil isométrico havia sido atribuído ao componente;
- o mapa materializado no PC continha essencialmente o chão de desenvolvimento;
- não havia representação visual runtime do personagem;
- não havia composição visual runtime suficiente de assentamento, estrada, vegetação, água, portão ou landmarks.

Esse resultado foi classificado como evidência real de que a arquitetura visual existente ainda não estava materializada.

## Implementação

### Câmera

UAether2DIsometricCameraComponent agora possui fallback runtime seguro quando nenhum UAether2DIsometricCameraProfile foi atribuído:

- modo fixo isométrico;
- yaw de 45 graus;
- pitch de -55 graus;
- distância inicial de 650 unidades;
- zoom entre 320 e 900;
- câmera sem free-look;
- rotação absoluta para não girar junto com a orientação do personagem.

Quando um UAether2DIsometricCameraProfile real existir, ele continua tendo precedência.

### Personagem

AAetherCharacter agora possui uma representação visual runtime mínima e determinística baseada exclusivamente em recursos nativos do Unreal:

- corpo;
- cabeça;
- manto;
- materiais dinâmicos com cores distintas;
- componentes sem colisão, preservando a cápsula gameplay.

Isso não substitui o futuro asset 2D premium; funciona como materialização visual legítima da fundação para impedir que o gameplay continue invisível.

### Primeira região

AAetherDevelopmentWorldActor deixou de representar apenas um cubo de chão e passou a construir, no runtime, uma composição visual inicial:

- terreno gramado;
- praça central;
- estrada principal;
- ramificação de estrada;
- área de água;
- três casas;
- telhados;
- portas;
- oito árvores;
- rochas;
- portão;
- landmark central.

A composição usa primitivas nativas do Unreal e materiais dinâmicos em runtime. Nenhum .uasset, .umap, .fbx ou textura binária foi fabricado.

## Limite consciente

Esta implementação é uma ponte de materialização, não a qualidade artística final.

A qualidade premium 2D continuará exigindo assets visuais reais, sprites/flipbooks, materiais, composição de camadas, iluminação, VFX e validação no Unreal.

## Validação obrigatória no PC

Após sincronizar este commit:

1. recompilar AgeOfAetherEditor Win64 Development;
2. abrir AetherWorld_FirstRegion;
3. executar PIE;
4. confirmar câmera isométrica fixa;
5. confirmar personagem visível;
6. caminhar pela praça, casas, estrada, portão, árvores, água e landmark;
7. confirmar que o mouse não gira a câmera;
8. confirmar zoom;
9. registrar screenshots e logs;
10. somente depois avançar para materialização de NPC/criatura e assets 2D reais.

## Regra de fechamento

F40 não é considerada runtime PASS apenas pela compilação. O runtime precisa ser executado no Unreal e visualmente verificado.
