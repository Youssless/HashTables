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
ulint orderG(ulint, ulint, HashTable&);
ulint discreteLog(ulint, ulint, ulint, HashTable&, HashTable&);
void MC_DLog(ulint, ulint, ulint, HashTable&, HashTable&, HashTable&);
inline ulint modexp(ulint, ulint, ulint);
inline ulint randNum(ulint);
//

int main(int argc, char** argv) {
    ulint n, g, a;
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


ulint orderG(ulint n, ulint g, HashTable &Ord) {
    ulint y, rnd, case_one, case_two;

    for (int i = 0; i < sqrt(n); i++) {
        rnd = randNum(n-1);
        y = modexp(g, rnd, n);

        // if the key is found
        if (Ord.findKey(y) == true) {
            // two cases 
            case_one = rnd - Ord.hash_function(y); // random minus hashed y
            case_two = Ord.hash_function(y) - rnd; // hased y minus random

            if (case_one > 0) { // first case greater than 0
                return case_one;  
            }
            else if (case_two > 0) { // second case greater than 0

                return case_two;
            }
            else
                rnd = Ord.hash_function(y); // set rnd to hashed y
        }
        else
            Ord.insert(y, rnd); // key not found then add to the hash table
    }
    return n - 1;
}

ulint discreteLog(ulint n, ulint g, ulint a, HashTable &A, HashTable &B) {
    ulint y, rnd, rnd_two; 
    for (ulint i = 0; i < (ulint)sqrt(n); i++) {
        rnd = randNum(n-1);
        y = a * (modexp(g, rnd, n));

        // key is found
        if (B.findKey(y) == true)
            return (B.hash_function(y) - rnd); // return rand minus hashed y if the key is found in B
        else
            A.insert(y, rnd); // not found then insert at key y with a random number in A

        rnd_two = randNum(n-1); // generate another random number
        y = modexp(g, rnd, n);

        // key is found
        if (A.findKey(y) == true) 
            return (rnd - A.hash_function(y)); // return rand minus hashed y if the key is found in A  
        else
            B.insert(y, rnd_two); // not found then insert at key y with a random number in A
    }
    
    return 0;
}

void MC_DLog(ulint n, ulint g, ulint a, HashTable &Ord, HashTable &A, HashTable &B) {
    ulint dLog = discreteLog(n, g, a, A, B);
    ulint oG = orderG(n, g, Ord);
    ulint result = dLog % oG; // modulus of discrete log and order of g is the result 

    if (result < 0) { // if the result is less than 0 then add order of g to the result
        result += oG;
    }
    cout << "result: " << result << endl;
}

inline ulint modexp(ulint x, ulint exp, ulint mod_num) {
    ulint result = 1;

	while ((exp--) > 0) {
		result = (result * x) % mod_num;
	}
	return result;
}

inline ulint randNum(ulint n) {
    ulint rnd;
    time_t t;

    // random number generator depending on the current data and time
    srand(time(&t));
    rnd = (int)rand() % n; 

    return rnd;
}
