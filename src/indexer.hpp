#ifndef INDEXER_HPP
#define INDEXER_HPP

#include "index.hpp"
#include "textProcessor.hpp"
#include <filesystem>
#include <fstream>
#include <sstream>

using namespace std;
namespace fs = filesystem;

/**
 * Classe responsável por indexar um diretório de documentos.
 * Utiliza o TextProcessor para processar o conteúdo dos arquivos
 * e preenche o índice invertido.
 */
class Indexer {
private:
    // Referência para o índice que será preenchido
    Index& index;
    // Referência para o processador de texto
    TextProcessor& textProcessor;

public:
    Indexer(Index& idx, TextProcessor& tp) : index(idx), textProcessor(tp) {}
    
    /**
     * Indexa todos os arquivos .txt no diretório especificado (recursivamente).
     * Para cada arquivo, lê o conteúdo, processa o texto e adiciona as palavras ao índice.
     */
    void indexDirectory(const string& directoryPath) {
        for (const auto& entry : fs::recursive_directory_iterator(directoryPath)) {
            if (entry.is_regular_file() && entry.path().extension() == ".txt") {
                string filename = entry.path().string();
                
                int docId = index.addDocument(filename);
                
                ifstream file(filename);
                if (!file.is_open()) {
                    continue; 
                }
                
                stringstream buffer;
                buffer << file.rdbuf();
                string content = buffer.str();
                file.close();
                
                vector<string> words = textProcessor.process(content);
                
                for (const string& word : words) {
                    index.addWordToDocument(word, docId);
                }
            }
        }
    }
};

#endif