#include <windows.h>
#include <assert.h>
#include <stdio.h>
#include <omp.h>
#include <conio.h>
#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <array>
#include <regex>
#include <string>
#include <time.h>
#include <sys/time.h>

#define _BSD_SOURCE
#define MICRO_PER_SECOND 1000000

using std::array;
using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::fill;
using std::fill_n;

std::vector<std::string> sequencia1;
std::vector<std::string> sequencia2;
int tracos[2];

size_t countOccurrences(char c, string &str) {
    size_t count = 0;

    for (char i : str)
        if (i == c)
            count++;

    return count;
}

string replaceString(string linha){
    string novaLinha = " ";

    for (char i : linha){
        if (i != '-'){
            novaLinha += i;
        }
    }
    return novaLinha;
}

bool isCabecalho(std::string linha){
    if (linha.find('>') != std::string::npos){
        return true;
    }
    return false;
}

void salvaArquivo(){
    FILE *file;
    file = fopen("F:\\arquivosgerados\\Teste2.txt", "w");
    std::string sLinha = "";

    for (int iLinha = 0; iLinha <= (int) sequencia1.size(); iLinha++) {
        sLinha = sequencia1[iLinha];
        sLinha.erase(std::remove(sLinha.begin(), sLinha.end(), ' '), sLinha.end());
        if(sLinha.length() > 0 ){
            sLinha = sLinha + "\n";
            fprintf(file, sLinha.c_str());
        }
    }

    for (int iLinha = 0; iLinha <= (int) sequencia2.size(); iLinha++) {
        sLinha = sequencia2[iLinha];
        sLinha.erase(std::remove(sLinha.begin(), sLinha.end(), ' '), sLinha.end());
        if(sLinha.length() > 0 ){
            sLinha = sLinha + "\n";
            fprintf(file, sLinha.c_str());
        }
    }

    fclose(file);
}

void editaSequenciaSaida(int iTipo, std::vector<std::string> &aSequencia2){
    string sLinha = " ";

    for (int iLinha = 0; iLinha <= (int) aSequencia2.size(); iLinha++) {
        sLinha          = aSequencia2[iLinha];
        int iCaracteres = sLinha.length();

        if(!isCabecalho(sLinha) && tracos[iTipo] > 0){
            if(iCaracteres > tracos[iTipo]){
                if(iTipo == 1){
                    reverse(sLinha.begin(), sLinha.end());
                }

                sLinha = sLinha.substr(tracos[iTipo]);
                tracos[iTipo] = tracos[iTipo] - tracos[iTipo];

                if(iTipo == 1){
                    reverse(sLinha.begin(), sLinha.end());
                }
            }
            else {
                tracos[iTipo] = tracos[iTipo] - iCaracteres;
                sLinha = sLinha.substr(iCaracteres);
            }
            aSequencia2[iLinha] = sLinha;
        }
        else if(tracos[iTipo] == 0){
            break;
        }
    }
}

void contagemGaps(int iTipo, std::vector<std::string> &aSequencia1){

    std::regex rgx("-");
    std::smatch match;
    size_t count = 0;

    char c = '-';

    for(int i = 0; i <= (int) aSequencia1.size(); i++){
        string linha = aSequencia1[i];

        if(!isCabecalho(linha)){
            if (std::regex_search(linha, match, rgx)){
                int quantidade = countOccurrences('-', linha);
                tracos[iTipo] += quantidade;
                aSequencia1[i] = replaceString(linha);
            }
            else {
                break;
            }
        }
    }
}

void edita(){
    contagemGaps(0, sequencia1);
    reverse(sequencia1.begin(), sequencia1.end());

    contagemGaps(1, sequencia1);
    reverse(sequencia1.begin(), sequencia1.end());

    editaSequenciaSaida(0, sequencia2);
    reverse(sequencia2.begin(), sequencia2.end());

    editaSequenciaSaida(1, sequencia2);
    reverse(sequencia2.begin(), sequencia2.end());

    int tamanhoSequencia1 = (int) sequencia1.size();
    int tamanhoSequencia2 = (int) sequencia2.size();

    salvaArquivo();
}

void setConteudo(){
    std::ifstream file{"DENV1-X-gb_A75711.fasta"};

    std::string linha;

    int i = 0;
    while ( !file.eof() ) {
        file >> linha;

        int tamanhoSequencia1 = (int) sequencia1.size();
        int tamanhoSequencia2 = (int) sequencia2.size();

        if( (isCabecalho(linha) || tamanhoSequencia2 >= 1) && tamanhoSequencia1 > 1) { // this.aSequencia2.size() >= 1) && this.aSequencia1.size() > 1
             sequencia2.push_back(linha);
        }
        else {
            sequencia1.push_back(linha);
        }

    }
    /*
    printf("Sequencia 1 \n");
    for (auto s : sequencia1)
        std::cout << s << "\n";

    printf("Sequencia 2 \n");
    for (auto s : sequencia2)
        std::cout << s << "\n";
    */
}

int main(int argc, char *argv[]){

    struct timeval start_time;
    struct timeval stop_time;
    float time_diff;

    gettimeofday(&start_time, NULL);

    setConteudo();
    edita();

    gettimeofday( &stop_time, NULL );

    time_diff = (float)(stop_time.tv_sec - start_time.tv_sec);
    time_diff += (stop_time.tv_usec - start_time.tv_usec)/(float)MICRO_PER_SECOND;
    printf("\nfuncao executada em %f ms",time_diff);

    return 0;
}



