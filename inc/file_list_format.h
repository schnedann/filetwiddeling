#ifndef FILE_LIST_FORMAT_H
#define FILE_LIST_FORMAT_H

#include <string>
#include <vector>
#include <set>

#include "dtypes.h"
#include "file_fkt.h"

namespace File_Lst::Format {

void print_informative_list(std::set<fs::directory_entry> const file_lst,
                            size_t const term_width,
                            size_t const maxpath);

} //namespace

#endif // FILE_LIST_FORMAT_H
