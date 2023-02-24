#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

struct Node {
    int key;
    Node *left, *right;
};

Node* newNode(int key){
    Node* node = new Node;
    node->key = key;
    node->left = node->right = NULL;
    return node;
}

Node* makeTree(int inorder[], int level[], int size, int istart, int iend, int lIndex);

Node* makeSubTree(Node* node, int inorder[], int level[], int size, int istart, int iend, int lIndex){
    while(lIndex < size){
        for (int i = istart; i <= iend; i++){
            if (inorder[i] == level[lIndex]) {
                return makeTree(inorder, level, size, istart, iend, lIndex);
            }
        }
        lIndex++;
    }
    return NULL;
}

Node* makeTree(int inorder[], int level[], int size, int istart, int iend, int lIndex){
    if (iend < istart) return NULL;
    Node* node = newNode(level[lIndex]);
    if (iend == istart){
        return node;
    }
    int n = 0;
    while(inorder[istart + n] != level[lIndex]){
        n++;
    }
    node->left = makeSubTree(node, inorder, level, size, istart, istart + n - 1, lIndex + 1);
    node->right = makeSubTree(node, inorder, level, size, istart + n + 1, iend, lIndex + 1);
    return node;
}

int decode(Node* node, string &decodedStr, string& coded, int i){
    if (node->left == NULL && node->right == NULL){
        char ch = char(node->key);
        decodedStr.push_back(ch);
        return i;
    }
    else if (coded[i] == '0') i = decode(node->left, decodedStr, coded, i + 1);
    else if (coded[i] == '1') i = decode(node->right, decodedStr, coded, i + 1);

    return i;
}

int main(int argc,char **argv) {
    if (argc != 4) {
        cout << "fail. you did not input enough arguments :(" << endl;//text something
        return 1;
    }

    string inorderfn = argv[1];
    string levelfn = argv[2];
    string codedfn = argv[3];
    
    ifstream file(inorderfn);
    if (!file) {
        cout << "Unable to open " << inorderfn << endl;
        return 2;
    }
    string line;
    int size = 0;
    while (getline(file, line)) {
        istringstream iss(line);
        int num;
        while (iss>>num) {
            size++;
        }
    }
    file.close();
    ifstream file3(inorderfn);

    string line3;
    int inorder[size];
    int i = 0;
    while (getline(file3, line3)) {
        istringstream iss(line3);
        int num;
        while (iss>>num) {
            inorder[i] = num;
            i++;
        }
    }
    file.close();

    ifstream file1(levelfn);
    if (!file1) {
        cout << "Unable to open " << levelfn << endl;
        return 2;
    }
    string line1;
    int level[size];
    int j = 0;
    while (getline(file1, line1)) {
        istringstream iss(line1);
        int num;
        while (iss>>num) {
            level[j] = num;
            j++;
        }
    }
    file1.close();

    ifstream file2(codedfn);
    if (!file2) {
        cout << "Unable to open " << codedfn << endl;
        return 2;
    }
    string line2, message;
    while (getline(file2, line2)) {
        istringstream iss(line2);
        string str;
        while (iss>>str) {
            message.append(str);
        }
    }
    file2.close();

    Node* node = makeTree(inorder,level,size,0,size - 1,0);

    string decodedStr;
    int pos = 0;
    while(pos < message.size()){
        pos = decode(node, decodedStr, message, pos);
    }
    cout<<decodedStr;

    return 0;
}