#ifndef QUERYPROCESSOR_HPP
#define QUERYPROCESSOR_HPP

#include "index.hpp"
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

class QueryProcessor {
private:
    const Index& index;

public:
    QueryProcessor(const Index& idx) : index(idx) {}
    
    vector<string> querySingle(const string& word) const {
        vector<string> result;
        set<int> docIds = index.getDocumentsForWord(word);
        
        for (int docId : docIds) {
            result.push_back(index.getFileName(docId));
        }
        
        return result;
    }
    
    vector<string> queryMultiple(const vector<string>& words) const {
        if (words.empty()) {
            return {};
        }
        
        // Começa com o primeiro conjunto de documentos
        set<int> result = index.getDocumentsForWord(words[0]);
        
        // Faz a interseção com os demais conjuntos
        for (size_t i = 1; i < words.size() && !result.empty(); ++i) {
            set<int> current = index.getDocumentsForWord(words[i]);
            set<int> intersection;
            
            // Interseção manual usando algoritmos da STL
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