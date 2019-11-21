#include "rna.h"
#include <iostream>


inline int ind_block(int i) {
    return (i / 4);
}

inline int ind_in_block(int i) {
    return (i + 1) % 4;
}

Nucleotide do_Mask(Nucleotide Nucl, int ind_in_block) {
    //put a nucl in the right place
    switch (ind_in_block) {
        case 1:
            Nucl = Nucl << 6;
            break;
        case 2:
            Nucl = Nucl << 4;
            break;
        case 3:
            Nucl = Nucl << 2;
            break;
        default:
            break;
    }
    return Nucl;
}

unsigned char convert(Nucleotide Nucl) {
    //convert nucl in char (ex. 'A') format
    switch (Nucl) {
        case A:
            Nucl = 'A';
            break;
        case G:
            Nucl = 'G';
            break;
        case C:
            Nucl = 'C';
            break;
        case T:
            Nucl = 'T';
            break;
        default:
            std::cout << "Wrong symbol." << std::endl;
            exit(0);
    }
    return Nucl;
}

Nucleotide convert_vise(unsigned char Nucl) {
    //convert nucl in Nucleotide (ex. Nucleotide A = 0b00) format
    switch (Nucl) {
        case 'A':
            Nucl = A;
            break;
        case 'G':
            Nucl = G;
            break;
        case 'C':
            Nucl = C;
            break;
        case 'T':
            Nucl = T;
            break;
        default:
            break;
    }
    return Nucl;
}

reference::reference(RNA& rna1, int ind) {
    index = ind;
    rna_ref = &rna1;
}

reference::operator Nucleotide() {
    //вытаскиваем нуклеотид который хранится в r1[index]
    unsigned char Nucl = (*rna_ref).get(index);
    return Nucl;
}

reference RNA::operator[](int index) {
    // r1[4]
    reference Nucl_ref(*this, index);
    //создаем объект reference r1[index]
    return Nucl_ref;
}

reference& reference::operator=(const Nucleotide Nucl) {
    //r1[2] = T;
    (*rna_ref).put_ref(index, Nucl);
    return *this;
}

reference& reference::operator=(const reference& nucl_here) {
    //r1[2] = r2[4];
    //r1.operator[]2.operator=(r2.operator[]4);
    Nucleotide Nucl = nucl_here.rna_ref->get(nucl_here.index);
    Nucl = convert_vise(Nucl);
    (*rna_ref).put_ref(index, Nucl);
    return *this;
}

RNA& RNA::put(int index, Nucleotide Nucl){ //поступает на вход не менее 1!
    --index;
    if (index < 0) {
        std::cout << "Wrong number of nucleotide." << std::endl;
        exit(0);
    }
    int ind_bloc = ind_block(index);
    int ind_in_bloc = ind_in_block(index);
    if (ind_in_bloc == 0) ind_in_bloc = 4;
    Nucleotide cur_n = do_Mask(Nucl, ind_in_bloc);
    if (ind_in_bloc == 1) rna[ind_bloc] = A;
    rna[ind_bloc] = rna[ind_bloc] | cur_n;
    return *this;
}

RNA& RNA::put_ref(int index, Nucleotide Nucl){ //поступает на вход не менее 1!
    --index;
    if (index < 0) {
        std::cout << "Wrong number of nucleotide." << std::endl;
        exit(0);
    }
    Nucleotide Nucl_in_block[4];
    int ind_bloc = ind_block(index);
    int ind_in_bloc = ind_in_block(index);
    int ind;
    if (ind_in_bloc == 0) ind_in_bloc = 4;
    for (int i = 0; i < 4; i++) { //i- number in block
        ind = (i + 1) + (ind_bloc * 4);
        if (ind <= length)
            Nucl_in_block[i] = (*this).get(ind);
    }
    Nucl_in_block[ind_in_bloc - 1] = Nucl;
    for (int i = 0; i < 4; i++) {
        Nucleotide Nucl = Nucl_in_block[i];
        Nucl = convert_vise(Nucl);
        ind = (i + 1) + (ind_bloc * 4);
        if (ind <= length) (*this).put(ind, Nucl);
    }
    index++;
    (*this).put(index, Nucl);
    return *this;
}

