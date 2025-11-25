#include "src/commandLineInterface.hpp"

/*
Cria a interface de linha de comando e a executa.
*/

int main(int argc, char* argv[]) {
    CommandLineInterface cli(argc, argv);
    cli.run();
    return 0;
}