#ifndef FILE_LIST_FORMAT_H
#define FILE_LIST_FORMAT_H

#include <string>
#include <vector>
#include <set>

#include "dtypes.h"
#include "file_fkt.h"

namespace File_Fkt::List_Format {

size_t get_max_path_length(std::set<fs::directory_entry> const& list);

std::vector<std::string> get_permissions_lst(std::set<fs::directory_entry> const& list);

std::vector<std::string> get_what_entry_lst(std::set<fs::directory_entry> const& list);

std::vector<std::string> get_file_size_lst(std::set<fs::directory_entry> const& list);

std::vector<std::string> get_path_lst(std::set<fs::directory_entry> const& list, size_t const maxpath);

} //namespace

#endif // FILE_LIST_FORMAT_H
