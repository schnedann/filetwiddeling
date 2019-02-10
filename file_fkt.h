/*
 *  Copyright (c) 2019,Danny Schneider
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *
 *  * Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 *  * Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 *  FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 *  DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 *  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 *  OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef FILE_FKT_H
#define FILE_FKT_H

#include <iostream>
#include <string>
#include <array>
#include <fstream>
#include <memory>
#include <utility>

#include "posix_detect.h"

#include "dtypes.h"

namespace File_Fkt {

std::pair<bool,size_t> getFileSize(std::string const& filename);
std::pair<bool,size_t> getBlockSize(std::string const& filename);
//template<size_t CNT> bool read_from_File(std::string  const& filename, std::array<u8,CNT>& buffer);
std::string read_File2String(std::string const& filename);
bool copy_File(std::string const& source, std::string const& target);
bool rename_File(std::string const& source, std::string const& target);

/**
 * @brief read_from_File - Open File and fill supplied buffer
 * @return true on error e.g. file does not exist, failed to open, or is smaller than the buffer
 */
template<size_t CNT> bool read_from_File(std::string  const& filename, std::array<u8,CNT>& buffer){
  bool err = true;
  auto fsize = getFileSize(filename);
  if(!fsize.first){
    if(fsize.second>=buffer.size()){
      try {
        std::ifstream file(filename, std::ios::binary | std::ios::in);
        if(file){
          file.read(reinterpret_cast<char*>(buffer.data()),long(buffer.size()));
          file.close();
          err = false;
        }
      } catch(const std::exception& e){
        std::cout << e.what();
      }
    }
  }
  return err;
}

bool write_block(std::string const& filename, std::pair<std::unique_ptr<u8[]>,size_t> buffer, size_t offset);
std::pair<bool,std::pair<std::unique_ptr<u8[]>,size_t>> read_block(std::string const& filename, size_t offset, size_t length);

}

#endif // FILE_FKT_H
