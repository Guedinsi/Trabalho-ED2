#ifndef TEXTPROCESSOR_HPP
#define TEXTPROCESSOR_HPP

#include <string>
#include <vector>
#include <unordered_set>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>

using namespace std;

/**
 * Classe responsável pelo processamento de texto.
 * Remove pontuações, converte para minúsculas e remove stop words.
 */
class TextProcessor {
private:
    // Conjunto de stop words (palavras comuns a serem ignoradas)
    unordered_set<string> stopWords;

public:
    TextProcessor() = default;
    
    /**
     * Carrega as stop words de um arquivo.
     * Retorna true se o arquivo foi carregado com sucesso, false caso contrário.
     */
    bool loadStopWords(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            return false;
        }
        
        string word;
        while (file >> word) {
            string normalized = normalizeWord(word);
            if (!normalized.empty()) {
                stopWords.insert(normalized);
            }
        }
        file.close();
        return true;
    }
    
    /**
     * Processa um texto: remove pontuações, converte para minúsculas,
     * tokeniza e remove stop words.
     * Retorna um vetor de palavras processadas.
     */
    vector<string> process(const string& text) const {
        vector<string> tokens = tokenize(text);
        vector<string> result;
        
        for (const string& token : tokens) {
            string normalized = normalizeWord(token);
            if (!normalized.empty() && stopWords.find(normalized) == stopWords.end()) {
                result.push_back(normalized);
            }
        }
        
        return result;
    }

    /**
     * Normaliza uma palavra: remove pontuações e converte para minúsculas.
     * Mantém a acentuação.
     * Esta função é estática para ser usada também na normalização dos termos de busca.
     */
    static string normalizeWord(const string& word) {
        string result;
        
        for (char c : word) {
            if (!isPunctuation(c)) {
                result += toLower(c);  
            }
        }
        
        return result;
    }

private:
    /**
     * Verifica se um caractere é de pontuação ASCII.
     */
    static bool isPunctuation(char c) {
        return (c >= 33 && c <= 47) ||    // !"#$%&'()*+,-./
               (c >= 58 && c <= 64) ||    // :;<=>?@
               (c >= 91 && c <= 96) ||    // [\]^_`
               (c >= 123 && c <= 126);    // {|}~
    }
    
    /**
     * Converte um caractere ASCII para minúscula.
     * Caracteres não-ASCII (como acentuados) são mantidos.
     */
    static char toLower(char c) {
        if (c >= 'A' && c <= 'Z') {
            return static_cast<char>(c + 32);  // 'a' - 'A' = 32
        }
        return c;
    }
    
    /**
     * Tokeniza o texto em palavras (split por espaços).
     */
    vector<string> tokenize(const string& text) const {
        vector<string> tokens;
        istringstream iss(text);
        string token;
        
        while (iss >> token) {
            tokens.push_back(token);
        }
        
        return tokens;
    }
};

#endif