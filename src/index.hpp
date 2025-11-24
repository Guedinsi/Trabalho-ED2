#ifndef INDEX_HPP
#define INDEX_HPP

#include <string>
#include <unordered_map>
#include <set>
#include <vector>
#include <algorithm>

class Index {
private:
    std::unordered_map<std::string, std::set<int>> invertedIndex;
    std::unordered_map<int, std::string> idToFile;
    std::unordered_map<std::string, int> fileToId;
    int nextId;

public:
    Index() : nextId(1) {}
    
    int addDocument(const std::string& filename) {
        if (fileToId.find(filename) != fileToId.end()) {
            return fileToId[filename];
        }
        
        int docId = nextId++;
        fileToId[filename] = docId;
        idToFile[docId] = filename;
        return docId;
    }
    
    void addWordToDocument(const std::string& word, int docId) {
        invertedIndex[word].insert(docId);
    }
    
    std::set<int> getDocumentsForWord(const std::string& word) const {
        auto it = invertedIndex.find(word);
        if (it != invertedIndex.end()) {
            return it->second;
        }
        return std::set<int>();
    }
    
    std::string getFileName(int docId) const {
        auto it = idToFile.find(docId);
        if (it != idToFile.end()) {
            return it->second;
        }
        return "";
    }
    
    int getFileId(const std::string& filename) const {
        auto it = fileToId.find(filename);
        if (it != fileToId.end()) {
            return it->second;
        }
        return -1;
    }
    
    std::set<int> getAllDocumentIds() const {
        std::set<int> allIds;
        for (const auto& pair : idToFile) {
            allIds.insert(pair.first);
        }
        return allIds;
    }
    
    std::vector<std::string> getAllWords() const {
        std::vector<std::string> words;
        for (const auto& pair : invertedIndex) {
            words.push_back(pair.first);
        }
        return words;
    }
    
    // Para serialização
    friend class Serializer;
};

#endif