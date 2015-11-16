# OS

My personal project - Operation System development.

# TODO
- [x] Problema no carregamento da imagem do kernel. Se for muito grande o tamanho do kernel, da problema. 
**Corrigido:** *27/09/15: Image carregada no endereço 0x3000.* Commit: [402033ca9daaf47093d13d2df854847f8c44036f].
- [x] Problema na função fdctrl_init_dma(). **Corrigido:** *21/09/15: A20 resolveu o problema* 
Commit: [397bd43de8a92ec94803fcc2d181192660184b81].
- [x] Problema no mecanismo de paginação. **Corrigido:** *21/09/15: A20 estava desabilitado - A20 foi habilitado.* Commit: [397bd43de8a92ec94803fcc2d181192660184b81].
- [x] Arrumar parte floopy - carregar tudo primeiro e depois acessar. **Corrigido** *30/09/15: problema conversão lba->chs* Commit: [ae3bcbc3fc6ebbdb05fe4d5c0617d5f3f6249f70].
- [ ] Problema ao alocar 4MB, ao tentar alocar PT#193, ocorre um problema na alocação uma vez que não tem mais páginas disponíveis. 
- [ ] Fazer scroll. 
- [ ] Filesystem (reservar um tempo para focar apenas no FS - ler Tanenbaum):
   - [x] Obtém próximo cluster. **Feito:** 05/08/15. Commit: [b45cae3a54b92f44fdc8227372a88fc8a5f3fc92]. 
   - [x] READ: lendo 512 bytes, o necessário para subir um programa para ser executado em ring3.
   - [ ] READ: ler vários sectores ao mesmo tempo, quando passado >512 Bytes.
   - [ ] WRITE: Implementar.
   - [ ] OPEN/CLOSE - verificar o esquema utilizado para saber se o arquivo esta open/close, se foi modificado para ser salvo.
   - [ ] Suporte para multiplos cluster.
   - [ ] Não subir tudo para a memória. Subir apenas cabeçalhos e não conteúdos.
   - [ ] show_tree esta com problema na listagem de uma diretório que esta entre dois arquivos.
- [ ] User-space/Multitask:
   - [x] 1) To ring3 e 'soft interrupt'. **Feito**: 04/11/15.
   - [x] 2) Por código acima de 0x0804A000. **Feito**: 12/11/15.
   - [x] 3) Obter os bytes de um executável e jogá-lo na memória. **Feito**: 12/11/15.
   - [x] 4) Tentar clonar a PD (para que o kernel sempre fique no mesmo lugar). **Feito**:17/11/15. Commit: [45f3c209dfeb8d844bcd299cc75613c90f5cb4ef].
   - [ ] 5) Criar uma BCP com info de CR3.
   - [ ] 6) Fazer dispatcher de int 0x08040000.
   - [ ] 7) Arrumar o PIT e fazer o escalonador.
