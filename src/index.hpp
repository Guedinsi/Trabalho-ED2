#ifndef INDEX_HPP
#define INDEX_HPP

#include <string>
#include <unordered_map>
#include <set>
#include <vector>
#include <algorithm>

using namespace std;
/**
 * Classe que representa o índice invertido.
 * Armazena mapeamentos de palavras para documentos e vice-versa.
 */
class Index {
private:
    // Índice invertido: palavra -> conjunto de IDs de documentos que contêm a palavra
    unordered_map<string, set<int>> invertedIndex;

    // Mapeamento de ID do documento para o nome do arquivo
    unordered_map<int, string> idToFile;

    // Mapeamento de nome do arquivo para ID do documento    
    unordered_map<string, int> fileToId;

    // Próximo ID a ser atribuído a um documento
    int nextId;

public:
    Index() : nextId(1) {}

    /**
     * Adiciona um documento ao índice e retorna seu ID.
     * Se o documento já existe, retorna o ID existente.
     */
    int addDocument(const string& filename) {
        if (fileToId.find(filename) != fileToId.end()) {
            return fileToId[filename];
        }
        
        int docId = nextId++;
        fileToId[filename] = docId;
        idToFile[docId] = filename;
        return docId;
    }
    
    /**
     * Adiciona uma palavra a um documento no índice.
     * Se a palavra não existia, é criada uma nova entrada.
     */
    void addWordToDocument(const string& word, int docId) {
        invertedIndex[word].insert(docId);
    }
   
    /**
     * Retorna o conjunto de IDs de documentos que contêm a palavra.
     * Se a palavra não existe, retorna um conjunto vazio.
     */
    set<int> getDocumentsForWord(const string& word) const {
        auto it = invertedIndex.find(word);
        if (it != invertedIndex.end()) {
            return it->second;
        }
        return set<int>();
    }
    
    /**
     * Retorna o nome do arquivo correspondente ao ID do documento.
     * Se o ID não existe, retorna string vazia.
     */
    string getFileName(int docId) const {
        auto it = idToFile.find(docId);
        if (it != idToFile.end()) {
            return it->second;
        }
        return "";
    }
    
    /**
     * Retorna o ID do documento correspondente ao nome do arquivo.
     * Se o arquivo não existe, retorna -1.
     */
    int getFileId(const string& filename) const {
        auto it = fileToId.find(filename);
        if (it != fileToId.end()) {
            return it->second;
        }
        return -1;
    }
    
    /**
     * Retorna um conjunto com todos os IDs de documentos indexados.
     */
    set<int> getAllDocumentIds() const {
        set<int> allIds;
        for (const auto& pair : idToFile) {
            allIds.insert(pair.first);
        }
        return allIds;
    }
    
    /**
     * Retorna um vetor com todas as palavras no índice.
     */
    vector<string> getAllWords() const {
        vector<string> words;
        for (const auto& pair : invertedIndex) {
            words.push_back(pair.first);
        }
        return words;
    }
    
    friend class Serializer;
};

#endif