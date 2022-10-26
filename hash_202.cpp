// John Kutbay
// CS202
// 9/28/2022
// Lab 5: Hash Tables

#include "hash_202.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <cstdio>
#include <sstream>
#include <ctype.h>
using namespace std;

string Hash_202::Set_Up(size_t table_size, const string &fxn, const string &collision)
{
	if (Keys.size() != 0) {
		return "Hash table already set up";
	} else if (collision != "Linear" && collision != "Double") {
		return "Bad collision resolution strategy";
	} else if (fxn != "Last7" && fxn != "XOR") {
		return "Bad hash function";
	}
	if (collision == "Linear") {
		Coll = 0;
	} else if (collision == "Double") {
		Coll = 1;
	}
	if (fxn == "XOR") {
		Fxn = 0;
	} else if (fxn == "Last7") {
		Fxn = 1;
	}
	// I am setting my vectors and variables here.
	Keys.resize(table_size);
	Vals.resize(table_size);
	for (unsigned long int i = 0;i < table_size;i++) {
		Keys[i] = "";
		Vals[i] = "";
	}
	Nkeys = 0;
	Nprobes = 0;
	return "";
}

string Hash_202::Add(const string &key, const string &val)
{
	// I will use the below to check if all the values are hex.
	stringstream ss;
	bool isHex = true;
	for (unsigned long int i = 0;i < key.size();i++) {
		if (!(isxdigit(key[i]))) {
			isHex = false;
			break;
		}
	}
	
	if (Keys.size() == 0) {
		return "Hash table not set up";
	} else if (key == "") {
		return "Empty key";
	} else if (isHex == false) {
		return "Bad key (not all hex digits)";
	} else if (val == "") {
		return "Empty val";
	} else if (Nkeys == Vals.size()) {
		return "Hash table full";
	} else if (Find(key) != "") {
		return "Key already in the table";
	}
	string hexKey;
	int index;
	string num1;
	int number1;
	string num2;
	int number2;
	int xorval = 0;

	// This coveres if my function is XOR, breaks up the key into seven characters, and 
	// continuously does Xor on it with the next seven characters.
	if (Fxn == 0) {
		if (key.length() <= 7) {
			hexKey = key;
		} else {
			for (long unsigned int i = 0;i < key.length() / 7;i++) {
				num1 = "";
				for (int j = 0;j < 7;j++) {
					num1 += key[(i * 7) + j];
				}
				ss.clear();
				ss.str("");
				ss.str(num1);
				ss << hex << num1;
				ss >> number1;
				xorval = (xorval ^ number1);
			}
			for (long unsigned int i = 0;i < key.length() % 7;i++) {
				num2 += key[i + ((key.length() / 7) * 7)];
			}
			ss.clear();
			ss.str("");
			ss.str(num2);
			ss << hex << num2;
			ss >> number2;
			xorval = (xorval ^ number2);
		}
		index = xorval % Keys.size();
	} else {
		// This checks if my functin is Last7, takes only the last seven characters, converts them to decimal,
		// and mods them by the table size.
		if (key.length() < 7) {
			hexKey = key;
		} else {
			for (long unsigned int i = key.length() - 7;i < key.length();i++) {
				hexKey += key[i];
			}
		}
		ss.clear();
		ss.str("");
		ss.str(hexKey);
		ss << hex << hexKey;
		ss >> index;
		index = index % Keys.size();
	}
	if (hexKey == key) {
		ss.clear();
		ss.str("");
		ss.str(hexKey);
		ss << hex << hexKey;
		ss >> index;
		index = index % Keys.size();
	}
	if (Vals[index] == "") {
		Vals[index] = val;
		Keys[index] = key;
	} else {
		// This checks if there are collisions present with storing the value.
		if (Coll == 0) {
			// This is for collision strategy Linear.
			while (Vals[index] != "") {
				if ((long unsigned int)(index + 1) >= Vals.size()) {
					index = 0;
				} else {
					index++;
				}
				Nprobes++;
			}
			Vals[index] = val;
			Keys[index] = key;
		} else {
			// This is for collision strategy Double hashing.
			int index2 = 0;
			string hexKey = "";
			num1 = "";
			number1 = 0;
			num2 = "";
			number2 = 0;
			if (Fxn == 1) {
				if (key.length() <= 7) {
					hexKey = key;
				} else {
					// This is setting the second index to the opposite function.
					for (long unsigned int i = 0;i < key.length() / 7;i++) {
						num1 = "";
						for (int j = 0;j < 7;j++) {
							num1 += key[(i * 7) + j];
						}
						ss. clear();
						ss.str("");
						ss.str(num1);
						ss << hex << num1;
						ss >> number1;
						xorval = (xorval ^ number1);
					}
					for (unsigned long int i = 0;i < key.length() % 7;i++) {
						num2 += key[i + ((key.length() / 7) * 7)];
					}
					ss. clear();
					ss.str("");
					ss.str(num2);
					ss << hex << num2;
					ss >> number2;
					xorval = (xorval ^ number2);
				}
				index2 = xorval % Keys.size();
				if (index2 == 0) {
					index2 = 1;
				}
				if (hexKey == key) {
					ss.clear();
					ss.str("");
					ss.str(hexKey);
					ss << hex << hexKey;
					ss >> index2;
					index2 = index2 % Keys.size();
					if (index2 == 0) {
						index2 = 1;
					}
				}
			} else {
				// Same, but with the other opposite function.
				hexKey = "";
				if (key.length() <= 7) {
					hexKey = key;
				} else {
					for (long unsigned int i = key.length() - 7;i < key.length();i++) {
						hexKey += key[i];
					}
				}
				ss.clear();
				ss.str("");
				ss.str(hexKey);
				ss << hex << hexKey;
				ss >> index2;
				index2 = index2 % Keys.size();
				if (index2 == 0) {
					index2 = 1;
				}
			}
			// This calculates the new index using double hashing.
			size_t runs = 0;
			while (Vals[index] != "" && runs <= Vals.size()) {
				if ((long unsigned int)(index + index2) >= Keys.size()) {
					index = (index + index2) % Keys.size();
				} else {
					index += index2;
				}
				Nprobes++;
				runs++;
			}
			if (runs <= Vals.size()) {
				Vals[index] = val;
				Keys[index] = key;
			} else {
				Nprobes -= runs;
				return "Cannot insert key";
			}
		}
	}
	Nkeys++;
	return "";
}

