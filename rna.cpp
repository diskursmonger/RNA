#include "rna.h"
#include <iostream>

static int countcounte = 0;

inline int ind_block(int i){
    return (i / 4);
}

inline int ind_in_block(int i){
    return (i + 1) % 4;
}

Nucleotide do_Mask(Nucleotide Nucl, int ind_in_block){
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

Nucleotide convert(Nucleotide Nucl){
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

Nucleotide convert_vise(Nucleotide Nucl){
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
reference::reference(RNA & rna1, int ind){
    index = ind;
    rna_ref = &rna1;
}

reference::operator Nucleotide(){
    //вытаскиваем нуклеотид который хранится в r1[index]
    Nucleotide Nucl = (*rna_ref).get(index);
    return Nucl;
}

reference RNA::operator[](int index) {
    // r1[4]
    reference Nucl_ref(*this, index);
    //создаем объект reference r1[index]
    return Nucl_ref;
}

reference& reference::operator=(const Nucleotide Nucl){
    //r1[2] = T;
    (*rna_ref).put_ref(index, Nucl);
    return *this;
}

reference& reference::operator=(const reference& nucl_here){
    //r1[2] = r2[4];
    //r1.operator[]2.operator=(r2.operator[]4);
    Nucleotide Nucl = nucl_here.rna_ref->get(nucl_here.index);
    Nucl = convert_vise(Nucl);
    (*rna_ref).put_ref(index, Nucl);
    return *this;
}

void RNA::put(int index, Nucleotide Nucl){
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
}

void RNA::put_ref(int index, Nucleotide Nucl){
    --index;
    if (index < 0) {
        std::cout << "Wrong number of nucleotide." << std::endl;
        exit(0);
    }
    Nucleotide Nucl_in_block[4];
    int ind_bloc = ind_block(index);
    int ind_in_bloc = ind_in_block(index);
    if (ind_in_bloc == 0) ind_in_bloc = 4;
    for (int i = 0; i < 4; i++){ //i- number in block
        int ind = (i + 1) + (ind_bloc * 4);
        Nucl_in_block[i] = (*this).get(ind);
    }
    Nucl_in_block[ind_in_bloc - 1] = Nucl;
    //peremestitt vse v block
    for (int i = 0; i < 4; i++){
        Nucleotide Nucl = Nucl_in_block[i];
        Nucl = convert_vise(Nucl);
        int ind = (i + 1) + (ind_bloc * 4);
        (*this).put(ind, Nucl);
    }
}

Nucleotide RNA::operator[](int index) const{
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

RNA& RNA::operator=(const RNA& rna1){
    if (&rna == &rna1.rna){
        return *this;
    }
    if (length != rna1.length){
        delete [] rna;
       // length_arr = rna1.length_arr; //max dlinu prisvaivau
        //if ((rna1.length / rna1.length_arr >= 2)) {
        //if (rna1.length / rna1.length_arr >= 3){
        length_arr = rna1.length_arr;
           // std::cout <<length_arr<<"1\n";
        //}
        //else {
          //  length_arr = rna1.length_arr;
            //std::cout << rna1.length <<"  "<< length_arr <<"  - length\n";
        //}
        rna = new Nucleotide[length_arr];
    }
   // length_arr = rna1.length_arr;
    length = rna1.length;
    for (int i = 1; i <= length; i++){
        Nucleotide Nucl = rna1[i];
        Nucl = convert_vise(Nucl);
        (*this).put(i, Nucl);
    }
    return *this;
}

RNA RNA::operator+(const RNA& rna1){
    RNA rna_temporary;
    rna_temporary = (*this);
    if(rna1.length == 1){
        rna_temporary.add(rna1[1]);
    } else {
        //RNA rna_temporary = (*this);
        rna_temporary.length = length + rna1.length;
        if (rna_temporary.length >= rna_temporary.length_arr * 4) {
            rna_temporary.length_arr = rna_temporary.length;
            // countcounte++;
            // std::cout << countcounte << std::endl;
            // std::cout << "rna_temporary is  "<< rna_temporary.length_arr << std::endl;
            rna_temporary.rna = new Nucleotide[rna_temporary.length_arr];
            for (int i = 1; i <= length; i++) {
                Nucleotide Nucl = (*this)[i];
                Nucl = convert_vise(Nucl);
                rna_temporary.put(i, Nucl);
            }
        }
        /*if (rna_temporary.length_arr * 4 < length_arr + rna1.length_arr){
           //countcounte++;
           // if (countcounte % 10 == 0) std::cout << countcounte << std::endl;
            rna_temporary.length_arr = (length + rna1.length) * 2 + 1;
           // std::cout << "rna_temporary is  "<< rna_temporary.length_arr << std::endl;
            rna_temporary.rna = new Nucleotide[rna_temporary.length_arr];
            for (int i = 1; i <= length; i++){
                Nucleotide Nucl = (*this)[i];
                Nucl = convert_vise(Nucl);
                rna_temporary.put(i, Nucl);
            }
        } */
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

RNA& RNA::split(int index){
    RNA rna_temporary = *this;
    delete []rna;
    this->length_arr = length / 4;
    rna = new Nucleotide[length_arr];
    int k = 1;
    for (int i = index + 1; i <= length; i++){ //begin from index+1
        Nucleotide Nucl = rna_temporary[i];
        Nucl = convert_vise(Nucl);
        (*this).put(k, Nucl);
        k++;
    }
    this->length = rna_temporary.length - index;
    return *this;
}

Nucleotide RNA::get(int index){
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

bool check(Nucleotide nucl1, Nucleotide nucl2){
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

void RNA::add(Nucleotide Nucl){
    //RNA rna1(1, Nucl);
    //(*this) = (*this) + rna1;
    //if (max_nucl)
    length++;
    int max_nucl = length_arr * 4;
    if (length == max_nucl){
        RNA rna_temporary;
        rna_temporary = (*this);
        length_arr *= 2;
        delete[] rna;
        rna = new Nucleotide[length_arr];
        for (int i = 1; i < length; i++){
            Nucleotide n = rna_temporary[i];
            n = convert_vise(n);
            (*this).put(i, n);
        }
        //std::cout << length << std::endl;
    }
    Nucl = convert_vise(Nucl);
    (*this).put(length, Nucl);
}

bool RNA::isComplimentary(const RNA& rna1){
    // A (00) - T (11), G (01) - C (10)
    if (length != rna1.length) return false;
    int count_comp = 0;
    int isComp;
    for (int i = 1; i <= length; i++){
        isComp = check((*this)[i], rna1[i]);
        if (isComp == 1) count_comp++;
    }
    if (count_comp != length) return false;
    return true;
}

bool RNA::operator==(const RNA& rna1){
    if (length != rna1.length) return false;
    for (int i = 1; i <= length; i++){
        if ((*this)[i] != rna1[i]){
            return false;
        }
    }
    return true;
}

bool RNA::operator!=(const RNA& rna1){
    int min_length = std::min(length,rna1.length);
    int count_matches = 0;
    for (int i = 1; i <= min_length; i++){
        if ((*this)[i] == rna1[i]){
            count_matches++;
        }
    }
    if (length == rna1.length && count_matches == length) return false;
    return true;
}

Nucleotide compNucl(Nucleotide Nucl){
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

RNA RNA::operator!(){
    RNA rna1;
    rna1.rna = new Nucleotide[length_arr];
    rna1.length = length;
    rna1.length_arr = length_arr;
    for (int i = 1; i <= length; i++){
        Nucleotide Nucl = compNucl((*this)[i]);
        rna1.put(i, Nucl);
    }
    return rna1;
}

RNA::RNA(const RNA& rna1){ //copyconstuctor
    length = rna1.length;
    length_arr = rna1.length_arr;
    rna = new Nucleotide[length_arr];
    for (int i = 1; i <= length; i++){
        Nucleotide Nucl = rna1[i];
        Nucl = convert_vise(Nucl);
        (*this).put(i, Nucl);
    }
    //(*this) = rna1;
}

RNA::RNA(int length, Nucleotide Nucl){
    rna = new Nucleotide[(length / 4) + 1];
   // for(int i = 0; i < ((length/4) + 1); i++){
   //     rna[i] = A;
   // }
    for (int i = 1; i <= length; i++){
        (*this).put(i, Nucl);
    }
    this->length = length;
    this->length_arr = length / 4 + 1;
  //  if (length % 4 != 0) this->length_arr++;
}

RNA::RNA(){
    rna = new Nucleotide[1];
}

void RNA::get_length() {
    std::cout << length << std::endl;
}
