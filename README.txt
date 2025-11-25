    Projeto: Mecanismo de Indexação de Arquivos
_______________________________________________
    
    Descrição:

Este projeto é uma aplicação de linha de comando que implementa um mecanismo de busca
para documentos de texto, com funcionalidades de:
- Indexação: processa uma coleção de arquivos ".txt", constrói um índice invertido em memória
  e o serializa em disco ("index.dat").
- Busca: carrega o índice serializado e permite consultas por palavras-chave, retornando os
  documentos que contêm os termos pesquisados.
- Avisos: mensagens de erro nos vários processos e aviso caso uma stopword foi buscada
_______________________________________________
    
    Arquivos:

- main.cpp : ponto de entrada que instancia a interface de linha de comando.
- src/commandLineInterface.hpp : interpreta argumentos e executa os comandos
  "construir" e "buscar".
- src/textProcessor.hpp : normalização de palavras e carregamento de "stopwords".
- src/indexer.hpp : percorre diretórios e popula o "Index" com tokens processados.
- src/index.hpp : estrutura do índice invertido e utilitários (lista de documentos/palavras).
- src/serializer.hpp : serialização e desserialização do índice para/desde "index.dat".
- src/queryProcessor.hpp : executa consultas simples e compostas sobre o índice.
- Makefile : compila o projeto e gera o executável indice, além de fornecer um alvo clean para remover binários/artefatos.
- data/stopwords.txt : lista de palavras irrelevantes que o TextProcessor ignora ao tokenizar e indexar documentos.
_______________________________________________
    
    Compilação:

No diretório do projeto, rode:
- make

Isso produzirá os executáveis "indice" e "main.o"
_______________________________________________
    
    Execução:

Construa o índice "index.dat", arquivo binário (ou serializado) contendo o índice invertido usado na busca, a partir de um diretório de documentos.
- ./indice construir <caminho_do_diretorio>

Exemplo:
- ./indice construir data/machado

Em seguida busque por termo(s) nos documentos desse diretório:
- ./indice buscar <termo1> [<termo2> ...]

Exemplo:
- ./indice buscar saudade
- ./indice buscar casa velho

Caso queira limpar os artefatos:
- make clean

Limitações:
- Considera símbolos comuns da lingua portuguesa UTF-8 de forma manual