string Hash_202::Find(const string &key)
{
	stringstream ss;
	ss.clear();
	ss.str(key);
	int index;

	bool isHex = true;
	for (unsigned long int i = 0;i < key.size();i++) {
		if (!(isxdigit(key[i]))) {
			isHex = false;
			break;
		}
	}

	if (Keys.size() == 0) {
		return "";
	} else if (isHex == false) {
		return "";
	} else {
		ss.clear();
		ss.str(key);
		ss << hex << key;
		ss >> index;
		index = index % Keys.size();
	}
	unsigned long int count;
	string val = "";

	for (count = 0;count < Keys.size();count++) {
		if (Keys.at(count) == key) {
			val = Vals.at(count);
			break;
		}
	}
	
	if (val == "") {
		return "";
	} else {
		return val;
	}
}

void Hash_202::Print() const
{
	// Copied from Blake's Writeup.
	if(Keys.size() != 0) {
		for(unsigned long int i = 0; i < Keys.size(); i++) {
			if(Keys.at(i) != "") 
				printf("%5lu %s %s\n", i, Keys.at(i).c_str(), Vals.at(i).c_str());
		}
	}
}

size_t Hash_202::Total_Probes()
{
	// Copied from Blake's writeup.
	size_t sum = 0;

	/*if(Keys.size() != 0) {
		for(unsigned long int i = 0; i < Keys.size(); i++) {
			if(Keys.at(i) != "") {
				Find(Keys.at(i));
				sum += Nprobes;
			}
		}
	}*/
	sum = Nprobes;
	return sum;
}