unsigned char RNA::operator[](int index) const {
    if (length < index) {
        std::cout << "You have only " << length << " nucleotides." << std::endl;
        exit(0);
    }
    --index;
    if (index < 0) {
        std::cout << "Wrong number of nucleotide." << std::endl;
        exit(0);
    }
    int ind_bloc = ind_block(index);
    int ind_in_bloc = ind_in_block(index);
    if (ind_in_bloc == 0) ind_in_bloc = 4;
    Nucleotide Nucl = rna[ind_bloc];
    switch (ind_in_bloc) {
        case 1:
            Nucl = Nucl >> 6;
            break;
        case 2:
            Nucl = Nucl << 2;
            Nucl = Nucl >> 6;
            break;
        case 3:
            Nucl = Nucl << 4;
            Nucl = Nucl >> 6;
            break;
        case 4:
            Nucl = Nucl << 6;
            Nucl = Nucl >> 6;
        default:
            break;
    }
    Nucl = convert(Nucl);
    return Nucl;
}

RNA& RNA::operator=(const RNA& rna1) {
    if (&rna == &rna1.rna) {
        return *this;
    }
    blocks = length / 4 + 1;
    if (length != rna1.length) {
        delete[] rna;
        rna = new Nucleotide[blocks];
    }
    length = rna1.length;
    for (int i = 1; i <= length; i++) {
        Nucleotide Nucl = rna1.get(i);
        Nucl = convert_vise(Nucl);
        (*this).put(i, Nucl);
    }
    return *this;
}

RNA RNA::operator+(const RNA& rna1) {
    RNA rna_temporary = (*this);
    if (rna1.length == 1) {
        rna_temporary.add(rna1[1]);
    }
    else {
        rna_temporary.length = length + rna1.length;
        int max_nucl = blocks * 4;
        if (rna_temporary.length >= max_nucl) {
            rna_temporary.blocks = rna_temporary.length / 2;
            delete[] rna_temporary.rna;
            rna_temporary.rna = new Nucleotide[rna_temporary.blocks];
            for (int i = 1; i <= length; i++) {
                Nucleotide Nucl = (*this)[i];
                Nucl = convert_vise(Nucl);
                rna_temporary.put(i, Nucl);
            }
        }
        int k = 1;
        for (int i = length + 1; i <= rna_temporary.length; i++) {
            Nucleotide Nucl = rna1[k];
            Nucl = convert_vise(Nucl);
            rna_temporary.put(i, Nucl);
            k++;
        }
    }
    return rna_temporary;
}

RNA& RNA::split(int index) {
    RNA rna_temporary = (*this);
    delete[]rna;
    this->blocks = length / 4 + 1;
    rna = new Nucleotide[blocks];
    int k = 1;
    for (int i = index + 1; i <= length; i++) { //begin from index+1
        Nucleotide Nucl = rna_temporary[i];
        Nucl = convert_vise(Nucl);
        (*this).put(k, Nucl);
        k++;
    }
    this->length = rna_temporary.length - index;
    return *this;
}

unsigned char RNA::get(int index) const{
    if (length < index) {
        std::cout << "You have only " << length << " nucleotides." << std::endl;
        exit(0);
    }
    --index;
    if (index < 0) {
        std::cout << "Wrong number of nucleotide." << std::endl;
        exit(0);
    }
    int ind_bloc = ind_block(index);
    int ind_in_bloc = ind_in_block(index);
    if (ind_in_bloc == 0) ind_in_bloc = 4;
    Nucleotide Nucl = rna[ind_bloc];
    switch (ind_in_bloc) {
        case 1:
            Nucl = Nucl >> 6;
            break;
        case 2:
            Nucl = Nucl << 2;
            Nucl = Nucl >> 6;
            break;
        case 3:
            Nucl = Nucl << 4;
            Nucl = Nucl >> 6;
            break;
        case 4:
            Nucl = Nucl << 6;
            Nucl = Nucl >> 6;
        default:
            break;
    }
    Nucl = convert(Nucl);
    return Nucl;
}

bool check(unsigned char nucl1, unsigned char nucl2) {
    switch (nucl1) {
        case 'A':
            if (nucl2 == 'T') return true;
        case 'G':
            if (nucl2 == 'C') return true;
        case 'C':
            if (nucl2 == 'G') return true;
        case 'T':
            if (nucl2 == 'A') return true;
        default:
            break;
    }
    return false;
}

