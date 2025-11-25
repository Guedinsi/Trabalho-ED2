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
 * Remove pontuações, converte para minúsculas, remove acentos e stop words.
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
            // Normaliza as stop words (minúsculas, sem acentos)
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
     * remove acentos, tokeniza e remove stop words.
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
     * Normaliza uma palavra: remove pontuações, converte para minúsculas e remove acentos.
     * Esta função é estática para ser usada também na normalização dos termos de busca.
     */
    static string normalizeWord(const string& word) {
        string result;
        
        // Remove pontuações
        for (char c : word) {
            if (!isPunctuation(c)) {
                result += c;  
            }
        }
        
        // Converte para minúsculas e remove acentos
        result = toLowerAndRemoveAccents(result);
        
        return result;
    }

    /**
     * Verifica se uma palavra é uma stop word.
     */
    bool isStopWord(const string& word) const {
        string normalized = normalizeWord(word);
        return stopWords.find(normalized) != stopWords.end();
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
     * Converte uma string para minúsculas e remove acentos (UTF-8).
     */
    static string toLowerAndRemoveAccents(const string& str) {
        string result;
        
        for (size_t i = 0; i < str.size(); i++) {
            unsigned char c = str[i];
            
            // Verifica se é um caractere UTF-8 multibyte (inicia com 0xC3)
            if (c == 0xC3 && i + 1 < str.size()) {
                unsigned char next = str[i + 1];
                
                // Mapeia caracteres acentuados (maiúsculos e minúsculos) para sem acento
                switch (next) {
                    // Letras minúsculas acentuadas -> sem acento
                    case 0xA1: // á
                    case 0xA0: // à
                    case 0xA2: // â
                    case 0xA3: // ã
                    case 0xA4: // ä
                        result += 'a';
                        i++;
                        break;
                    case 0xA9: // é
                    case 0xA8: // è
                    case 0xAA: // ê
                    case 0xAB: // ë
                        result += 'e';
                        i++;
                        break;
                    case 0xAD: // í
                    case 0xAC: // ì
                    case 0xAE: // î
                    case 0xAF: // ï
                        result += 'i';
                        i++;
                        break;
                    case 0xB3: // ó
                    case 0xB2: // ò
                    case 0xB4: // ô
                    case 0xB5: // õ
                    case 0xB6: // ö
                        result += 'o';
                        i++;
                        break;
                    case 0xBA: // ú
                    case 0xB9: // ù
                    case 0xBB: // û
                    case 0xBC: // ü
                        result += 'u';
                        i++;
                        break;
                    case 0xA7: // ç
                        result += 'c';
                        i++;
                        break;
                    // Letras maiúsculas acentuadas -> sem acento
                    case 0x81: // Á
                    case 0x80: // À
                    case 0x82: // Â
                    case 0x83: // Ã
                    case 0x84: // Ä
                        result += 'a';
                        i++;
                        break;
                    case 0x89: // É
                    case 0x88: // È
                    case 0x8A: // Ê
                    case 0x8B: // Ë
                        result += 'e';
                        i++;
                        break;
                    case 0x8D: // Í
                    case 0x8C: // Ì
                    case 0x8E: // Î
                    case 0x8F: // Ï
                        result += 'i';
                        i++;
                        break;
                    case 0x93: // Ó
                    case 0x92: // Ò
                    case 0x94: // Ô
                    case 0x95: // Õ
                    case 0x96: // Ö
                        result += 'o';
                        i++;
                        break;
                    case 0x9A: // Ú
                    case 0x99: // Ù
                    case 0x9B: // Û
                    case 0x9C: // Ü
                        result += 'u';
                        i++;
                        break;
                    case 0x87: // Ç
                        result += 'c';
                        i++;
                        break;
                    default:
                        // Se não for um caractere acentuado conhecido, mantém original
                        result += static_cast<char>(c);
                        break;
                }
            }
            // Converte letras ASCII maiúsculas para minúsculas
            else if (c >= 'A' && c <= 'Z') {
                result += static_cast<char>(c + 32);
            }
            // Mantém outros caracteres (minúsculos, números, etc.)
            else {
                result += static_cast<char>(c);
            }
        }
        
        return result;
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