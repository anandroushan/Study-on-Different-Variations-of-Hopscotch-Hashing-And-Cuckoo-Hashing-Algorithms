#include "bits/stdc++.h"
using namespace std;
#define PRIME_A 1627
#define PRIME_B 28921
#define PRIME_C 2473
#define PRIME_D 47653
#define H 64

unsigned long hash1(string word) {
    unsigned long hash = 0;

    for (int i = 0; i < (int)word.length(); i++) {
        hash = (hash * PRIME_A) ^ ((int)word[i] * PRIME_B);
    }

    return hash;
}

unsigned long hash2(string word) {
    unsigned long hash = 0;

    for (int i = 0; i < (int)word.length(); i++) {
        hash = (hash * PRIME_C) ^ ((int)word[i] * PRIME_D);
    }

    return hash;
}  
class Hopscotch_Hashtable {
    struct Data { std::string word; int og_index; };
    Data *hash_table;
    int size;
    int capacity;
    void addToTable(Data d, int index);
    int findBlankSpot(int og_index);
    int searchNeighborhood(Data d, int blank_index);
    void swapSpots(int spot, int empty);
    bool isInNeighborhood(int og_index, int i);
    void expand();
    double times_expanded;
    double total_load_factor_percentage;
public:
    Hopscotch_Hashtable();
    std::string search(std::string);
    void insert(std::string);
    void print();
};

Hopscotch_Hashtable::Hopscotch_Hashtable()
{
    size = 0;
    capacity = 2;
    hash_table = new Data[capacity];
    times_expanded = 0.0;
    total_load_factor_percentage = 0.0;
}
string Hopscotch_Hashtable::search(string word)
{
    unsigned long hash_key = hash2(word);
    int index = hash_key % capacity;

    /* 
     *  Searches for the word in its corresponding neighborhood
     *  The loop supports circular behavior for the data structure
     */
    for (int i = 0; i < H; i++) {
        if (word == hash_table[(index + i) % capacity].word) {
            return hash_table[(index + i) % capacity].word;
        }
    }

    return "";
}

void Hopscotch_Hashtable::insert(string word)
{
    if (size == capacity) {
        expand();
    }

    unsigned long hash_key = hash2(word);
    int index = hash_key % capacity;
    Data d = {word, index};

    /*
     *  Checks for an empty spot within the neighborhood of the hashed index
     *  Will end the function if the value is successfully added to the table
     */
    for (int i = 0; i < H; i++) {
        if (hash_table[(index + i) % capacity].word == "") {
            index = (index + i) % capacity;
            addToTable(d, index);
            return;
        }
    }

    /*
     *  Searches for the next nearest blank spot to deal with the collisions
     */
    int blank_index = findBlankSpot(d.og_index);
    index = searchNeighborhood(d, blank_index);

    /*
     *  Adds value to the table if it hasn't already been added
     */
    if (index != -1) {
        addToTable(d, index);
    }
}

void Hopscotch_Hashtable::addToTable(Data d, int index)
{
    hash_table[index] = d;
    size++;
}

int Hopscotch_Hashtable::findBlankSpot(int og_index)
{
    int blank_index = og_index + H;
    
    /*
     *  Finding the index of the blank spot in the hash_table
     */
    for (int i = 0; i < capacity; i++) {
        if (hash_table[(blank_index + i) % capacity].word == "") {
            blank_index = (blank_index + i) % capacity;
            break;
        }
    }

    return blank_index;
}

int Hopscotch_Hashtable::searchNeighborhood(Data d, int blank_index)
{
    int curr_index;
    int curr_og_index;
    bool swapped;

    while (!isInNeighborhood(d.og_index, blank_index)) {
        swapped = false;

        /*
         *  Check for a potential swap within prior neighborhood of blank_index
         *  
         *  Will force our table to expand if the blank space is not in our
         *  neighborhood and there is no possible swap to move the blank space
         *  to the neighborhood of the value
         */
        for (int i = 0; i < H; i++) {
            curr_index = (capacity + blank_index - H + i) % capacity;
            curr_og_index = hash_table[curr_index].og_index;
            if (isInNeighborhood(curr_og_index, blank_index)) {
                swapped = true;
                swapSpots(curr_index, blank_index);
                blank_index = curr_index;
                break;
            }
        }

        if (!swapped) {
            expand();
            insert(d.word);
            return -1;
        }
    }

    return blank_index;
}

void Hopscotch_Hashtable::swapSpots(int spot, int empty)
{
    Data d = hash_table[spot];
    hash_table[spot] = hash_table[empty];
    hash_table[empty] = d;
}

bool Hopscotch_Hashtable::isInNeighborhood(int og_index, int index)
{
    if (index >= og_index && index < (og_index + H)) {
        return true;
    }

    return false;
}

void Hopscotch_Hashtable::expand()
{
    /*
     *  These two lines were used to calculate the average load factor before
     *  the table needed to expand
     */
    times_expanded++;
    total_load_factor_percentage += ((double)size/(double)capacity);

    Data *old_hash_table = hash_table;
    hash_table = new Data[capacity*2];

    size = 0;
    capacity *= 2;
    for (int i = 0; i < (capacity/2); i++) {
        string word = old_hash_table[i].word;
        if (word != "") {
            insert(word);
        }
    }

    delete [] old_hash_table;
}

void Hopscotch_Hashtable::print()
{
    cout << "Size: " << size << endl;
    cout << "Capacity: " << capacity << endl;
    cout << "Number of Expansions: " << times_expanded << endl;
    cout << "Average Load Factor: " << (total_load_factor_percentage/times_expanded) * 100 << endl;
}

int main()
{
    Hopscotch_Hashtable hopscotch_Hashtable;

    // number of words
    int n;
    cin >> n;
    vector<string> v(n);
    clock_t t1, t2;

    for(int i = 0; i < n; i++){
        cin >> v[i];
    }

    /* Timing insertion for hopscotch hashing */
    t1 = clock();
    for(int i = 0; i < n; i++){
        hopscotch_Hashtable.insert(v[i]);
    }
    t1 = clock() - t1;

    // Timing searching for hopscotch hashing 
    t2 = clock();
    for(int i = 0; i < n; i++){
        hopscotch_Hashtable.search(v[i]);
    }
    t2 = clock() - t2;

    hopscotch_Hashtable.print();
    cout << "It took " << ((double)t1 / CLOCKS_PER_SEC) << " seconds for insertion" << endl;
    cout << "It took " << ((double)t2 / CLOCKS_PER_SEC) << " seconds for searching" << endl;
    return 0;
}