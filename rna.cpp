#include "rna.hpp"
#include <iostream>

const Nucleotide A = 0b00;
const Nucleotide G = 0b01;
const Nucleotide C = 0b10;
const Nucleotide T = 0b11;

inline int ind_block(int i){
    return int (i / 4);
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

reference::reference(RNA & rna1, int ind){
    index = ind;
    rna_ref = &rna1;
}

reference::operator Nucleotide(){
    Nucleotide Nucl = (*rna_ref).get(index);
    return Nucl;
}

reference RNA::operator[](int index) {
    reference Nucleot(*this, index);
    return Nucleot;
}

reference& reference::operator=(const Nucleotide Nucl){
    (*rna_ref).eto_change(index, Nucl);
    return *this;
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
            break;
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
    }
    return Nucl;
}

reference& reference::operator=(const reference& nucl_here){
    Nucleotide Nucl = nucl_here.rna_ref->get(nucl_here.index);
    Nucl = convert_vise(Nucl);
    (*rna_ref).eto_change(index, Nucl);
    return *this;
}

void RNA::change(int index, Nucleotide Nucl){
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

void RNA::eto_change(int index, Nucleotide Nucl){
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
        (*this).change(ind, Nucl);
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
        length_arr = rna1.length_arr; //max dlinu prisvaivau
        rna = new Nucleotide[length_arr];
    }
    length = rna1.length;
    for (int i = 1; i <= length; i++){
        Nucleotide Nucl = rna1[i];
        Nucl = convert_vise(Nucl);
        (*this).change(i, Nucl);
    }
    return *this;
}

RNA RNA::operator+(const RNA& rna1){
    RNA rna_temporary;
    rna_temporary.length_arr = (length_arr + rna1.length_arr) * 2;
    rna_temporary.length = length + rna1.length;
    rna_temporary.rna = new Nucleotide[rna_temporary.length_arr];
    for (int i = 1; i <= length; i++){
        Nucleotide Nucl = (*this)[i];
        Nucl = convert_vise(Nucl);
        rna_temporary.change(i, Nucl);
    }
    int k = 1;
    for (int i = length + 1; i <= rna_temporary.length; i++){
        Nucleotide Nucl = rna1[k];
        Nucl = convert_vise(Nucl);
        rna_temporary.change(i, Nucl);
        k++;
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
        (*this).change(k, Nucl);
        k++;
    }
    this->length = rna_temporary.length - index;
    return *this;
}

RNA::RNA(){
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
            if (nucl2 == 'T') return 1;
        case 'G':
            if (nucl2 == 'C') return 1;
        case 'C':
            if (nucl2 == 'G') return 1;
        case 'T':
            if (nucl2 == 'A') return 1;
        default:
            break;
    }
    return 0;
}
void RNA::add(Nucleotide Nucl){
    RNA rna1(1, Nucl);
    (*this) = (*this) + rna1;
}

bool RNA::isComplimentary(const RNA& rna1){
    // A (00) - T (11), G (01) - C (10)
    if (length != rna1.length) return 0;
    int count_compl = 0;
    int isCompl;
    for (int i = 1; i <= length; i++){
        isCompl = check((*this)[i], rna1[i]);
        if (isCompl == 1) count_compl++;
    }
    if (count_compl != length) return 0;
    return 1;
}

bool RNA::operator==(const RNA& rna1){
    if (length != rna1.length) return 0;
    for (int i = 1; i <= length; i++){
        if ((*this)[i] != rna1[i]){
            return 0;
        }
    }
    return 1;
}

bool RNA::operator!=(const RNA& rna1){
    int min_length = std::min(length,rna1.length);
    int count_matches = 0;
    for (int i = 1; i <= min_length; i++){
        if ((*this)[i] == rna1[i]){
            count_matches++;
        }
    }
    if (length == rna1.length && count_matches == length) return 0;
    return 1;
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
        rna1.change(i, Nucl);
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
        (*this).change(i, Nucl);
    }
}

RNA::RNA(int length, Nucleotide Nucl){
    rna = new Nucleotide[(length / 4) + 1];
    for(int i = 0; i < ((length/4) +1); i++){
        rna[i] = A;
    }
    for (int i = 1; i <= length; i++){
        (*this).change(i, Nucl);
    }
    this->length = length;
    this->length_arr = length / 4;
    if (length % 4 != 0) this->length_arr++;
}
