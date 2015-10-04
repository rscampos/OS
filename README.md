# OS

My personal project - Operation System development.

# TODO
- [x] Problema no carregamento da imagem do kernel. Se for muito grande o tamanho do kernel, da problema. 
**Corrigido:** *27/09/15: Image carregada no endereço 0x3000.* Commit: [402033ca9daaf47093d13d2df854847f8c44036f].
- [x] Problema na função fdctrl_init_dma(). **Corrigido:** *21/09/15: A20 resolveu o problema* 
Commit: [397bd43de8a92ec94803fcc2d181192660184b81].
- [x] Problema no mecanismo de paginação. **Corrigido:** *21/09/15: A20 estava desabilitado - A20 foi habilitado.* Commit: [397bd43de8a92ec94803fcc2d181192660184b81].
- [x] Arrumar parte floopy - carregar tudo primeiro e depois acessar. **Corrigido** *30/09/15: problema conversão lba->chs* Commit: [ae3bcbc3fc6ebbdb05fe4d5c0617d5f3f6249f70].
- [ ] Carregar arquivos com multiplos clusters. Testar mais pra frente. 
- [ ] Problema ao alocar 4MB, ao tentar alocar PT#193, ocorre um problema na alocação uma vez que não tem mais páginas disponíveis. 
- [ ] Fazer scroll. 
