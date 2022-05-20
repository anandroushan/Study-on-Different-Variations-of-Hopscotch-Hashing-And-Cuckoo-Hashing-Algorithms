/**
* Anand Roushan
* Project II
* 7th SEMESTER
* Cuckoo Hashing
**/
#include "bits/stdc++.h"
using namespace std;

#define MAXELEM 2
#define NUMHASH 2
#define MAXREC 15
#define PRIME_A 1627
#define PRIME_B 28921
#define PRIME_C 2473
#define PRIME_D 47653

// First Hash Function
unsigned long hash1(string word) {
    unsigned long hash = 0;

    for (int i = 0; i < (int)word.length(); i++) {
        hash = (hash * PRIME_A) ^ ((int)word[i] * PRIME_B);
    }
    return hash;
}

// Second Hash Function
unsigned long hash2(string word) {
    unsigned long hash = 0;

    for (int i = 0; i < (int)word.length(); i++) {
        hash = (hash * PRIME_C) ^ ((int)word[i] * PRIME_D);
    }

    return hash;
}

// Class
class Cuckoo {
	public:
	Cuckoo();
	~Cuckoo();
	void insert(string key, int tablenum, int rec);
	string search(string key);
	void expand();
	void print();

private:
	string **hashTable;
	int size;
	int capacity;
	int maxrec;
	int times_expanded;
	double total_load_factor_percentage;
	void insertKey(string key, int tablenum, int rec);
};

Cuckoo :: Cuckoo(){
	capacity = MAXELEM;
	maxrec = MAXREC;
	times_expanded = 0;
	size = 0;

	hashTable = new string*[NUMHASH];

	for (int i = 0; i < NUMHASH; i++) {
		hashTable[i] = new string[capacity];
	}

    for (int i = 0; i < NUMHASH; i++)
        for (int j = 0; j < capacity; j++)
            hashTable[i][j] = "";
}

Cuckoo::~Cuckoo() {

	for (int i = 0; i < NUMHASH; i++) {
        delete [] hashTable[i];
	}

    delete [] hashTable;
}

// Method - Insert into Hash Table
void Cuckoo::insert(string key, int tablenum, int rec) {
	insertKey(key, tablenum, rec);
	size++;
}
void Cuckoo :: insertKey(string key, int tablenum, int rec){
		// unsuccessful insertion, rehash
	if (rec == maxrec) {
		expand();
		return;
	}

	unsigned long hash_key;

	switch (tablenum) {

		case 1: hash_key =  hash2(key);
		case 2: hash_key =  hash1(key);
	}

	int index = hash_key % capacity;

	// check if key is already in table
	if (hashTable[tablenum - 1][index] == key) return;

	// insert into table
	if (hashTable[tablenum - 1][index] != "") {
		// save evicted key
		string evicted = hashTable[tablenum - 1][index];
		// place new key in cell
		hashTable[tablenum - 1][index] = key;
		int newtablenum;
		(tablenum == 1) ? newtablenum = 2 : newtablenum = 1;
		insertKey(evicted, newtablenum, rec + 1);
	}
	else {
		hashTable[tablenum - 1][index] = key;
	}
}

// Method - Expand Table Size b
void Cuckoo :: expand(){
	string **old_hash_table = hashTable;
    capacity *= 2;
    hashTable = new string*[NUMHASH];
    times_expanded++;
    total_load_factor_percentage += ((double)size/(double)capacity);
    size = 0;

	for (int i = 0; i < NUMHASH; i++) {
		hashTable[i] = new string[capacity];
	}

	// init all cells
    for (int i = 0; i < NUMHASH; i++) {
        for (int j = 0; j < capacity; j++) {
            hashTable[i][j] = "";
        }
    }

    // rehash
    for (int i = 0; i < NUMHASH; i++) {
        for (int j = 0; j < (capacity/2); j++) {
        	if (old_hash_table[i][j] != "") {
            	insert(old_hash_table[i][j], 1, 0);
        	}
        }
    }

    for (int i = 0; i < NUMHASH; i++) {
        delete [] old_hash_table[i];
    }

    delete [] old_hash_table;


}

// Method - Search a Key
string Cuckoo :: search(string key){
	unsigned long hash_key; 
	int index;

	hash_key = hash2(key);
	index = hash_key % capacity;

	if(hashTable[1][index] == key) {
		return key;
	}
	else {
		hash_key = hash1(key);
		index = hash_key % capacity;
		if (hashTable[0][index] == key) {
			return key;
		}
		else {
			return "";
		}
	}
}

// Method - Print Hash Tables
void Cuckoo :: print(){
	cout << "Capacity: " << capacity << endl;
    cout << "Number of Expansions: " << times_expanded << endl;
	cout << "Average Load Factor: " << (total_load_factor_percentage/times_expanded) << endl;
}

// Driver Code
int main(){
    Cuckoo cuckoo;
    clock_t t1, t2;
    int n;
    cin >> n;
    vector<string> v(n);
    for(int i = 0; i < n; i++){
    	cin >> v[i];
    }
    t1 = clock();
    for(int i = 0; i < n; i++){
    	cuckoo.insert(v[i], 1, 0);
    }
    t1 = clock() - t1;
    
	t2 = clock();
    for(int i = 0; i < n; i++){
    	cuckoo.search(v[i]);
    }
    t2 = clock() - t2;
    
    cuckoo.print();
    cout << "It took " << ((float)t1 / CLOCKS_PER_SEC) << " seconds for insertion" << endl;
    cout << "It took " << ((float)t2 / CLOCKS_PER_SEC) << " seconds for searching" << endl;


    return 0;
}