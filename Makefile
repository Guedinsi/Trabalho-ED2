CXX = g++
# Flags de compilação:
# -std=c++17: Usa o padrão C++17 (necessário para <filesystem>)
# -Wall: Ativa todos os warnings comuns
# -Wextra: Warnings extras
# -pedantic: Conformidade rigorosa com o padrão
# -I.: Inclui o diretório atual no path de includes
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic -I.
TARGET = indice
SOURCES = main.cpp
OBJECTS = $(SOURCES:.cpp=.o)

$(TARGET): $(OBJECTS)
	@echo "================================================"
	@echo "Compilação concluída com sucesso!"
	@echo "Executável: ./$(TARGET)"
	@echo ""
	@echo "Uso:"
	@echo "  ./$(TARGET) construir <caminho_do_diretorio>"
	@echo "  ./$(TARGET) buscar <termo_de_busca> [<termo2> ...]"
	@echo ""
	@echo "Exemplos:"
	@echo "  ./$(TARGET) construir data/machado"
	@echo "  ./$(TARGET) buscar capitu"
	@echo "  ./$(TARGET) buscar dom casmurro"
	@echo "================================================"
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET) index.dat

.PHONY: clean