#ifndef INDEXER_HPP
#define INDEXER_HPP

#include "index.hpp"
#include "textProcessor.hpp"
#include <filesystem>
#include <fstream>
#include <sstream>

namespace fs = std::filesystem;

class Indexer {
private:
    Index& index;
    TextProcessor& textProcessor;

public:
    Indexer(Index& idx, TextProcessor& tp) : index(idx), textProcessor(tp) {}
    
    void indexDirectory(const std::string& directoryPath) {
        for (const auto& entry : fs::recursive_directory_iterator(directoryPath)) {
            if (entry.is_regular_file() && entry.path().extension() == ".txt") {
                std::string filename = entry.path().string();
                
                int docId = index.addDocument(filename);
                
                std::ifstream file(filename);
                if (!file.is_open()) {
                    continue; // Pula arquivos que não podem ser abertos
                }
                
                std::stringstream buffer;
                buffer << file.rdbuf();
                std::string content = buffer.str();
                file.close();
                
                std::vector<std::string> words = textProcessor.process(content);
                
                for (const std::string& word : words) {
                    index.addWordToDocument(word, docId);
                }
            }
        }
    }
};

#endif