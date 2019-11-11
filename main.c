#include "rna.h"
#include <iostream>
#include <gtest/gtest.h>
#include <sys/times.h>

int main() {
    struct tms start, end;
    long clocks_per_sec = sysconf(_SC_CLK_TCK);
    long clocks;
    times(&start);
    RNA r1(11,C);
    RNA r2(11,G);
    RNA r3;
    r3 = !r1;
    Nucleotide Nucl = r3[11];
   // std::cout << Nucl << std::endl;
    testing::InitGoogleTest();
    RUN_ALL_TESTS();
    RNA r4;
    RNA r5(1,T);
    for (int i = 0; i < 1000000; i++){
       // r4 = r4 + r5;
       r4.add(T);
        if (i % 2000 == 0) {
            std::cout << i << std::endl;
            std::cout << r4[i+1] << std::endl;
        }
    }
    r4.get_length();
   // Nucleotide Nuclec = r4.get(1000000);
   // std::cout << Nuclec << std::endl;
    times(&end);
    clocks = end.tms_utime - start.tms_utime;
    printf("Time taken: %lf sec.\n", (double)clocks / clocks_per_sec);

   // Nucleotide Nucl_5 = r4.get(7777);
   // std::cout << Nucl_5 << std::endl;
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
