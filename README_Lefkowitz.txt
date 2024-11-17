Neste programa tem-se como objetivo ler um arquivo binário com dados do tipo "Pessoa", organizar quais apresentam a mesma idade e, posteriormente, permitir que visualize todos que apresentem a idade desejada.

Ordem dos programas a serem executados: Lef_gerador -> Lefkowitz.

A parte de criar o arquivo e inserir os dados esta no programa Lef_gerador.
    Caso queira alterar a quantidade de dados inseridos é só alterar o valor de MAX (linha 9)
    Quanto a linha 10, temos CARGA, esta é apenas uma variável acelerar a iserção das idades, sendo elas cod % CARGA

A struct Pessoa contem os tipos
    cod - Baseado na posição dele no arquivo    
    nome - Nome da "pessoa" esta sendo adicionado na linha 108 basicamente somando o código à letra 'A'
    idade - Assim como explicado anteriormente, esta funcionado baseado no id % carga (linha 106)
    prox - Se inicia com -1, devido ao fato de ainda não ter sido organizado os valores dos saltos, posteriormente irá se alterar isso.

Nos loops se utiliza o "fread > 0" como ponto verificador pelo fato do fread retornar o valor que foi lido, logo se for nulo significa que chegou ao fim do arquivo.

As funções LeIdade e OrganizaSaltos contidas no arquivo Lefkowitz.c serão explicadas a baixo:

    LeIdade:
            Recebe-se a idade desejada pelo usuário a procurar, após isto a procura no arquivo, ao encontrar a primeira (caso encontre), encerra-se esse loop e começa a busca por todas as iguais utilizando os saltos contidos no campo "prox" com o fseek (linha 109).
            Caso não encontre, é printada essa informação.

    OrganizaSaltos:
            Verifica um a um onde esta o mais próximo com a mesma idade, após o fazer retorna para o termo seguinte a ele, nesta área não se da saltos buscando todos os termos iguais, mais sim executa em forma de FiFo.
            Caso encontre valores de idade iguais configura o prox para o salto correspondente, em caso negativo, o deixa como "-1"
            Os avanços são definidos baseado no busca_pos, o qual salva qual era o tipo "Pessoa" que estava sendo lido e ao somar um, avança inteiramente, utilizando o fseek (linha 152)
            Vale ressaltar que esta função informa como eram os saltos antes de organizar e após organizar, sequencialmente.


