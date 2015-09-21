#include <sstream>

#include "csv_reader.h"

csv_reader::csv_reader(const char* filename, const std::set<Column>& columns, const char* delimiter, int max_size) :
    m_fd(fopen64(filename, "r")), m_delim(delimiter), m_max_sz(max_size){
  if (m_fd == NULL){
    std::stringstream ss;
    ss << "Failed to open file " << filename;
    m_error = ss.str();
    return;
  }
  if (m_max_sz <= 0){
    m_error = "Maximum line size <= 0";
    return;
  }
  read_header(columns);
}

void csv_reader::read_header(const std::set<Column>& columns){
  Buffer buffer(m_max_sz);
  if (buffer.m_buff == 0){
    m_error = "buffer Bad alloc";
    close();
    return;
  }

  get_line(buffer, m_max_sz, m_fd);

  std::set<Column> found;
  char* pc = strtok(buffer, m_delim.c_str());
  for (int col_count = 0; pc; col_count++, pc = strtok(NULL, m_delim.c_str())){
    Column c = {pc, false};
    std::set<Column>::iterator it = columns.find(c);
    if (columns.size() == 0 || it != columns.end()){
      found.insert(c);
      m_columns.insert(std::make_pair(col_count, std::string(c.m_name)));
      m_linemap.insert(std::make_pair(std::string(c.m_name), (const char*)0));
    }
  }

  if (columns.size() == 0) return;

  for (std::set<Column>::iterator it = columns.begin(); it != columns.end(); ++it)
    if ((*it).m_required && found.find(*it) == found.end()){
      std::stringstream ss;
      ss << "Column " << (*it).m_name << " not found";
      m_error = ss.str();
      close();
      return;
    }
}

void csv_reader::read(Buffer& buffer){
  char* pc = strtok(buffer, m_delim.c_str());
  for (int col = 0; pc; ++col, pc = strtok(NULL, m_delim.c_str())){
    std::map<int, std::string>::iterator it = m_columns.find(col);
    if (it != m_columns.end()){
      m_linemap[(*it).second] = pc;
    }
  }
}

bool csv_reader::is_open() const {
  return m_fd != NULL;
}

const char* csv_reader::error() const {
  return m_error.c_str();
}

bool csv_reader::read_line(){
  if (m_fd == NULL) return false;

  Buffer buffer(m_max_sz);
  if (buffer.m_buff == NULL){
    m_error = "Buffer bad alloc";
    close();
    return false;
  }

  get_line(buffer, m_max_sz, m_fd);

  read(buffer);
  return true;
}

void csv_reader::process(callback func){
  if (m_fd == NULL) return;

  Buffer buffer(m_max_sz);
  if (buffer.m_buff == NULL){
    m_error = "Buffer bad alloc";
    close();
    return;
  }

  for (unsigned lineno = 0; get_line(buffer, m_max_sz, m_fd); ++lineno){
    read(buffer);
    if (not func(map(), lineno)) break;
  }
}
