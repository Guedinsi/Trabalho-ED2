#include "src/commandLineInterface.hpp"

int main(int argc, char* argv[]) {
    CommandLineInterface cli(argc, argv);
    cli.run();
    return 0;
}