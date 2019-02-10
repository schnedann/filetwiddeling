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

#include <fstream>
#include <sys/stat.h>
#include <cstdio>
#include <cerrno>
#include <cstring>

#include "file_fkt.h"

/**
 * @brief getFileSize
 * @param filename
 * @return
 */
std::pair<bool,size_t> File_Fkt::getFileSize(std::string const& filename){
  struct stat stat_buf;
  int rc = stat(filename.c_str(), &stat_buf);
  return std::make_pair((rc!=0),stat_buf.st_size);
}

/**
 * @brief getFileSize
 * @param fd
 * @return
 */
/*std::pair<bool,size_t> File_Fkt::getFileSize(int const fd){
  struct stat stat_buf;
  int rc = fstat(fd, &stat_buf);
  return std::make_pair((rc!=0),stat_buf.st_size);
}*/

/**
 * @brief getBlockSize
 * @param filename
 * @return
 */
std::pair<bool,size_t> File_Fkt::getBlockSize(std::string const& filename){
  struct stat stat_buf;
  int rc = stat(filename.c_str(), &stat_buf);
  return std::make_pair((rc!=0),stat_buf.st_blksize);
}

/**
 * @brief read_File2String
 * @param filename
 * @return
 */
std::string File_Fkt::read_File2String(std::string const& filename){
  std::string contents;
  auto fsize = getFileSize(filename);
  if(!fsize.first){
    contents.resize(fsize.second);
    try {
      std::ifstream file(filename, std::ios::binary | std::ios::in);
      if(file){
        file.read(&contents[0], long(contents.size()));
        file.close();
      }
    } catch(const std::exception& e){
      std::cout << e.what();
    }
  }
  return(contents);
}

/**
 * @brief copy_File
 * @param source - Source File Name
 * @param target - Target File Name
 * @return true on Error
 */
bool File_Fkt::copy_File(std::string const& source, std::string const& target){
  bool err = true;
  try {
    std::ifstream  src(source, std::ios::binary | std::ios::in);
    std::ofstream  dst(target, std::ios::binary | std::ios::out);
    if(src){
      if(dst){
        dst << src.rdbuf();
        err = false;
        dst.close();
      }
      src.close();
    }
  } catch(const std::exception& e){
    std::cout << e.what();
  }
  return err;
}

/**
 * @brief rename_File
 * @param source - File 2 Rename
 * @param target - Target File Name
 * @return true on Error
 */
bool File_Fkt::rename_File(std::string const& source, std::string const& target){
  bool err=false;
  if(0>std::rename(source.c_str(), target.c_str())){
    std::cout << std::strerror(errno) << '\n';
    err = true;
  }
  return err;
}

bool File_Fkt::write_block(const std::string &filename, std::pair<std::unique_ptr<u8[]>, size_t> buffer, size_t offset)
{
  bool err = true;

  return err;
}

/**
 * @brief File_Fkt::read_block
 * @param filename
 * @param offset
 * @param length
 * @return returns a pointer to a self managed, self destructing intermeadiate buffer if first element is false
 *         error if first element is true
 */
std::pair<bool, std::pair<std::unique_ptr<u8[]>, size_t> > File_Fkt::read_block(const std::string &filename, size_t offset, size_t length){
  auto res = std::make_pair(true,std::make_pair(std::unique_ptr<u8[]>(nullptr),size_t(0)));
  auto fsize = getFileSize(filename);
  if(!fsize.first){
    if((offset+length)<=fsize.second){
      std::unique_ptr<u8[]> ptr(new u8[length]);
      try {
        std::ifstream file(filename, std::ios::binary | std::ios::in);
        if(file){
          file.read(reinterpret_cast<char*>(ptr.get()), long(length));
          file.close();
          res.first = false;
          res.second.first = std::move(ptr);
          res.second.second = length;
        }
      } catch(const std::exception& e){
        std::cout << e.what();
      }
    }
  }
  return res;
}

/*void removeLine(char* sourcefile,int line){
  ifstream infile;
  char tempPath[100]="D:/tempfile.txt";
  infile.open(sourcefile,ios::in);
  if(infile){
    int numLine=countLine(sourcefile);
    if(numLine<line){
       cout<<"\nNo line to delete\n.";
       return;
    }
    ofstream outfile;
    outfile.open(tempPath,ios::out);
    char data[100];
    int i=0;
    while(infile.getline(data,100)){
       i++;
       if(i==line) continue;
       outfile<<data<<"\n";
    }
    outfile.close();
  }
  infile.close();
 remove(sourcefile);
 rename(tempPath,sourcefile);
}

int countLine(char* sourcefile){
  ifstream infile;
  infile.open(sourcefile,ios::in);
  char data[100];
  int line=0;
  while(infile.getline(data,100)) line++;
  return line;
}*/
