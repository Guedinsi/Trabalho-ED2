#ifndef COMMANDLINEINTERFACE_HPP
#define COMMANDLINEINTERFACE_HPP

#include "index.hpp"
#include "textProcessor.hpp"
#include "indexer.hpp"
#include "serializer.hpp"
#include "queryProcessor.hpp"
#include <iostream>
#include <vector>
#include <string>

class CommandLineInterface {
public:
    CommandLineInterface(int argc, char* argv[]) {
        for (int i = 1; i < argc; ++i) {
            args.push_back(argv[i]);
        }
    }
    
    void run() {
        if (args.empty()) {
            showUsage();
            return;
        }
        
        if (args[0] == "construir") {
            if (args.size() != 2) {
                showUsage();
                return;
            }
            buildIndex(args[1]);
        } else if (args[0] == "buscar") {
            if (args.size() < 2) {
                showUsage();
                return;
            }
            std::vector<std::string> terms(args.begin() + 1, args.end());
            search(terms);
        } else {
            showUsage();
        }
    }

private:
    std::vector<std::string> args;
    
    void showUsage() const {
        std::cout << "Uso:\n";
        std::cout << "  indice construir <caminho_do_diretorio>\n";
        std::cout << "  indice buscar <termo_de_busca> [<termo2> ...]\n";
    }
    
    void buildIndex(const std::string& directoryPath) {
        try {
            Index index;
            TextProcessor textProcessor;
            if (!textProcessor.loadStopWords("data/stopwords.txt")) {
                std::cerr << "Erro: Não foi possível carregar o arquivo data/stopwords.txt\n";
                return;
            }
            
            Indexer indexer(index, textProcessor);
            indexer.indexDirectory(directoryPath);
            
            Serializer::serialize(index, "index.dat");
            
            std::cout << "Índice construído e salvo em index.dat\n";
            std::cout << "Documentos indexados: " << index.getAllDocumentIds().size() << "\n";
            std::cout << "Palavras únicas no índice: " << index.getAllWords().size() << "\n";
        } catch (const std::exception& e) {
            std::cerr << "Erro durante a indexação: " << e.what() << std::endl;
        }
    }
    
    void search(const std::vector<std::string>& terms) {
        try {
            Index index = Serializer::deserialize("index.dat");
            QueryProcessor queryProcessor(index);
            
            // NOVO: Normalizar os termos de busca
            std::vector<std::string> normalizedTerms;
            for (const std::string& term : terms) {
                normalizedTerms.push_back(TextProcessor::normalizeWord(term));
            }
            
            std::vector<std::string> results;
            if (normalizedTerms.size() == 1) {
                results = queryProcessor.querySingle(normalizedTerms[0]);
            } else {
                results = queryProcessor.queryMultiple(normalizedTerms);
            }
            
            if (results.empty()) {
                std::cout << "Nenhum documento encontrado.\n";
            } else {
                std::cout << "Documentos encontrados (" << results.size() << "):\n";
                for (const std::string& filename : results) {
                    std::cout << "  " << filename << std::endl;
                }
            }
        } catch (const std::exception& e) {
            std::cerr << "Erro durante a busca: " << e.what() << std::endl;
            std::cerr << "Execute primeiro: indice construir <diretorio>\n";
        }
    }
};

#endif