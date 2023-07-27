#include <iostream>
#include <fstream>
#include <vector>
#include "HCTree.hpp"
#include "Helper.hpp"


int main(int argc, char* argv[]) {
    // your program's main execution code

    //Parse the command line arguments and throw an error message if the user runs your program incorrectly
    if (argc != 3) {
        error("Usage: ./compress [input_file] [output_file]");
    }

    //Open the input file for reading
    const char* input_file = argv[1];
    const char* output_file = argv[2];
   
    FancyInputStream input(input_file);
    if (!input.good()) {
        error("Error in opening input file");
    }
    FancyOutputStream output(output_file);   
    if (!output.good()) {
        error("Error in opening output file for writing");
    }

    if(input.filesize() == 0){
        return 0;
    }
    //Read bytes from the file. Count the number of occurrences of each byte value
    vector<int> byteCounts(256, 0);
    int byte;
    while ((byte = input.read_byte()) != -1) {
        byteCounts[byte]++;
    }
   
    //file header
    for (int count : byteCounts) {
        output.write_int(count);    // count of each 
    }

    HCTree hftree;
    hftree.build(byteCounts);
    input.reset();

    //translate each byte from the input file into its code, and append after the header
    while ((byte = input.read_byte()) != -1) {
        hftree.encode(byte, output);
    }

    return 0;
}