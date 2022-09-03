#include <iostream>
#include <vector>
#include <fstream> 
#include <string>
using namespace std;


struct Node {
    vector<Node*> children;
    string name;
    Node(vector<Node*> children, string name) { this->children = children; this->name = name;}
};

void print_children(Node* nodes_ch, int lvl) {
    cout << string(lvl, ' ') << nodes_ch->name << endl;
    int new_lvl = lvl + 1;
    for (Node* node : nodes_ch->children) {
        if(node->children.size() != 0)
            print_children(node, ++lvl);
        else 
            cout << string(new_lvl, ' ') << node->name << endl;
    }
}

class Tree {

private:
    vector<Node*> nodes;

    bool is_contains(vector<Node*> ns, string name) {
        bool b = false;
        for (Node* n : ns){
            if (name == n->name) {
                b = true;
                break;
            }
        }
        return b;
    }

    Node* find_node(string name, vector<Node*> ns) {
        for (Node* node : ns) {
            if (name == node->name)
                return node;
        }
        return nullptr;
    }

    void add_children(vector<string> names) {
        Node* cont = nullptr;
        for (int i = 0; i < names.size(); i++) {

            if (i == 0 && !is_contains(nodes, names[i])) { 
                Node* n = new Node(vector<Node*>(), names[i]);
                nodes.push_back(n);
                cont = n;
            }
            else if (i == 0) {
                cont = find_node(names[i], nodes);
            }
            else {
                if (cont != nullptr && !is_contains(cont->children, names[i])) {
                    Node* n = new Node(vector<Node*>(), names[i]);
                    cont->children.push_back(n);
                    cont = n;
                }
                else {
                    cont = find_node(names[i], cont->children);
                }
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

    void sort_dir(vector<string> &names_dir) {
        for (int i = 0; i < names_dir.size() - 1; i++) {
            for (int j = i + 1; j < names_dir.size(); j++) {
                char* char_array1 = new char[names_dir[i].length() + 1];
                char* char_array2 = new char[names_dir[j].length() + 1];
                strcpy_s(char_array1, names_dir[i].size() + 1, names_dir[i].c_str());
                strcpy_s(char_array2, names_dir[j].size() + 1, names_dir[j].c_str());
                if (strcmp(char_array1, char_array2) > 0) {
                    string tmp = names_dir[i];
                    names_dir[i] = names_dir[j];
                    names_dir[j] = tmp;
                }
            }
        }
    }

    void sort_files(vector<string> &names_files) {
        for (int i = 0; i < names_files.size() - 1; i++) {
            for (int j = i + 1; j < names_files.size(); j++) {
                int last1 = stoi(names_files[i].substr(names_files[i].find_last_of(' ') + 1));
                int last2 = stoi(names_files[j].substr(names_files[j].find_last_of(' ') + 1));
                if (last2 > last1) {
                    string tmp = names_files[i];
                    names_files[i] = names_files[j];
                    names_files[j] = tmp;
                }
                else if (last2 == last1) {
                    char* char_array1 = new char[names_files[i].length() + 1];
                    char* char_array2 = new char[names_files[j].length() + 1];
                    strcpy_s(char_array1, names_files[i].size() + 1, names_files[i].c_str());
                    strcpy_s(char_array2, names_files[j].size() + 1, names_files[j].c_str());
                    if (strcmp(char_array1, char_array2) > 0) {
                        string tmp = names_files[i];
                        names_files[i] = names_files[j];
                        names_files[j] = tmp;
                    }
                }
            }
        }
    }

    void del_last(string &name_file) {
        string last = " " + name_file.substr(name_file.find_last_of(' ') + 1);
        name_file.erase(name_file.size() - last.length());
    }

    void sort_children(vector<Node*> &nodes) {
        sort_children_impl(nodes);
        for (Node* n : nodes) {
            if (n->children.size() != 0)
                sort_children(n->children);
        }
    }

    void sort_children_impl(vector<Node*> &nodes) {
        vector<string> names_dir;
        vector<string> names_files;
        for (Node* n : nodes) {
            if (isupper(n->name[0]))
                names_dir.push_back(n->name);
            else 
                names_files.push_back(n->name);
        }
        if (names_dir.size() != 0) {
            sort_dir(names_dir);
        }
        if (names_files.size() != 0) {
            sort_files(names_files);
        }
        for (int i = 0; i < names_dir.size(); i++) {
            if (names_dir[i] == nodes[i]->name) {
                continue;
            }
            for (int j = 1; j < nodes.size(); j++) {
                if (names_dir[i] == nodes[j]->name) {
                    Node* tmp = nodes[i];
                    nodes[i] = nodes[j];
                    nodes[j] = tmp;
                }
            }
        }
        for (int i = 0; i < names_files.size(); i++) {
            if (names_files[i] == nodes[i]->name) {
                continue;
            }
            for (int j = names_dir.size(); j < nodes.size(); j++) {
                if (names_files[i] == nodes[j]->name) {
                    Node* tmp = nodes[i + names_dir.size()];
                    nodes[i + names_dir.size()] = nodes[j];
                    nodes[j] = tmp;
                }
            }
        }
        for (int i = 0; i < names_files.size(); i++) {
            del_last(nodes[i + names_dir.size()]->name);
        }
    }

public:
    void start() {
        add_nodes("example.txt");
        sort_children(nodes);
        for (Node* node : nodes) {
            print_children(node, 0);
        }
    }
};


int main()
{
    Tree tree;
    tree.start();
}
