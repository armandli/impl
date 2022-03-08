#ifndef __KD_TREE_V1__
#define __KD_TREE_V1__

#include <cassert>
#include <ctime>
#include <cmath>
#include <vector>
#include <array>
#include <numeric>
#include <algorithm>
#include <random>
#include <queue>
#include <sstream>

#include <iostream>

namespace s = std;

s::default_random_engine& get_default_random_engine() {
  static s::default_random_engine engine(s::time(0));
  return engine;
}

template <typename DTY, size_t DIM, typename T>
s::array<DTY, DIM> coordinate_of(const T& obj);

template <typename DTY, size_t DIM>
DTY distance(const s::array<DTY,DIM>& a, const s::array<DTY,DIM>& b){
  DTY d{};
  for (size_t i = 0; i < DIM; ++i)
    d += pow(a[i] - b[i], 2);
  return d;
}

template <typename T, size_t DIM, typename DTY=float>
class kdtree_v1 {
  struct Node {
    T obj;
    Node* l;
    Node* r;

    Node(const T& obj): obj(obj), l(nullptr), r(nullptr) {}
    ~Node(){
      if (l) delete l;
      if (r) delete r;
      l = r = nullptr;
    }
  };
  Node* root = nullptr;
protected:
  void construct_tree(const s::vector<T>& data){
    s::vector<size_t> idxes(data.size());
    s::iota(s::begin(idxes), s::end(idxes), 0UL);

    struct Recursion {
      const s::vector<T>& data;
      Recursion(const s::vector<T>& data): data(data) {}

      size_t median(const s::vector<T>& data, s::vector<size_t>& idxes, size_t beg, size_t end, size_t dim, size_t midx){
        if (end == beg){
          assert(midx == 0);
          return beg;
        }
        s::uniform_int_distribution<size_t> uniform(beg, end);
        size_t pidx = uniform(get_default_random_engine());
        s::swap(idxes[pidx], idxes[beg]);
        s::array<DTY,DIM> pcord = coordinate_of<DTY,DIM>(data[idxes[beg]]);
        size_t more_beg = end+1;
        size_t less_end = beg;
        while (more_beg > less_end+1){
          s::array<DTY,DIM> cord = coordinate_of<DTY,DIM>(data[idxes[less_end+1]]);
          if (cord[dim] < pcord[dim]){
            less_end++;
          } else {
            s::swap(idxes[less_end+1], idxes[--more_beg]);
          }
        }
        size_t less_sz = less_end - beg;
        size_t more_sz = end+1 - more_beg;
        if (less_sz == midx)
          return beg;
        else if (less_sz > midx){
          assert(less_sz > 0);
          return median(data, idxes, beg+1, less_end, dim, midx);
        } else {
          assert(more_sz > 0);
          return median(data, idxes, more_beg, end, dim, midx-less_sz-1);
        }
      }

      Node* operator()(s::vector<size_t>& idxes, size_t beg, size_t end, size_t dim){
        if (end == beg){
          Node* node = new Node(data[idxes[beg]]);
          return node;
        }
        size_t pidx = median(data, idxes, beg, end, dim, (end-beg)/2);
        s::swap(idxes[beg], idxes[pidx]);
        s::array<DTY,DIM> pcord = coordinate_of<DTY,DIM>(data[idxes[beg]]);
        size_t more_beg = end+1;
        size_t less_end = beg;
        while (more_beg > less_end+1){
          s::array<DTY,DIM> cord = coordinate_of<DTY,DIM>(data[idxes[less_end+1]]);
          if (cord[dim] < pcord[dim]){
            less_end++;
          } else {
            s::swap(idxes[less_end+1], idxes[--more_beg]);
          }
        }
        size_t less_sz = less_end - beg;
        size_t more_sz = end+1 - more_beg;
        Node* node = new Node(data[idxes[beg]]);
        if (less_sz > 0)
          node->l = this->operator()(idxes, beg+1, less_end, (dim+1)%DIM);
        if (more_sz > 0)
          node->r = this->operator()(idxes, more_beg, end, (dim+1)%DIM);
        return node;
      }
    } recursion(data);

    root = recursion(idxes, 0, idxes.size()-1, 0);
  }
public:
  kdtree_v1() = default;
  kdtree_v1(const s::vector<T>& data){
    if (data.size() == 0) return;
    construct_tree(data);
  }
  ~kdtree_v1(){
    if (root) delete root;
    root = nullptr;
  }

  template <typename F>
  T* neighbour(const s::array<DTY,DIM>& pt, F dist_f){
    struct Recursion {
      const s::array<DTY,DIM>& pt;
      F* dist_f;
      Recursion(const s::array<DTY,DIM>& pt, F* dist_f): pt(pt), dist_f(dist_f) {}
      Node* operator()(Node* node, size_t dim){
        if (node == nullptr) return nullptr;
        s::array<DTY,DIM> ncord = coordinate_of<DTY,DIM>(node->obj);
        DTY bdist = dist_f(ncord, pt);
        Node* best = node;
        Node* tmp = nullptr;
        if (pt[dim] < ncord[dim]){
          tmp = this->operator()(node->l, (dim+1)%DIM);
        } else {
          tmp = this->operator()(node->r, (dim+1)%DIM);
        }
        if (tmp){
          s::array<DTY,DIM> cord = coordinate_of<DTY,DIM>(tmp->obj);
          DTY dist = dist_f(cord, pt);
          if (dist < bdist){
            best = tmp;
            bdist = dist;
          }
        }
        //heuristic
        if (bdist > s::abs(ncord[dim] - pt[dim])){
          if (pt[dim] < ncord[dim]){
            tmp = this->operator()(node->r, (dim+1)%DIM);
          } else {
            tmp = this->operator()(node->l, (dim+1)%DIM);
          }
          if (tmp){
            s::array<DTY,DIM> cord = coordinate_of<DTY,DIM>(tmp->obj);
            DTY dist = dist_f(cord, pt);
            if (dist < bdist){
              bdist = dist;
              best = tmp;
            }
          }
        }
        return best;
      }
    } recursion(pt, dist_f);

    Node* node = recursion(root, 0);
    if (node) return &node->obj;
    else      return nullptr;
  }

  s::string print() const {
    if (root == nullptr) return "";

    s::stringstream ss;
    s::queue<Node*> q; q.push(root);
    while (not q.empty()){
      Node* node = q.front(); q.pop();
      s::array<DTY,DIM> cord = coordinate_of<DTY,DIM>(node->obj);
      ss << "(";
      for (size_t i = 0; i < DIM; ++i)
        ss << cord[i] << ",";
      ss << ")\n";

      if (node->l) q.push(node->l);
      if (node->r) q.push(node->r);
    }

    return ss.str();
  }
};

#endif//__KD_TREE_V1__
