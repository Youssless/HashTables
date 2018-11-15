#include <iostream>
#include <sstream>
#include <ctime>
#include <random>
#include <cmath>
//#include <cstring>
#include "HashTable.h"

using namespace std;

/* Implement mc_dlog in this file */

// function declerations
int orderG(int n, int g, HashTable &Ord);
int discreteLog(int n, int g, int a, HashTable &A, HashTable &B);
void MC_DLog(int n, int g, int a, HashTable &Ord, HashTable &A, HashTable &B);
int randNum(int n);
//

int main(int argc, char** argv) {
    int n, g, a;
    if (argc == 4) {
        HashTable Ord = HashTable();
        HashTable A = HashTable();
        HashTable B = HashTable();

        n = atoi(argv[1]);
        g = atoi(argv[2]);
        a = atoi(argv[3]);

        cout << "order g" << endl;
        cout << orderG(n, g, Ord) << endl;

        cout << "\ndiscrete log" << endl;
        cout << discreteLog(n, g, a, A, B) << endl;

        cout << "\nmc_dlog" << endl;
        MC_DLog(n, g, a, Ord, A, B);

    }
    else
        cout << "Number of arguments must be no more or no less than 3." << endl;

    return 0;
}


int orderG(int n, int g, HashTable &Ord) {
    int y, rnd, case_one, case_two;

    for (int i = 0; i < sqrt(n); i++) {
        rnd = randNum(n);
        y = (int)pow(g, rnd) % n;

        // if the key is found
        if (Ord.findKey(y) == true) {
            // two cases 
            case_one = rnd - Ord.hash_function(y); // random minus hashed y
            case_two = Ord.hash_function(y) - rnd; // hased y minus random

            if (case_one > 0) { // first case greater than 0
                if (case_one > case_two)
                    return case_one; // return the greater number of the two
                else if (case_one == case_two)
                    return case_one; // return one of them if equal   
            }
            else if (case_two > 0) { // second case greater than 0
                if (case_two > case_one)
                    return case_two; // return the greater number of the two
                else if (case_two == case_one)
                    return case_two; // return one of them if equal
            }
            else
                rnd = Ord.hash_function(y); // set rnd to hashed y
        }
        else
            Ord.insert(y, rnd); // key not found then add to the hash table
    }
    return n - 1;
}

int discreteLog(int n, int g, int a, HashTable &A, HashTable &B) {
    int y, rnd, rnd_two; 
    for (int i = 0; i < sqrt(n); i++) {
        rnd = randNum(n);
        y = (a * (int)pow(g, rnd)) % n ;

        // key is found
        if (B.findKey(y) == true)
            return (rnd - B.hash_function(y)); // return rand minus hashed y if the key is found in B
        else
            A.insert(y, rnd); // not found then insert at key y with a random number in A

        rnd_two = randNum(n); // generate another random number

        // key is found
        if (A.findKey(y) == true) 
            return (rnd - A.hash_function(y)); // return rand minus hashed y if the key is found in A  
        else
            B.insert(y, rnd_two); // not found then insert at key y with a random number in A
    }
    return 0; 
}

void MC_DLog(int n, int g, int a, HashTable &Ord, HashTable &A, HashTable &B) {
    int dLog = discreteLog(n, g, a, A, B);
    int oG = orderG(n, g, Ord);
    int result = dLog % oG; // modulus of discrete log and order of g is the result

    cout << "result: " << result << endl; 

    if (result < 0) { // if the result is less than 0 then add order of g to the result
        result += oG;
        cout << "result if < 0 : " << result << endl;
    }
}



int randNum(int n) {
    int rnd;
    time_t t;

    // random number generator depending on the current data and time
    srand(time(&t));
    rnd = (int)rand() % n; 

    return rnd;
}
