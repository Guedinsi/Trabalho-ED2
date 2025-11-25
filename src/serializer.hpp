#ifndef SERIALIZER_HPP
#define SERIALIZER_HPP

#include "index.hpp"
#include <string>
#include <fstream>
#include <stdexcept>

using namespace std;

/**
 * Classe responsável por serializar e desserializar o índice invertido.
 * Os dados são salvos em formato binário para eficiência.
 */
class Serializer {
public:
    /**
     * Serializa o índice para um arquivo binário.
     * Salva o mapeamento de documentos e o índice invertido.
     * Lança uma exceção se não conseguir abrir o arquivo.
     */
    static void serialize(const Index& index, const string& filename) {
        ofstream file(filename, ios::binary);
        if (!file) {
            throw runtime_error("Não foi possível abrir o arquivo para escrita: " + filename);
        }
        
        size_t numDocuments = index.idToFile.size();
        file.write(reinterpret_cast<const char*>(&numDocuments), sizeof(numDocuments));
        
        for (const auto& pair : index.idToFile) {
            int docId = pair.first;
            file.write(reinterpret_cast<const char*>(&docId), sizeof(docId));
            
            const string& filenameStr = pair.second;
            size_t filenameSize = filenameStr.size();
            file.write(reinterpret_cast<const char*>(&filenameSize), sizeof(filenameSize));
            file.write(filenameStr.c_str(), filenameSize);
        }
        
        size_t numWords = index.invertedIndex.size();
        file.write(reinterpret_cast<const char*>(&numWords), sizeof(numWords));
        
        for (const auto& pair : index.invertedIndex) {
            const string& word = pair.first;
            size_t wordSize = word.size();
            file.write(reinterpret_cast<const char*>(&wordSize), sizeof(wordSize));
            file.write(word.c_str(), wordSize);
            
            const set<int>& docIds = pair.second;
            size_t numDocs = docIds.size();
            file.write(reinterpret_cast<const char*>(&numDocs), sizeof(numDocs));
            
            for (int docId : docIds) {
                file.write(reinterpret_cast<const char*>(&docId), sizeof(docId));
            }
        }
        
        file.close();
    }
    
    /**
     * Desserializa o índice de um arquivo binário.
     * Retorna um objeto Index reconstruído.
     * Lança uma exceção se não conseguir abrir o arquivo.
     */
    static Index deserialize(const string& filename) {
        ifstream file(filename, ios::binary);
        if (!file) {
            throw runtime_error("Não foi possível abrir o arquivo para leitura: " + filename);
        }
        
        Index index;
        
        size_t numDocuments;
        file.read(reinterpret_cast<char*>(&numDocuments), sizeof(numDocuments));
        
        for (size_t i = 0; i < numDocuments; ++i) {
            int docId;
            file.read(reinterpret_cast<char*>(&docId), sizeof(docId));
            
            size_t filenameSize;
            file.read(reinterpret_cast<char*>(&filenameSize), sizeof(filenameSize));
            string filename(filenameSize, ' ');
            file.read(&filename[0], filenameSize);
            
            index.idToFile[docId] = filename;
            index.fileToId[filename] = docId;
            
            if (docId >= index.nextId) {
                index.nextId = docId + 1;
            }
        }
        
        size_t numWords;
        file.read(reinterpret_cast<char*>(&numWords), sizeof(numWords));
        
        for (size_t i = 0; i < numWords; ++i) {
            size_t wordSize;
            file.read(reinterpret_cast<char*>(&wordSize), sizeof(wordSize));
            string word(wordSize, ' ');
            file.read(&word[0], wordSize);
            
            size_t numDocs;
            file.read(reinterpret_cast<char*>(&numDocs), sizeof(numDocs));
            
            set<int> docIds;
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