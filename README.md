# OS

My personal project - Operation System development.

# TODO
- [ ] Problema no carregamento da imagem do kernel. Se for muito grande o tamanho do kernel, da problema.
- [ ] Problema na função fdctrl_init_dma().
- [x] Problema no mecanismo de paginação. **Corrigido:** *21/09/15: A20 estava desabilitado - A20 foi habilitado.*
* Commit: 397bd43de8a92ec94803fcc2d181192660184b81 .
- [ ] Problema ao alocar 4MB, ao tentar alocar PT#193, ocorre um problema na alocação uma vez que não tem mais páginas disponíveis. 
