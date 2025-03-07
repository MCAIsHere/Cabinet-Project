// NFA 
#include <iostream>
#include <fstream>
using namespace std;
string Cuvant;
long int Numar_stari, Numar_litere, Starea_initiala, Numar_stari_finale, Nr_tranzitii, Nr_cuvinte, i, points;
void world_machine(string cuvant, int pozitie, int Road, long int Stari_finale[], long int Cifra_stanga[], char Muchii[], long int Cifra_dreapta[]);

int main() {
    fstream input("Input.txt");
    fstream output("Output.txt");

    input >> Numar_stari;
    long int Stari[Numar_stari];
    for (i = 0; i < Numar_stari; i++) input >> Stari[i];

    input >> Numar_litere;
    char Literele[Numar_litere];
    for (i = 0; i < Numar_litere; i++) input >> Literele[i];

    input >> Starea_initiala;
    input >> Numar_stari_finale;
    long int Stari_finale[Numar_stari_finale];
    for (i = 0; i < Numar_stari_finale; i++) input >> Stari_finale[i];

    input >> Nr_tranzitii;
    long int Cifra_stanga[Nr_tranzitii], Cifra_dreapta[Nr_tranzitii];
    char Muchii[Nr_tranzitii];
    for (i = 0; i < Nr_tranzitii; i++) {
        input >> Cifra_stanga[i];
        input >> Muchii[i];
        input >> Cifra_dreapta[i];
    }

    //
    input >> Nr_cuvinte;
    while (input >> Cuvant) {
        points = 0;
        world_machine(Cuvant, 0, Starea_initiala, Stari_finale, Cifra_stanga, Muchii, Cifra_dreapta);
        if (points >= 1) output << "YES" << endl;
        else output << "NO" << endl;
    }
    //

    input.close();
    output.close();
    return 0;
}

void world_machine(string cuvant, int pozitie, int Road, long int Stari_finale[], long int Cifra_stanga[], char Muchii[], long int Cifra_dreapta[]) {
    if (pozitie == cuvant.length()) {
        for (int j = 0; j < Numar_stari_finale; j++) {
            if (Stari_finale[j] == Road) points++;
        }
    }
    else {
        for (int j = 0; j < Nr_tranzitii; j++) {
            if (Cifra_stanga[j] == Road && Muchii[j] == cuvant[pozitie]) world_machine(cuvant, pozitie + 1, Cifra_dreapta[j], Stari_finale, Cifra_stanga, Muchii, Cifra_dreapta);
        }
    }
}