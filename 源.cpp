#include <iostream>
#include <openssl/rand.h>
#include <openssl/evp.h>
#include<iostream>
using namespace std;

class node {
public:
	string name;
	node* sun[16];
	int value;
	int sun_num;
	int is_leaf;

	node() {
		for (int i = 0; i < 16; i++)
		{
			sun[i] = NULL;
		}
		sun_num = 0;
		is_leaf = 0;
	}
};

int sm3_hash_openssl(uint8_t* dgst, const void* msg, size_t len) {
	int res = 0;
	const EVP_MD* md = EVP_get_digestbyname("sm3");
	EVP_MD_CTX* mdctx = EVP_MD_CTX_new();
	if (!mdctx) goto done;

	EVP_DigestInit_ex(mdctx, md, NULL);
	EVP_DigestUpdate(mdctx, msg, len);
	res = EVP_DigestFinal_ex(mdctx, dgst, NULL);

done:
	EVP_MD_CTX_free(mdctx);
	return res;
}

string int_to_hex(uint8_t* list, int len) {
	string buff;
	char a[] = "0123456789abcdef";
	for (int i = 0; i < len; i++) {
		int b = list[i] >> 4;
		uint8_t c = uint8_t((list[i] << 4));
		c = c / 16;
		buff.push_back(a[b]);
		buff.push_back(a[c]);
	}
	return buff;
}

int get_index(char world) {
	switch (world) {
	case '0':
		return 0;
	case '1':
		return 1;
	case '2':
		return 2;
	case '3':
		return 3;
	case '4':
		return 4;
	case '5':
		return 5;
	case '6':
		return 6;
	case '7':
		return 7;
	case '8':
		return 8;
	case '9':
		return 9;
	case 'a':
		return 10;
	case 'b':
		return 11;
	case 'c':
		return 12;
	case 'd':
		return 13;
	case 'e':
		return 14;
	case 'f':
		return 15;
	}


}

class MPT {
public:
	node root;

	void add(string name,int value) {
		char* name2 = new char(name.length() + 1);
		for (int i = 0; i < name.length(); i++) {
			name2[i] = name[1];
		}
		name2[name.length()] = '0';
		uint8_t dgst[32];
		sm3_hash_openssl(dgst, name2, name.length());
		string str1 = int_to_hex(dgst, 32);
		node* temp=&root;
		int num = 0;
		while (temp->sun[get_index(str1[num])] != NULL) {
			if (temp->sun[get_index(str1[num])]->is_leaf == 1) {
				temp= temp->sun[get_index(str1[num])];
				char temp_char = temp->name[num+1];
				string temp_name= temp->name;
				int temp_value= temp->value;
				temp->name = "";
				temp->value = 0;
				temp->is_leaf = 0;
				num += 1;
				node* temp1 = new node();
				node* temp2 = new node();
				temp1->name = str1;
				temp1->value = value;
				temp1->is_leaf = 1;
				temp2->name = temp_name;
				temp2->value = temp_value;
				temp2->is_leaf = 1;
				temp->sun[get_index(str1[num])] = temp1;
				temp->sun[get_index(temp_char)] = temp2;
				return;
			}
			else {
				temp = temp->sun[get_index(str1[num])];
			}
			num += 1;
		}
		node* temp1 = new node();
		temp1->name = str1;
		temp1->value = value;
		temp1->is_leaf = 1;
		temp->sun[get_index(str1[num])] = temp1;

	}

	int find(string name) {
		char* name2 = new char(name.length() + 1);
		for (int i = 0; i < name.length(); i++) {
			name2[i] = name[1];
		}
		name2[name.length()] = '0';
		uint8_t dgst[32];
		sm3_hash_openssl(dgst, name2, name.length());
		string str1 = int_to_hex(dgst, 32);
		node* temp = &root;
		int num = 0;
		while (temp != NULL) {
			if (temp->is_leaf == 1) {
				//temp = temp->sun[get_index(name[num])];
				return temp->value;
			}
			else {
				temp = temp->sun[get_index(str1[num])];
			}
			num += 1;
		}
	}
};


int main() {
	MPT mpt;
	mpt.add("abc", 1);
	mpt.add("acd", 2);
	cout<< mpt.find("acd");

}