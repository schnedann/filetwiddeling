#ifndef FILE_LIST_H
#define FILE_LIST_H

#include <vector>
#include <functional>
#include <set>

//#include "dtypes.h"

#include "enviroment_detection.h" // IWYU pragma: export
#if (0<dIS_MINGW)
#include <boost/filesystem.hpp>
namespace fs=boost::filesystem;
#else
#include <filesystem>
namespace fs=std::filesystem;
#endif

namespace File_Lst {

/**
 * @brief list_worker - create sorted filelist, filtered by worker functions
 * @param fs_target
 * @param file_lst
 * @param workers
 * @param recursive
 */
void list_worker(fs::path const fs_target,
                 std::set<fs::directory_entry>& file_lst,
                 std::vector<std::function<bool(fs::directory_entry const de)>> const workers,
                 bool const recursive);

} //namespace

#endif // FILE_LIST_H
