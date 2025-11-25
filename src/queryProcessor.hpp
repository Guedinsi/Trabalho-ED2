#ifndef QUERYPROCESSOR_HPP
#define QUERYPROCESSOR_HPP

#include "index.hpp"
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

/**
 * Classe responsável por processar consultas no índice invertido.
 * Suporta consultas com uma única palavra ou múltiplas palavras (operação AND).
 */
class QueryProcessor {
private:
    // Referência para o índice invertido (const, pois não modifica)
    const Index& index;

public:
    QueryProcessor(const Index& idx) : index(idx) {}
    
    /**
     * Processa uma consulta com uma única palavra.
     * Retorna os nomes dos arquivos que contêm a palavra.
     */
    vector<string> querySingle(const string& word) const {
        vector<string> result;
        set<int> docIds = index.getDocumentsForWord(word);
        
        for (int docId : docIds) {
            result.push_back(index.getFileName(docId));
        }
        
        return result;
    }
    
    /**
     * Processa uma consulta com múltiplas palavras (operação AND).
     * Retorna os nomes dos arquivos que contêm todas as palavras.
     */
    vector<string> queryMultiple(const vector<string>& words) const {
        if (words.empty()) {
            return {};
        }
        
        set<int> result = index.getDocumentsForWord(words[0]);
        
        for (size_t i = 1; i < words.size() && !result.empty(); ++i) {
            set<int> current = index.getDocumentsForWord(words[i]);
            set<int> intersection;
            
            set_intersection(
                result.begin(), result.end(),
                current.begin(), current.end(),
                inserter(intersection, intersection.begin())
            );
            
            result = intersection;
        }
        
        vector<string> fileResults;
        for (int docId : result) {
            fileResults.push_back(index.getFileName(docId));
        }
        
        return fileResults;
    }
};

#endif