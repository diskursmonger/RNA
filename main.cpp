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
    RNA r5(10, T);
    r4.get_length();
    r4 = r4 + r5;
    r4.get_length();
    for (int i = 1; i < 11; i++) {
        Nucleotide Nuclec = r4.get(i);
        std::cout << Nuclec << std::endl;
    }
    times(&end);
    clocks = end.tms_utime - start.tms_utime;
    printf("Time taken: %lf sec.\n", (double) clocks / clocks_per_sec);
    return 0;
}
