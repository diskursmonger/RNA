#include "rna.h"
#include <iostream>
#include <gtest/gtest.h>
#include <sys/times.h>


int main() {
    struct tms start, end;
    long clocks_per_sec = sysconf(_SC_CLK_TCK);
    long clocks;
    times(&start);
    testing::InitGoogleTest();
    RUN_ALL_TESTS();
    RNA r4;
    RNA r5(1, T);
    for (int i = 1; i <= 1000000; i++) {
        // r4 = r4 + r5;
        r4.add(T);
        if (i % 2000 == 0) {
            // std::cout << i << std::endl;
            std::cout << r4[i] << std::endl;
        }
    }
    r4.get_length();
    // Nucleotide Nuclec = r4.get(1000000);
    // std::cout << Nuclec << std::endl;
    times(&end);
    clocks = end.tms_utime - start.tms_utime;
    printf("Time taken: %lf sec.\n", (double) clocks / clocks_per_sec);
    return 0;
}
