#include "rna.hpp"
#include "rna.cpp"

int main() {
    RNA r1(11,C);
    RNA r2(11,G);
    RNA r3;
    r3 = !r1;
    Nucleotide Nucl = r3[11];
    std::cout << Nucl << std::endl;
    //RNA r3 = r1 + r2;
    //r1 = r1 + r2;
    //r1.add(T);
    // Nucleotide Nuclelele = r1[12];
    // std::cout << Nuclelele << std::endl;
    // bool checking = r1.isComplimentary(r2);
    // std::cout << checking << std::endl;
    //  r2 = r1 + r2;
    // r1 = r1.split(3);
    //r1[3] = G;
    // r1[3] = r2[2];
    // if (r1 != r2){
    //     std::cout << "hello" << std::endl;
    // }
    //Nucleotide Nucl = r1[3];
    //std::cout << Nucl << std::endl;
    /*Nucleotide Nucl = r1[1];
     Nucleotide Nucl1 = r2[16];
     std::cout << Nucl << std::endl;
     std::cout << Nucl1 << std::endl;
     */
    return 0;
}
