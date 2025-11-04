#include <iostream>
#include <cstdlib>
#include <random>


using namespace std;

const int n1 = 2, n2 = 2;
const int k = 2;
double lambda1, lambda2, miu1, miu2;
const int np = 7;
const int nt = 6;
int M[] = { 0, n1, n2, 0, 0, 0, 0, 1};
double D[] = { -1, -1, -1, -1, -1, -1, -1 };
double STf[k + 1], STr;
double DS, ceas, pas;
long Nd[k+1], Nr[k+1];
int urmTranzitie(void);
void execTranzitie(int te);
double genExp(double lambda);



int main() {
    //Simularea si monitorizarea sistemelor
    lambda1 = 1;
    lambda2 = 2;
    miu1 = 10;
    miu2 = 25;

    DS = 1e+5;
    ceas = 0;

    STf[1] = 0;
    STf[2] = 0;
    STr = 0;
    Nd[1] = 0;
    Nd[2] = 0;
    Nr[1] = 0;
    Nr[2] = 0;

    D[1] = genExp(M[1] * lambda1);
    D[2] = genExp(M[2] * lambda2);

    int iteratii = 0;

    while (ceas < DS)
    {
        iteratii++;
        int te = urmTranzitie();
        
        if (te == -1) {
            //cout << "\n[Blocaj] Nicio tranzitie activa. Oprire simulare.\n";
            break;
        }
        pas = D[te];
        if (pas < 0) {
            //cout << "\n[EROARE] pas invalid (" << pas << ") pentru te=" << te << ". Oprire.\n";
            break;
        }
        ceas += pas;        
        //inregistrarea timpilor de functionare pt masini
        if (D[1] > 0)
        {
            STf[1] += M[1] * pas;
        }
        if (D[2] > 0)
        {
            STf[2] += M[2] * pas;
        }
        //inregistrarea timpilor de remediere pt muncitori
        if (D[5] > 0)
        {
            STr += M[5] * pas;
        }
        if (D[6] > 0)
        {
            STr += M[6] * pas;
        }

        for (int t = 1; t <= nt; t++) {
            if (D[t] > 0) {
                if (D[t] > pas) {
                    D[t] -= pas;
                }
            }
        }
        execTranzitie(te);
    }
    //cout << iteratii << endl;
    double Disp1 = STf[1] / (DS * n1) * 100;
    cout << "Disp1=" << Disp1 << endl;
    double Disp2 = STf[2] / (DS * n2) * 100;
    cout << "Disp2=" << Disp2 << endl;

    return 0;
}

double genExp(double lambda)
{
    double u, x;
    u = (double)rand() / ((double)RAND_MAX + 1);
    x = -1 / lambda * log(1 - u);
    return x;
}

int urmTranzitie() {
    int te = -1;
    double de_min = numeric_limits<double>::max();
  
    for (int t = 1; t <= nt; t++) {
        if (D[t] >= 0 && D[t] <= de_min)
        {
            de_min = D[t];
            te = t;
        }
    }
    return te;
}

void execTranzitie(int te) {
    switch (te)
    { 
        case 1:
            Nd[1]++;
            //actualizare marcaj
            M[1]--; M[3]++;
            //actualizare durata
            if (!M[1]) {
                D[1] = -1;
            }
            else {
                D[1] = genExp(M[1] * lambda1);
            }
            if (M[7] >= 1) {
                D[3] = 0;
            }
            break;

        case 2:
            Nd[2]++;
            //actualizare marcaj
            M[2]--; M[4]++;
            //actualizare durata
            if (!M[2]) {
                D[2] = -1;
            }
            else {
                D[2] = genExp(M[2] * lambda2);
            }
            if (M[7] >= 1) {
                D[4] = 0;
            }
            break;

        case 3:
            //actualizare marcaj
            M[3]--; M[7]--; M[5]++;
            //actualizare durata
            if (!M[3]) {
                D[3] = -1;
            }
            if (!M[7]) {
                D[3] = -1;
                D[4] = -1;
            }
            D[5] = genExp(M[5] * miu1);
            break;

        case 4:
            //actualizare marcaj
            M[4]--; M[7]--; M[6]++;
            //actualizare durata
            if (!M[4]) {
                D[4] = -1;
            }
            if (!M[7]) {
                D[3] = -1;
                D[4] = -1;
            }
            D[6] = genExp(M[6] * miu2);
            break;

        case 5:
            //actualizare marcaj
            M[5]--; M[1]++; M[7]++;
            //actualizare durata
            if (!M[5]) {
                D[5] = -1;
            }
            else {
                D[5] = genExp(M[5] * miu1);
            }
            D[1] = genExp(M[1] * lambda1);
            if (M[3] > 0) {
                D[3] = 0;
            }
            if (M[4] > 0) {
                D[4] = 0;
            }
            Nr[1]++;
            break;

        case 6:
            //actualizare marcaj
            M[6]--; M[2]++; M[7]++;
            //actualizare durata
            if (!M[6]) {
                D[6] = -1;
            }
            else {
                D[6] = genExp(M[6] * miu2);
            }
            D[2] = genExp(M[2] * lambda2);
            if (M[3] > 0 ) {
                D[3] = 0;
            }
            if (M[4] > 0 ) {
                D[4] = 0;
            }
            Nr[2]++;
            break;
    }
}


