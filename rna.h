#ifndef rna_h
#define rna_h

#include <iostream>
#pragma once
#define _CRT_SECURE_NO_WARNINGS

typedef unsigned char Nucleotide;

const Nucleotide A = 0b00;
const Nucleotide G = 0b01;
const Nucleotide C = 0b10;
const Nucleotide T = 0b11;

class reference;

class RNA{
    friend class reference;
    int length = 0;
    int length_arr = 0;
    Nucleotide *rna = nullptr;
public:
    RNA(int length, Nucleotide);
    RNA();
    RNA(const RNA&);
    void add(Nucleotide);
    Nucleotide get(int);
    void put(int, Nucleotide);
    void put_ref(int, Nucleotide);
    RNA operator+(const RNA&);
    RNA& operator=(const RNA&);
    Nucleotide operator[](int) const;
    reference operator[](int);
    RNA& split(int);
    bool isComplimentary(const RNA&);
    bool operator==(const RNA&);
    bool operator!=(const RNA&);
    RNA operator!();
    ~RNA(){
        delete []rna;
    }
};

class reference{
    int index = 0;
    RNA *rna_ref;
public:
    reference(RNA&, int index);
    reference& operator=(const reference&);
    reference& operator=(const Nucleotide);
    operator Nucleotide();
};

#endif /* rna_h */
