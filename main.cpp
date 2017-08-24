#include <iostream>
#include <fstream>
#include <algorithm>
#include <map>
#include <string>
#include <vector>
#include <sstream>
#include <boost/algorithm/string.hpp>
using namespace std;

class Point {
public:
  double x, y;
  Point(double _x=0, double _y=0): x(_x), y(_y) {}
  friend ostream& operator <<(ostream &out, Point &p) {
    out << "(" << p.x << "," << p.y << ")";
    return out;
  }
};


class Node {
public:
  string id;
  vector<Node *> children;
  string style;
  Node *parent;

  Point pos; // output positions
  Node(string _id): id(_id), parent(NULL) {}
  string make_tikz_id() {
    return "V" + id;
  }
  void output_node_definition(ostream &out) {
    if (style == "hidden") return;
    pos.y = -pos.y;
    out << "\\node(" << make_tikz_id() << ") at " << pos << "{$" << id << "$};" << '\n';
    pos.y = -pos.y;
  }
  void output_edges(ostream &out) {
    for (auto &&y: children) {
      if (y->style == "hidden") continue;
      out << "\\draw (" << make_tikz_id() << ") -- (" << y->make_tikz_id() << ");\n";
    }
  }
};

map<string, Node*> pools;
Node* find_or_create_node(string &ID) {
  if (pools.find(ID) == pools.end()) {
    pools[ID] = new Node(ID);
  }
  return pools[ID];
}

void read_vertices() {
  string ID;
  while (cin >> ID && ID != "END_OF_VERTICES") {
    Node *node = find_or_create_node(ID);
    int k;
    string child_id;
    cin >> k;
    for(int i=0;i<k;i++) {
      cin >> child_id;
      Node *child = find_or_create_node(child_id);
      child->parent = node;
      node->children.push_back(child);
    }
    getline(cin, node->style);
    boost::trim(node->style);
    cerr << "Read node [" << node->id << "], style=[" << node->style << "]" << '\n';
  }
}
void read_edges() {

}

void dfs(Node *x, double &current_width, int depth = 0) {
  cerr << x->id << endl;
  double l=current_width+1e9, r=current_width-1e9;
  for (auto &&y: x->children) {
    dfs(y, current_width, depth+1);
    l = min(l, y->pos.x);
    r = max(r, y->pos.x);
  }
  cerr << x->id << ":" << l << ", " << r << endl;
  x->pos.y = depth;
  x->pos.x = (l+r)/2;
  current_width = max(current_width, x->pos.x + 1);
}

void compute_positions() {
  double current_width = 0;
  for (auto &&x: pools) {
    cerr << x.second->id << endl;
    if (x.second->parent == NULL) {
      dfs(x.second, current_width);
    }
  }
}

void generate_output() {
  compute_positions();

  ifstream preamble("preamble.tex");
  string line;
  while(getline(preamble, line) && !preamble.eof()) {
    cout << line << '\n';
  }
  cout << "\\begin{document}" << '\n';
  cout << "\\begin{tikzpicture}" << '\n';
  for (auto &&x: pools) {
    x.second->output_node_definition(cout);
  }
  for (auto &&x: pools) {
    x.second->output_edges(cout);
  }
  cout << "\\end{tikzpicture}" << '\n';
  cout << "\\end{document}" << '\n';
}

int main(int argc, char *argv[]) {
  /* input format:
    ID n_children ID_1 ID_2 ID_3... ID_n [style]
    ...
    END_OF_VERTICES
    ...
    ID_u ID_v [style]
    ...
    END_OF_EDGES
    */
  read_vertices();
  read_edges();
  generate_output();
  return 0;
}
