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

using namespace std;

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
            vector<string> terms(args.begin() + 1, args.end());
            search(terms);
        } else {
            showUsage();
        }
    }

private:
    vector<string> args;
    
    void showUsage() const {
        cout << "Uso:\n";
        cout << "  indice construir <caminho_do_diretorio>\n";
        cout << "  indice buscar <termo_de_busca> [<termo2> ...]\n";
    }
    
    void buildIndex(const string& directoryPath) {
        try {
            Index index;
            TextProcessor textProcessor;
            if (!textProcessor.loadStopWords("data/stopwords.txt")) {
                cerr << "Erro: Não foi possível carregar o arquivo data/stopwords.txt\n";
                return;
            }
            
            Indexer indexer(index, textProcessor);
            indexer.indexDirectory(directoryPath);
            
            Serializer::serialize(index, "index.dat");
            
            cout << "Índice construído e salvo em index.dat\n";
            cout << "Documentos indexados: " << index.getAllDocumentIds().size() << "\n";
            cout << "Palavras únicas no índice: " << index.getAllWords().size() << "\n";
        } catch (const exception& e) {
            cerr << "Erro durante a indexação: " << e.what() << endl;
        }
    }
    
    void search(const vector<string>& terms) {
        try {
            Index index = Serializer::deserialize("index.dat");
            QueryProcessor queryProcessor(index);
            
            vector<string> normalizedTerms;
            for (const string& term : terms) {
                normalizedTerms.push_back(TextProcessor::normalizeWord(term));
            }
            
            vector<string> results;
            if (normalizedTerms.size() == 1) {
                results = queryProcessor.querySingle(normalizedTerms[0]);
            } else {
                results = queryProcessor.queryMultiple(normalizedTerms);
            }
            
            if (results.empty()) {
                cout << "Nenhum documento encontrado.\n";
            } else {
                cout << "Documentos encontrados (" << results.size() << "):\n";
                for (const string& filename : results) {
                    cout << "  " << filename << endl;
                }
            }
        } catch (const exception& e) {
            cerr << "Erro durante a busca: " << e.what() << endl;
            cerr << "Execute primeiro: indice construir <diretorio>\n";
        }
    }
};

#endif