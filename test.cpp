#include <iostream>
#include <vector>
#include <fstream> 
#include <string>
#include <algorithm>  
using namespace std;

void del_last(string& name_file) {
    string last = " " + name_file.substr(name_file.find_last_of(' ') + 1);
    name_file.erase(name_file.size() - last.length());
}

bool sort_dir(string a, string b) {
    char* char_array1 = new char[a.length() + 1];
    char* char_array2 = new char[b.length() + 1];
    strcpy_s(char_array1, a.size() + 1, a.c_str());
    strcpy_s(char_array2, b.size() + 1, b.c_str());
    bool res = strcmp(char_array1, char_array2) > 0;
    delete[] char_array1;
    delete[] char_array2;
    return !res;
}

bool sort_files(string a, string b) {
    int last1 = stoi(a.substr(a.find_last_of(' ') + 1));
    int last2 = stoi(b.substr(b.find_last_of(' ') + 1));
    if (last1 != last2)
        return last1 < last2;
    char* char_array1 = new char[a.length() + 1];
    char* char_array2 = new char[b.length() + 1];
    strcpy_s(char_array1, a.size() + 1, a.c_str());
    strcpy_s(char_array2, b.size() + 1, b.c_str());
    bool res = strcmp(char_array1, char_array2) > 0;
    delete[] char_array1;
    delete[] char_array2;
    return !res;
}

struct Node {
    vector<Node> children;
    string name;
    Node(string name) { this->name = name; }
    Node(vector<Node> children, string name) { this->children = children; this->name = name;}
    bool operator <(const Node& n) {
        if (isupper(name[0]) && islower(n.name[0]))
            return 1;
        else if (isupper(name[0]) && isupper(n.name[0]))
            return sort_dir(name, n.name);
        else if (islower(name[0]) && isupper(n.name[0]))
            return 0;
        else
            return sort_files(name, n.name);
    }
};

void print_children(Node& nodes_ch, int lvl) {
    cout << string(lvl, ' ') << nodes_ch.name << endl;
    int new_lvl = lvl + 1;
    for (Node& node : nodes_ch.children) {
        if(node.children.size() != 0)
            print_children(node, ++lvl);
        else 
            cout << string(new_lvl, ' ') << node.name << endl;
    }
}

bool comporator (Node& a, Node& b) {
        return a < b;
}

class Tree {

private:
    vector<Node> nodes;

    bool is_contains(vector<Node> &ns, string name) {
        for (Node& n : ns){
            if (name == n.name) {
                return true;
            }
        }
        return false;
    }

    Node* find_node(string name, vector<Node> &ns) {
        for (Node& node : ns) {
            if (name == node.name)
                return &node;
        }
        return nullptr;
    }

    void add_children(vector<string> names) {
        Node* cont = nullptr;
        for (int i = 0; i < names.size(); i++) {

            if (i == 0 && !is_contains(nodes, names[i])) {
                nodes.emplace_back(names[i]);
                cont = &nodes[nodes.size() - 1];
            }
            else if (i == 0) {
                cont = find_node(names[i], nodes);
            }
            else if (cont != nullptr && !is_contains(cont->children, names[i])) {
                cont->children.emplace_back(names[i]);
                cont = &cont->children[cont->children.size() - 1];
            }
            else if(cont != nullptr) {
                cont = find_node(names[i], cont->children);
            }
        }
    }

    void add_nodes(string name) {
        ifstream file(name);
        char sep = '\\';

        string s;
        while (getline(file, s)) { 
            if (s.size() != 0) {
                vector<string> names;
                for (size_t p = 0, q = 0; p != s.npos; p = q){
                    string name = s.substr(p + (p != 0), (q = s.find(sep, p + 1)) - p - (p != 0));

                    names.push_back(name);
                }
                add_children(names);
            }
        }
        file.close(); 
    }

    void sort_children(vector<Node> &nodes) {
        sort_children_impl(nodes);
        for (Node& n : nodes) {
            if (n.children.size() != 0)
                sort_children(n.children);
        }
    }

    void sort_children_impl(vector<Node> &nodes) {
        sort(nodes.begin(), nodes.end(), comporator);
        
        for (Node& n: nodes) {
            if(islower(n.name[0]))
                del_last(n.name);
        }
    }

public:
    void start() {
        add_nodes("example2.txt");
        sort_children(nodes);
        for (Node& node : nodes) {
            print_children(node, 0);
        }
    }
};

int main()
{
    Tree tree;
    tree.start();
}
