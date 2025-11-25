#ifndef INDEX_HPP
#define INDEX_HPP

#include <string>
#include <unordered_map>
#include <set>
#include <vector>
#include <algorithm>

using namespace std;

class Index {
private:
    unordered_map<string, set<int>> invertedIndex;
    unordered_map<int, string> idToFile;
    unordered_map<string, int> fileToId;
    int nextId;

public:
    Index() : nextId(1) {}
    
    int addDocument(const string& filename) {
        if (fileToId.find(filename) != fileToId.end()) {
            return fileToId[filename];
        }
        
        int docId = nextId++;
        fileToId[filename] = docId;
        idToFile[docId] = filename;
        return docId;
    }
    
    void addWordToDocument(const string& word, int docId) {
        invertedIndex[word].insert(docId);
    }
    
    set<int> getDocumentsForWord(const string& word) const {
        auto it = invertedIndex.find(word);
        if (it != invertedIndex.end()) {
            return it->second;
        }
        return set<int>();
    }
    
    string getFileName(int docId) const {
        auto it = idToFile.find(docId);
        if (it != idToFile.end()) {
            return it->second;
        }
        return "";
    }
    
    int getFileId(const string& filename) const {
        auto it = fileToId.find(filename);
        if (it != fileToId.end()) {
            return it->second;
        }
        return -1;
    }
    
    set<int> getAllDocumentIds() const {
        set<int> allIds;
        for (const auto& pair : idToFile) {
            allIds.insert(pair.first);
        }
        return allIds;
    }
    
    vector<string> getAllWords() const {
        vector<string> words;
        for (const auto& pair : invertedIndex) {
            words.push_back(pair.first);
        }
        return words;
    }
    
    // Para serialização
    friend class Serializer;
};

#endif