RNA& RNA::add(Nucleotide Nucl){
    length++;
    int max_nucl = blocks * 4;
    if (length > max_nucl) {
        RNA rna_temporary = (*this);
        blocks *= 2;
        delete[] rna;
        rna = new Nucleotide[blocks];
        for (int i = 1; i < length; i++) {
            //Nucleotide n = rna_temporary[i];
            Nucleotide n = rna_temporary.get(i);
            n = convert_vise(n);
            (*this).put(i, n);
        }
    }
    Nucl = convert_vise(Nucl);
    (*this).put(length, Nucl);
    return *this;
}

bool RNA::isComplimentary(const RNA& rna1) {
    // A (00) - T (11) /////// G (01) - C (10)
    if (length != rna1.length) return false;
    int count_comp = 0;
    int isComp;
    for (int i = 1; i <= length; i++) {
        isComp = check((*this)[i], rna1[i]);
        if (isComp == 1) count_comp++;
    }
    if (count_comp != length) return false;
    return true;
}

bool RNA::operator==(const RNA& rna1) {
    if (length != rna1.length) return false;
    for (int i = 1; i <= length; i++) {
        if ((*this)[i] != rna1[i]) {
            return false;
        }
    }
    return true;
}

int min(int a, int b) {
    if (a < b) return a;
    else return b;
}

bool RNA::operator!=(const RNA& rna1) {
    int min_length = min(length, rna1.length);
    int count_matches = 0;
    for (int i = 1; i <= min_length; i++) {
        if ((*this)[i] == rna1[i]) {
            count_matches++;
        }
    }
    if (length == rna1.length && count_matches == length) return false;
    return true;
}

Nucleotide compNucl(unsigned char Nucl) {
    switch (Nucl) {
        case 'A':
            Nucl = T;
            break;
        case 'G':
            Nucl = C;
            break;
        case 'C':
            Nucl = G;
            break;
        case 'T':
            Nucl = A;
            break;
        default:
            break;
    }
    return Nucl;
}

RNA RNA::operator!() {
    RNA rna1;
    delete[] rna1.rna;
    rna1.rna = new Nucleotide[blocks];
    rna1.length = length;
    rna1.blocks = blocks;
    for (int i = 1; i <= length; i++) {
        Nucleotide Nucl = compNucl((*this)[i]);
        rna1.put(i, Nucl);
    }
    return rna1;
}

RNA::RNA(const RNA& rna1) {
    length = rna1.length;
    blocks = length / 4 + 1;
    rna = new Nucleotide[blocks];
    for (int i = 1; i <= length; i++) {
        Nucleotide Nucl = rna1[i];
        Nucl = convert_vise(Nucl);
        (*this).put(i, Nucl);
    }
}

RNA::RNA(int length, Nucleotide Nucl) {
    this->length = length;
    this->blocks = length / 4 + 1;
    rna = new Nucleotide[blocks];
    for (int i = 1; i <= length; i++) {
        (*this).put(i, Nucl);
    }
}

RNA::RNA() {
    rna = new Nucleotide[1];
    length = 0;
    blocks = 1;
}

void RNA::get_length() {
    std::cout << length << std::endl;
}


/*RNA RNA::operator+(const RNA &rna1) {
    RNA rna_temporary = (*this);
    //rna_temporary = (*this);
    if (rna1.length == 1) {
        rna_temporary.add(rna1[1]);
    } else {
        rna_temporary.length = length + rna1.length;
        if (rna_temporary.length >= rna_temporary.blocks * 4) {
            //rna_temporary.length_arr = rna_temporary.length / 2;
            rna_temporary.blocks *= 2;
            // countcounte++;
            // std::cout << countcounte << std::endl;
            //std::cout << "rna_temporary length_arr is  "<< rna_temporary.length_arr / 2<< std::endl;
            //std::cout << "rna_temporary length is  "<< rna_temporary.length << std::endl;
            delete[] rna_temporary.rna;
            rna_temporary.rna = new Nucleotide[rna_temporary.blocks];
            for (int i = 1; i <= length; i++) {
                Nucleotide Nucl = (*this)[i];
                Nucl = convert_vise(Nucl);
                rna_temporary.put(i, Nucl);
            }
        }
        int k = 1;
        for (int i = length + 1; i <= rna_temporary.length; i++) {
            Nucleotide Nucl = rna1[k];
            Nucl = convert_vise(Nucl);
            rna_temporary.put(i, Nucl);
            k++;
        }
    }
    return rna_temporary;
} */
