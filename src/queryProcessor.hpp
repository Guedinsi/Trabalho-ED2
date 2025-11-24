#ifndef QUERYPROCESSOR_HPP
#define QUERYPROCESSOR_HPP

#include "index.hpp"
#include <vector>
#include <string>
#include <algorithm>

class QueryProcessor {
private:
    const Index& index;

public:
    QueryProcessor(const Index& idx) : index(idx) {}
    
    std::vector<std::string> querySingle(const std::string& word) const {
        std::vector<std::string> result;
        std::set<int> docIds = index.getDocumentsForWord(word);
        
        for (int docId : docIds) {
            result.push_back(index.getFileName(docId));
        }
        
        return result;
    }
    
    std::vector<std::string> queryMultiple(const std::vector<std::string>& words) const {
        if (words.empty()) {
            return {};
        }
        
        // Começa com o primeiro conjunto de documentos
        std::set<int> result = index.getDocumentsForWord(words[0]);
        
        // Faz a interseção com os demais conjuntos
        for (size_t i = 1; i < words.size() && !result.empty(); ++i) {
            std::set<int> current = index.getDocumentsForWord(words[i]);
            std::set<int> intersection;
            
            // Interseção manual usando algoritmos da STL
            std::set_intersection(
                result.begin(), result.end(),
                current.begin(), current.end(),
                std::inserter(intersection, intersection.begin())
            );
            
            result = intersection;
        }
        
        std::vector<std::string> fileResults;
        for (int docId : result) {
            fileResults.push_back(index.getFileName(docId));
        }
        
        return fileResults;
    }
};

#endif