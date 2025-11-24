#ifndef TEXTPROCESSOR_HPP
#define TEXTPROCESSOR_HPP

#include <string>
#include <vector>
#include <unordered_set>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>

class TextProcessor {
private:
    std::unordered_set<std::string> stopWords;

public:
    TextProcessor() = default;
    
    bool loadStopWords(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            return false;
        }
        
        std::string word;
        while (file >> word) {
            std::string normalized = normalizeWord(word);
            if (!normalized.empty()) {
                stopWords.insert(normalized);
            }
        }
        file.close();
        return true;
    }
    
    std::vector<std::string> process(const std::string& text) const {
        std::vector<std::string> tokens = tokenize(text);
        std::vector<std::string> result;
        
        for (const std::string& token : tokens) {
            std::string normalized = normalizeWord(token);
            if (!normalized.empty() && stopWords.find(normalized) == stopWords.end()) {
                result.push_back(normalized);
            }
        }
        
        return result;
    }

    // Normaliza palavra: remove pontuações e converte para minúsculas
    // MANTÉM ACENTUAÇÃO (usuário acerta a acentuação)
    static std::string normalizeWord(const std::string& word) {
        std::string result;
        
        for (char c : word) {
            if (!isPunctuation(c)) {
                result += toLower(c);  // Converte para minúscula, mantém acentos
            }
        }
        
        return result;
    }

private:
    static bool isPunctuation(char c) {
        return (c >= 33 && c <= 47) ||    // !"#$%&'()*+,-./
               (c >= 58 && c <= 64) ||    // :;<=>?@
               (c >= 91 && c <= 96) ||    // [\]^_`
               (c >= 123 && c <= 126);    // {|}~
    }
    
    // Converte para minúscula apenas caracteres ASCII A-Z
    // Preserva caracteres acentuados
    static char toLower(char c) {
        if (c >= 'A' && c <= 'Z') {
            return static_cast<char>(c + 32);  // 'a' - 'A' = 32
        }
        return c;
    }
    
    std::vector<std::string> tokenize(const std::string& text) const {
        std::vector<std::string> tokens;
        std::istringstream iss(text);
        std::string token;
        
        while (iss >> token) {
            tokens.push_back(token);
        }
        
        return tokens;
    }
};

#endif