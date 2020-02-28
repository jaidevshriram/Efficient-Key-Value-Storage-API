#include<bits/stdc++.h>
#include<chrono>
#include<future>
#include<thread>
#include "../src/trie.cpp"

using namespace std;

class kvstore{
public:
	Trie db;

	bool get(string key){
		Slice a(key.data()), b;
                //cout<<"done creating "<<key.data()<<"\n";
		bool is_valid = db.get_val(a, b);
                //cout<<"done getting\n";
		return is_valid;
	}

	bool put(string key, string value){
                Slice a(key.data()), b(value.data());
                auto result = async(launch::async, bind(&Trie::insertMain, &db, a, b));
                return result.get();
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
