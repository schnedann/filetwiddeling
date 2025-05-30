#ifndef FILE_FKT_H
#define FILE_FKT_H

//for Copyright see this file
#include "modified_4Cbsd.h"

#include <iostream>
#include <string>
#include <string_view>
#include <array>
#include <fstream>
#include <memory>
#include <utility>

#include "enviroment_detection.h"

#include "dtypes.h"

#include "enviroment_detection.h"
#if (0<dIS_MINGW)
#include <boost/filesystem.hpp>
namespace fs=boost::filesystem;
#else
#include <filesystem>
namespace fs=std::filesystem;
#endif

namespace File_Fkt {

/**
 * @brief getFileSize
 * @param filename
 * @return
 */
std::pair<bool,size_t> getFileSize(std::string_view const filename);

#if defined(PREDEF_PLATFORM_UNIX)

/**
 * @brief getBlockSize
 * @param filename
 * @return
 */
std::pair<bool,size_t> getBlockSize(std::string_view const filename);

#endif

//-----

/**
 * @brief read_File2String
 * @param path2file
 * @return
 */
std::string read_File2String(fs::path const path2file);

/**
 * @brief read_from_File - Open File and fill supplied buffer
 * @return true on error e.g. file does not exist, failed to open, or is smaller than the buffer
 */
template<size_t CNT> bool read_from_File(fs::path const path2file,
                    std::array<u8,CNT>& buffer){
  bool err = true;
  auto fsize = getFileSize(path2file.string());
  if(!fsize.first){
    if(fsize.second>=buffer.size()){
      try {
        std::ifstream file(path2file, std::ios::binary | std::ios::in);
        if(file){
          file.read(reinterpret_cast<char*>(buffer.data()),long(buffer.size()));
          file.close();
          err = false;
        }
      } catch(const std::exception& e){
        std::cerr << e.what();
      }
    }
  }
  return err;
}

bool write_block(std::string const& filename, std::pair<std::unique_ptr<u8[]>,size_t> buffer, size_t offset);

/**
 * @brief File_Fkt::read_block
 * @param filename
 * @param offset
 * @param length
 * @return returns a pointer to a self managed, self destructing intermeadiate buffer if first element is false
 *         error if first element is true
 */
std::pair<bool,std::pair<std::unique_ptr<u8[]>,size_t>> read_block(fs::path const path2file,
                                                                   size_t offset,
                                                                   size_t length);

//-----

/**
 * @brief copy_File
 * @param path2source - Source File Name
 * @param path2target - Target File Name
 * @return true on Error
 */
bool copy_File(fs::path const path2source, fs::path const path2target);

/**
 * @brief rename_File
 * @param path2source - File 2 Rename
 * @param path2target - Target File Name
 * @return true on Error
 */
bool rename_File(fs::path const path2source, fs::path const path2target);

//-----

#if defined(PREDEF_PLATFORM_UNIX)
/**
 * @brief chown_File
 * @param path2file
 * @param owner
 * @param group
 * @return
 */
bool chown_File(fs::path const path2file, uid_t owner, gid_t group);

/**
 * @brief chmod_File
 * @param path2file
 * @param mode
 * @return
 */
bool chmod_File(fs::path const path2file, mode_t mode);
#endif

//-----

}

#endif // FILE_FKT_H
