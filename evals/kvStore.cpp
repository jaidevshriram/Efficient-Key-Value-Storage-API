#include<bits/stdc++.h>
#include "../src/trie.cpp"
using namespace std;

class kvstore{
public:
	Trie db;

	bool get(string key){
		Slice a(key.data()), b;
		bool is_valid = db.get_val(a, b);
		return is_valid;
	}

	bool put(string key, string value){
		Slice a(key.data()), b(value.data());
		//printf("%d - %s\n", a.size, a.data);
		db.insert(a, b);
                return true;
	}

	bool del(string key){
		Slice a(key.data());
		bool is_valid = db.del(a);
		return is_valid;
	}

	pair<string,string> get(int N){
		// Your Code Here
		pair<string,string> temp = make_pair("key","value");
		return temp;
	}

	bool del(int N){
		// Your Code Here
		return true;
	}

        void display() {
            db.display();
        }
};
