#include <cstdio>
#include <cstring>
#include <map>
#include <set>
#include <vector>
#include <string>

struct csv_reader {
  typedef bool (&callback)(const std::map<std::string, const char*>& cmap, unsigned lineno);
  struct Column {
    const char* m_name;
    bool m_required;

    bool operator<(const Column& b) const {
      return strcmp(m_name, b.m_name) < 0;
    }
    bool operator==(const Column& b) const {
      return strcmp(m_name, b.m_name) == 0;
    }
  };
private:
  FILE* m_fd;
  std::string m_delim;
  int m_max_sz;
  std::vector<std::map<std::string, const char*>::iterator> m_columns;
  std::map<std::string, const char*> m_linemap;
  std::string m_error;

  struct Buffer {
    char* m_buff;
    int m_sz;

    Buffer(int sz) : m_buff(new (std::nothrow) char[sz]), m_sz(sz) {}
    ~Buffer(){
      if (m_buff) delete[] m_buff;
    }

    operator char*(){
      return m_buff;
    }
  };

  void close(){
    if (m_fd){
      fclose(m_fd);
      m_fd = NULL;
    }
  }
  char* get_line(char* buffer, int max_size, FILE* fd){
    char* ret = fgets(buffer, max_size, fd);
    char* el = strrchr(buffer, '\n');
    if (el) *el = '\0';
    return ret;
  }
  void read_header(const std::set<Column>& columns);
  void read(Buffer& buffer);
public:
  csv_reader(const char* filename, const std::set<Column>& columns, const char* delimiter = ",", int max_size = 4096);
  ~csv_reader(){ close(); }

  bool is_open() const { return m_fd != NULL; }
  const char* error() const { return m_error.c_str(); }

  bool read_line();
  const std::map<std::string, const char*>& map() const { return m_linemap; }

  void process(callback func);
};
