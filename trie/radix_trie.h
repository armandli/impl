// radix tree, or prefix trie optimize space by merging child with parent
// having only a single child. the edges now include a label representing
// segment of the prefix
// futher optimzation avaiable such as using original string object and
// have edge prefix locating the begin and end substring of the original
// key object

#include <cassert>
#include <string>
#include <string_view>
#include <utility>
#include <vector>
#include <iterator>

namespace s = std;

class RadixTrie {
  struct Node;
  struct Edge {
    s::string seg;
    Node*     child;

    Edge(const Edge&) = delete;
    Edge& operator=(const Edge&) = delete;

    Edge(): seg(), child(nullptr) {}
    Edge(const s::string& s, Node* c): seg(s), child(c) {}
    Edge(Edge&& o): seg(s::move(o.seg)), child(o.child) {
      o.child = nullptr;
    }
    Edge& operator=(Edge&& o){
      seg = s::move(o.seg);
      child = o.child;
      o.child = nullptr;
      return *this;
    }
    ~Edge(){
      if (child){
        delete child;
        child = nullptr;
      }
    }
  };
  struct Node {
    s::vector<Edge> children;
    bool            terminal;

    Node(): children(), terminal(false) {}
    explicit Node(bool is_term): terminal(is_term) {}
  };

  template <typename IT1, typename IT2>
  s::pair<IT1, IT2> mismatch(IT1 begin1, IT1 end1, IT2 begin2, IT2 end2) const {
    while (begin1 != end1 && begin2 != end2 && *begin1 == *begin2){
      ++begin1;
      ++begin2;
    }
    return s::make_pair(begin1, begin2);
  }

  enum MatchState {
    NoMatch = 0,
    PartialMatch,
    FullMatch,
  };

  struct MatchResult {
    MatchState state;
    s::string_view::const_iterator key_match_iter;
    s::string::const_iterator edge_match_iter;

    MatchResult() = default;
    MatchResult(MatchState st, s::string_view::const_iterator ki, s::string::const_iterator ei):
      state(st), key_match_iter(ki), edge_match_iter(ei) {}
  };

  MatchResult prefix_compare(const s::string_view& key, const s::string& edge) const {
    s::pair<s::string::const_iterator, s::string_view::const_iterator> c = mismatch(edge.cbegin(), edge.cend(), key.cbegin(), key.cend());
    if (c.first == edge.cbegin())    return MatchResult(NoMatch, c.second, c.first);
    else if (c.first == edge.cend()) return MatchResult(FullMatch, c.second, c.first);
    else                             return MatchResult(PartialMatch, c.second, c.first);
  }

  struct TrieMatchResult {
    s::string_view key_segment;
    const Node*    match_node;
    size_t         match_edge_idx;
    MatchResult    last_match;

    TrieMatchResult(const s::string_view& ks, const Node* node): key_segment(ks), match_node(node), match_edge_idx(~0UL), last_match() {}
  };

  TrieMatchResult trie_match(const s::string& str) const {
    TrieMatchResult ret(str.c_str(), &mRoot);
    while (ret.key_segment.size() > 0){
      bool full_match = false;
      for (size_t i = 0; i < ret.match_node->children.size(); ++i){
        MatchResult m = prefix_compare(ret.key_segment, ret.match_node->children[i].seg);
        switch (m.state){
        case NoMatch:
        break;
        case PartialMatch:
          ret.match_edge_idx = i;
          ret.last_match = m;
        goto PartialMatchFound;
        case FullMatch:
          ret.key_segment.remove_prefix(s::distance(ret.key_segment.cbegin(), m.key_match_iter));
          ret.match_node = ret.match_node->children[i].child;
          ret.match_edge_idx = i;
          ret.last_match = m;
          full_match = true;
        goto FullMatchFound;
        default: assert(false);
        }
      }
  FullMatchFound:
      if (not full_match){
        ret.last_match.state = NoMatch;
        break;
      }
    }
  PartialMatchFound:
    return ret;
  }

  bool is_full_match(const TrieMatchResult& r) const {
    if (r.last_match.state == FullMatch && r.match_node->terminal) return true;
    else                                                     return false;
  }

  Node mRoot;
public:
  bool insert(const s::string& str){
    TrieMatchResult r = trie_match(str);
    Node* match_node = const_cast<Node*>(r.match_node);
    switch (r.last_match.state){
    case NoMatch:
      match_node->children.emplace_back(s::string(r.key_segment.data(), r.key_segment.size()), new Node(true));
    break;
    case PartialMatch: {
      Edge& old_edge = match_node->children[r.match_edge_idx];
      old_edge.seg = s::string(r.last_match.edge_match_iter, old_edge.seg.cend());
      s::string_view common_prefix = r.key_segment;
      common_prefix.remove_suffix(s::distance(r.last_match.key_match_iter, common_prefix.cend()));
      s::string_view new_suffix = r.key_segment;
      new_suffix.remove_prefix(s::distance(new_suffix.begin(), r.last_match.key_match_iter));
      Node* middle_node = new Node();
      middle_node->children.emplace_back(old_edge.seg, old_edge.child);
      if (new_suffix.size() > 0)
        middle_node->children.emplace_back(s::string(new_suffix.data(), new_suffix.size()), new Node(true));
      else
        middle_node->terminal = true;
      match_node->children[r.match_edge_idx].seg = s::string(common_prefix.data(), common_prefix.size());
      match_node->children[r.match_edge_idx].child = middle_node;
    }
    break;
    case FullMatch:
      match_node->terminal = true;
    break;
    default: assert(false);
    }
    return true;
  }
  bool exist(const s::string& str) const {
    TrieMatchResult r = trie_match(str);
    if (is_full_match(r)) return true;
    else                  return false;
  }
  bool remove(const s::string& str){
    TrieMatchResult r = trie_match(str);
    Node* node = const_cast<Node*>(r.match_node);
    if (is_full_match(r)){
      node->terminal = false;
      return true;
    }
    return false;
  }
  size_t size() const {
    struct Recursion {
      size_t operator()(const Node* node){
        size_t ret = node->terminal;
        for (decltype(node->children.begin()) it = node->children.begin(); it != node->children.end(); ++it)
          ret += (*this)((*it).child);
        return ret;
      }
    } recursion;
    return recursion(&mRoot);
  }
};
