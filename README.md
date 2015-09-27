# OS

My personal project - Operation System development.

# TODO
- [x] Problema no carregamento da imagem do kernel. Se for muito grande o tamanho do kernel, da problema. 
**Corrigido:** *27/09/15: Image carregada no endereço 0x3000.* Commit: [402033ca9daaf47093d13d2df854847f8c44036f].
- [x] Problema na função fdctrl_init_dma(). **Corrigido:** *21/09/15: A20 resolveu o problema* 
Commit: [397bd43de8a92ec94803fcc2d181192660184b81].
- [x] Problema no mecanismo de paginação. **Corrigido:** *21/09/15: A20 estava desabilitado - A20 foi habilitado.* Commit: [397bd43de8a92ec94803fcc2d181192660184b81].
- [ ] Problema ao alocar 4MB, ao tentar alocar PT#193, ocorre um problema na alocação uma vez que não tem mais páginas disponíveis. 
- [ ] Arrumar parte floopy - carregar tudo primeiro e depois acessar. 
- [ ] Fazer scroll. 
