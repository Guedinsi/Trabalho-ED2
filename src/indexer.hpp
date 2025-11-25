#ifndef INDEXER_HPP
#define INDEXER_HPP

#include "index.hpp"
#include "textProcessor.hpp"
#include <filesystem>
#include <fstream>
#include <sstream>

using namespace std;
namespace fs = filesystem;

class Indexer {
private:
    Index& index;
    TextProcessor& textProcessor;

public:
    Indexer(Index& idx, TextProcessor& tp) : index(idx), textProcessor(tp) {}
    
    void indexDirectory(const string& directoryPath) {
        for (const auto& entry : fs::recursive_directory_iterator(directoryPath)) {
            if (entry.is_regular_file() && entry.path().extension() == ".txt") {
                string filename = entry.path().string();
                
                int docId = index.addDocument(filename);
                
                ifstream file(filename);
                if (!file.is_open()) {
                    continue; // Pula arquivos que não podem ser abertos
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