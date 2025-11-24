#ifndef SERIALIZER_HPP
#define SERIALIZER_HPP

#include "index.hpp"
#include <string>
#include <fstream>
#include <stdexcept>

class Serializer {
public:
    static void serialize(const Index& index, const std::string& filename) {
        std::ofstream file(filename, std::ios::binary);
        if (!file) {
            throw std::runtime_error("Não foi possível abrir o arquivo para escrita: " + filename);
        }
        
        // Serializa o mapeamento de documentos
        size_t numDocuments = index.idToFile.size();
        file.write(reinterpret_cast<const char*>(&numDocuments), sizeof(numDocuments));
        
        for (const auto& pair : index.idToFile) {
            int docId = pair.first;
            file.write(reinterpret_cast<const char*>(&docId), sizeof(docId));
            
            const std::string& filenameStr = pair.second;
            size_t filenameSize = filenameStr.size();
            file.write(reinterpret_cast<const char*>(&filenameSize), sizeof(filenameSize));
            file.write(filenameStr.c_str(), filenameSize);
        }
        
        // Serializa o índice invertido
        size_t numWords = index.invertedIndex.size();
        file.write(reinterpret_cast<const char*>(&numWords), sizeof(numWords));
        
        for (const auto& pair : index.invertedIndex) {
            const std::string& word = pair.first;
            size_t wordSize = word.size();
            file.write(reinterpret_cast<const char*>(&wordSize), sizeof(wordSize));
            file.write(word.c_str(), wordSize);
            
            const std::set<int>& docIds = pair.second;
            size_t numDocs = docIds.size();
            file.write(reinterpret_cast<const char*>(&numDocs), sizeof(numDocs));
            
            for (int docId : docIds) {
                file.write(reinterpret_cast<const char*>(&docId), sizeof(docId));
            }
        }
        
        file.close();
    }
    
    static Index deserialize(const std::string& filename) {
        std::ifstream file(filename, std::ios::binary);
        if (!file) {
            throw std::runtime_error("Não foi possível abrir o arquivo para leitura: " + filename);
        }
        
        Index index;
        
        // Desserializa o mapeamento de documentos
        size_t numDocuments;
        file.read(reinterpret_cast<char*>(&numDocuments), sizeof(numDocuments));
        
        for (size_t i = 0; i < numDocuments; ++i) {
            int docId;
            file.read(reinterpret_cast<char*>(&docId), sizeof(docId));
            
            size_t filenameSize;
            file.read(reinterpret_cast<char*>(&filenameSize), sizeof(filenameSize));
            std::string filename(filenameSize, ' ');
            file.read(&filename[0], filenameSize);
            
            index.idToFile[docId] = filename;
            index.fileToId[filename] = docId;
            
            if (docId >= index.nextId) {
                index.nextId = docId + 1;
            }
        }
        
        // Desserializa o índice invertido
        size_t numWords;
        file.read(reinterpret_cast<char*>(&numWords), sizeof(numWords));
        
        for (size_t i = 0; i < numWords; ++i) {
            size_t wordSize;
            file.read(reinterpret_cast<char*>(&wordSize), sizeof(wordSize));
            std::string word(wordSize, ' ');
            file.read(&word[0], wordSize);
            
            size_t numDocs;
            file.read(reinterpret_cast<char*>(&numDocs), sizeof(numDocs));
            
            std::set<int> docIds;
            for (size_t j = 0; j < numDocs; ++j) {
                int docId;
                file.read(reinterpret_cast<char*>(&docId), sizeof(docId));
                docIds.insert(docId);
            }
            
            index.invertedIndex[word] = docIds;
        }
        
        file.close();
        return index;
    }
};

#endif