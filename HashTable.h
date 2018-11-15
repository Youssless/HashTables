#ifndef MY_HASH_TABLE
#define MY_HASH_TABLE

#include "HashNode.h"
#include <vector>
#include <list>

using namespace std;

enum HashTableError { OUT_OF_MEMORY, KEY_NOT_FOUND, DUPLICATE_KEY }; // extend if necessary

typedef unsigned long ulint;

class HashTable {
  // typedef list<HashNode>::iterator ListIterator;
  typedef vector <list<HashNode>> Table;
  Table *table; // size of table is stored in the Table data structure
  size_t num;   // number of entries in the HashTable;

public:
  HashTable();       // constructor, initializes table of size 11;
  HashTable(size_t); // constructor, requires size of table as arg
  ~HashTable();      // deconstructor

  size_t size(); // returns size of the hash table (number of buckets)
  size_t hash_function(ulint);  // the table's hash function
  ulint getValue(ulint);    // find and return data associated with key

  void insert(ulint,ulint); // insert data associated with key into table
  void erase(ulint);        // remove key and associated data from table

  void rehash(size_t); // sets a new size for the hash table, reha/shes the hash table 

  // extend if necessary

  bool isresize(); // resize if the table size is less than the number of entries
  bool findKey(ulint); // retruns true if the key is found
};


/* Implement the 
- Constructors, Destructor
- hash_function, insert, getValue methods
- erase, and rehash methods 
*/

HashTable::HashTable() {
  num = 0;
  table = new Table(11); // table of size 11
    
  if (table == nullptr) {
    throw OUT_OF_MEMORY; // error code 0 - when the memory hasn't been initialised
  }
}

HashTable::HashTable(size_t tableSize) {
  num = 0;
  table = new Table(tableSize); // table of size tableSize

  if (table == nullptr) {
    throw OUT_OF_MEMORY; // error code 0 - when the memory hasn't been initialised
  }
}

HashTable::~HashTable() {
  delete table; // frees the memory
}

size_t HashTable::size() {
  return table->size();
}

size_t HashTable::hash_function(ulint data) {
  ulint key;
  key = data % size(); // key is the data mod the table size

  return key;
}

ulint HashTable::getValue(ulint hashKey) {
  list<HashNode> *hashNodeList;
  ulint key = hash_function(hashKey); // hash 
  hashNodeList = &(table->at(key)); // linked list initialised at the vector index determined by the key

  for (auto& n : *hashNodeList) { // searches the hashNodeList 
    if (hashKey == n.getKey())
      return n.getValue();
  }
  throw KEY_NOT_FOUND;
}

/*
  used in mc_dlog.cpp
*/
bool HashTable::findKey(ulint y) {
  list<HashNode> *hashNodeList;
  ulint key = hash_function(y);
  hashNodeList = &(table->at(key));

  for (auto& n : *hashNodeList) {
    if (y == n.getKey())
      return true;
  }
  return false;
}

void HashTable::insert(ulint hashKey, ulint data) {
  list<HashNode>* hashNodeList;
  HashNode* node = new HashNode; // new node created at insert

  ulint key = hash_function(hashKey); // key for each individual node
  hashNodeList = &(table->at(key)); // linked list initialised at the vector index determined by the key


  node->assign(hashKey, data); // assign the key and the data to the node


  for (list<HashNode>::iterator it = hashNodeList->begin(); it != hashNodeList->end(); ++it) {
    if (it->getKey() == hashKey)
      return;
  }
  hashNodeList->push_back(*node); // add the node to the back


  num++; // number of entries increased

  // resize if the table size is less than the number of entries
  if (isresize() == true)
    rehash(2*size() + 1); // make sure its prime
    
  if (getValue(hashKey) == node->getValue()) {
    cout << hashKey << " key and value " << node->getValue() << 
    " inserted successfully" << endl;
  }

}

void HashTable::erase(ulint hashKey) {
  list<HashNode>* hashNodeList;
  ulint key = hash_function(hashKey); // hash
  hashNodeList = &(table->at(key)); // linked list initialised at the vector index determined by the key

  //int count = 0;
  
  for (list<HashNode>::iterator it = hashNodeList->begin(); it != hashNodeList->end(); ++it) {

    if ((*it).getKey() == hashKey) {
        cout << "Value to be deleted: " << (*it).getValue() 
        << ", Key to be deleted: " << (*it).getKey() << endl;
        hashNodeList->erase(it); // erase the contents at iterator pos
        num--; // table size decreased by 1

        return; // once found return otherwise it will keep on searching 
    }
  }
  throw KEY_NOT_FOUND; // throws error code 1
}

void HashTable::rehash(size_t tableSize) {
  Table oldTable = *table;
  table->clear();
  table = new Table(tableSize);

  printf("\nREHASHING\n");
  // reference to list in oldTable
  for (list<HashNode> &list : oldTable) {
    //reference to node in list
    for (HashNode &node : list) {
      // oldTable must not be empty before the insert and list
      if (!oldTable.empty() && !list.empty()) {
        // insert key and value each node at the list
        insert(node.getKey(), node.getValue());
      }
    }
  }
}

bool HashTable::isresize() {
 if (size() <= num)
    return true;
  else
    return false;
}

  
#endif