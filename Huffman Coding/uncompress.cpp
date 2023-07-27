#include <iostream>
#include <fstream>
#include <vector>
#include "HCTree.hpp"
#include "Helper.hpp"

int main(int argc, char* argv[]) {
    // your program's main execution code

    //Open the input/output file for reading
    const char* input_file = argv[1];
    const char* output_file = argv[2];
   
    FancyInputStream input(input_file);
    if (!input.good()) {
        error("Error in opening input file");
        return 1;
    }

    FancyOutputStream output(output_file);
    if (!output.good()) {
        error("Error in opening output file for writing");
        return 1;
    }
    
    // check empty file
    if(input.filesize() == 0){
        return 0;
    }

    vector<int> byteCounts(256, 0); // freq of each symbol
    for(long unsigned int i = 0; i < byteCounts.size(); i++){
        byteCounts[i] = input.read_int();
    }

    HCTree hftree;
    hftree.build(byteCounts);

    while(input.good()) {
        unsigned char symbol = hftree.decode(input);
        //cout << "char sym: " << symbol << endl;
        if(symbol != '\0'){
            output.write_byte(symbol);
        }
        output.flush();
        output.flush_bitwise();
    }

    return 0;

}