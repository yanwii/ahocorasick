#include <iostream>
#include <vector>
#include <unordered_map>
#include <exception>
#include <time.h>
#include <algorithm>
#include <deque>
#include "trie_tree.h"
#include "utils.h"
#include "ahocorasick.h"

using namespace std;
void AhoCorasick::make_ac(){
    deque <Node *> q;
    Node* root = proot;
    Node* parent_failure_node;
    q.push_back(root);

    while(!q.empty()){
        //root = pop_back(queue);
        root = q.front();
        q.pop_front();
        int parent_state = root->state;
        for (auto it=root->child.begin(); it!=root->child.end();it++){
            int word_to_int = it->first;
            Node* node = it->second;
            q.push_back(node);
            //cout << "word " << word << " state " << node->state;
            //cout << " parent_word " << parent_state << endl;
            int state = node->state;
            
            if (node->depth==1){
                failure[state] = proot;
                continue;
            }
            parent_failure_node = failure[parent_state];
            if (parent_failure_node->child[word_to_int]!=NULL){
                failure[state] = parent_failure_node->child[word_to_int];
                //cout << state << " failure state " << parent_failure_node->child[word]->state << endl;
            } else {
                failure[state] = proot;
            }
        }
    }
}

//void AhoCorasick::search(string text){
//    search(text, false);
//}

void AhoCorasick::search(string text, bool if_reverse){
    vector<string> segment = cut(text, if_reverse);
    search_(segment);
}


void AhoCorasick::search(vector<string> segements){
    cout << &segements << endl;
}

void AhoCorasick::search_(vector<string> &segments){
    unordered_map<string, string> result;
    Node* root = proot;
    for (int i=0 ; i < segments.size(); i++){
        string seg = segments[i];
        int word_to_int;
        if (vocab[seg]==0){
            word_to_int = -1;
        } else {
            word_to_int = vocab[seg];
        }
        //cout << seg << endl;
        //cout << root->state << endl;
        //cout << root->is_end << endl;
        if (root->child[word_to_int]==NULL){
            if (root->state==0){
                root = proot;
                continue;
            } else {
                root = failure[root->state];
                checkout(root, result, i+1);
                i--;
            }
        } else {
            root = root->child[word_to_int];
            checkout(root, result, i+1);
        }
        //
    }
    for (auto it=result.begin(); it!=result.end(); it++){
        cout << "from_to " << it->first << " word " << it->second << endl;
    }
}

void AhoCorasick::checkout(Node* &root, unordered_map<string, string> &result,const int &stop){
    if (root->is_end){
        string text = root->segment;
        int start = stop - cut(text, true).size();
        string from_to = to_string(start) + "_" + to_string(stop);
        result[from_to] = text;
    }            
}


void AhoCorasick::insert(string word, bool if_reverse){
    try{
        vector<string> segment = cut(word, if_reverse);
        string tmp = "";
        Node* root = proot;
        for (int i=0; i<segment.size(); i++){
            string word = segment[i];
            tmp += word;
            if (vocab[word]==0){
                vocab[word] = vocab.size()+1;
            }
            int word_to_int = vocab[word];


            if (root->child[word_to_int]==NULL){
                node_nums ++;
                root->child[word_to_int] = new Node();
                root->child[word_to_int]->word = word;
                root->child[word_to_int]->state = node_nums;
                root->child[word_to_int]->depth = i+1;
            }
            
            int depth = root->child[word_to_int]->depth;
            int state = root->child[word_to_int]->state;
            if (depth<=1){
                failure[root->child[word_to_int]->state] = proot;
            } else {
                int parent_state = root->state;
                Node* parent_failure_node = failure[parent_state];
                if (parent_failure_node->child[word_to_int]!=NULL){
                    failure[state] = parent_failure_node->child[word_to_int];
                    //cout << state << " failure state " << parent_failure_node->child[word]->state << endl;
                } else {
                    failure[state] = proot;
                }
            }
            root = root->child[word_to_int];
        }
        root->is_end = true;
        root->segment = tmp;
    } catch (exception &e){
        cout << &e << endl;
    }
}
