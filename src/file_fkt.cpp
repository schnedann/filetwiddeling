//for Copyright see this file
#include "modified_4Cbsd.h"

#include <fstream>
#include <cstdio>
#include <cerrno>
#include <cstring>
#include <string>
#include <string_view>

#include "file_fkt.h"
#include "enviroment_detection.h"

#if (0<dIS_LINUX)
  #include <sys/stat.h>
  #include <unistd.h>
#endif

//-----

#define USE_FILESYSTEM 0
#if (0<dIS_MINGW)
  #include <boost/filesystem.hpp>
  #include <boost/system.hpp>
  namespace fs=boost::filesystem;
  namespace sys = boost::system;
  #undef  USE_FILESYSTEM
  #define USE_FILESYSTEM 1
#else
  #include <filesystem>
  #include <system_error>
  namespace fs=std::filesystem;
  namespace sys=std;
  #undef  USE_FILESYSTEM
  #define USE_FILESYSTEM 1
#endif

//-----

std::pair<bool,size_t> File_Fkt::getFileSize(std::string_view const filename){
#if (0<USE_FILESYSTEM)
  std::error_code ec;
  auto size = fs::file_size(filename,ec);
  return {ec.operator bool(),size};
#elif defined(PREDEF_PLATFORM_UNIX)
  struct stat stat_buf;
  int rc = stat(filename.data(), &stat_buf);
  return {(rc!=0),stat_buf.st_size};
#endif
}

#if defined(PREDEF_PLATFORM_UNIX)
std::pair<bool,size_t> File_Fkt::getBlockSize(std::string_view const filename){
  struct stat stat_buf;
  int rc = stat(filename.data(), &stat_buf);
  return {(rc!=0),stat_buf.st_blksize};
}
#endif

//-----

std::string File_Fkt::read_File2String(fs::path const path2file){
  std::string contents;
  auto fsize = getFileSize(path2file.string());
  if(!fsize.first){
    contents.resize(fsize.second);
    try {
      std::ifstream file(path2file, std::ios::binary | std::ios::in);
      if(file){
        file.read(&contents[0], long(contents.size()));
        file.close();
      }
    } catch(const std::exception& e){
      std::cerr << e.what();
    }
  }
  return(contents);
}

std::pair<bool, std::pair<std::unique_ptr<u8[]>, size_t> > File_Fkt::read_block(fs::path const path2file,
                                                                                size_t offset,
                                                                                size_t length){
  //Error, empty buffer, size 0
  auto res = std::make_pair(true,std::make_pair(std::unique_ptr<u8[]>(nullptr),size_t(0)));
  auto fsize = getFileSize(path2file.string());
  if(!fsize.first){
    bool OK  = (length<=fsize.second);          //Block needs to be smaller or equal than File
         OK &= ((offset+length)<=fsize.second); //Last byte of block needs to be within file
    if(OK){
      std::unique_ptr<u8[]> ptr(new u8[length]);
      try {
        std::ifstream file(path2file, std::ios::binary | std::ios::in);
        if(file){
          file.read(reinterpret_cast<char*>(ptr.get()), long(length));
          file.close();
          res.first = false;
          res.second.first = std::move(ptr);
          res.second.second = length;
        }
      } catch(const std::exception& e){
        std::cerr << e.what();
      }
    }
  }
  return res;
}

//-----

bool File_Fkt::write_block(const std::string &filename, std::pair<std::unique_ptr<u8[]>, size_t> buffer, size_t offset)
{
  bool err = true;

  return err;
}

//-----

bool File_Fkt::copy_File(fs::path const path2source, fs::path const path2target){
  bool err = true;
#if (0<USE_FILESYSTEM)
  std::error_code ec;
  err  = !fs::copy_file(path2source,path2target,fs::copy_options::overwrite_existing,ec);
  err |= ec.operator bool();
#else
  try {
    std::ifstream  src(path2source, std::ios::binary | std::ios::in);
    std::ofstream  dst(path2target, std::ios::binary | std::ios::out);
    if(src){
      if(dst){
        dst << src.rdbuf();
        err = false;
        dst.close();
      }
      src.close();
    }
  } catch(const std::exception& e){
    std::cerr << e.what();
  }
#endif
  return err;
}

bool File_Fkt::rename_File(fs::path const path2source, fs::path const path2target){
  bool err=false;
#if (0<USE_FILESYSTEM)
  fs::rename(path2source,path2target);
#else
  if(0>std::rename(path2source.string().c_str(), path2target.string().c_str())){
    std::cerr << std::strerror(errno) << '\n';
    err = true;
  }
#endif
  return err;
}

//-----

#if defined(PREDEF_PLATFORM_UNIX)

bool File_Fkt::chown_File(fs::path const path2file, uid_t owner, gid_t group){
  bool err=false;
  if(0>chown(path2file.string().c_str(), owner, group)){
    std::cerr << std::strerror(errno) << '\n';
    err = true;
  }
  return err;
}

bool File_Fkt::chmod_File(fs::path const path2file, mode_t mode){
  bool err=false;
  if(0>chmod(path2file.string().c_str(), mode)){
    std::cerr << std::strerror(errno) << '\n';
    err = true;
  }
  return err;
}

#endif


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
