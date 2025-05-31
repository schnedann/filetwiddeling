#include "file_list_format.h"

#include <string>
#include <sstream>
#include <vector>
#include <set>

#include "dtypes.h"
#include "file_fkt.h"
#include "ansiconsolecolor.h"

namespace {

auto const str_extend = [](std::string& str, size_t target_length){
  size_t pos = str.size();
  str.resize(target_length);
  for(;pos<target_length;++pos){
    str.at(pos) = ' ';
  }
  return;
};

} //namespace

//---

size_t File_Fkt::List_Format::get_max_path_length(std::set<fs::directory_entry> const& list){
  size_t path_maxl = 0;
  //longest path
  for(auto const& entry:list){
    size_t tmp = entry.path().string().size();
    if(path_maxl < tmp){
      path_maxl = tmp;
    }
  }
  return path_maxl;
}

std::vector<std::string> File_Fkt::List_Format::get_permissions_lst(std::set<fs::directory_entry> const& list){
  std::vector<std::string> permissions_lst(list.size());
  {
    size_t idx = 0;
    for(auto const& entry:list){

      if(!entry.is_directory() &&
         !entry.is_socket() &&
         !entry.is_symlink() &&
         !entry.is_other()){
        auto perms = std::filesystem::status(entry).permissions();

        auto perm2str = [](std::string& str, std::filesystem::perms const& pperms){
          std::stringstream ss;
          using std::filesystem::perms;
          auto show = [&ss, &pperms](char op, perms perm)
          {
            ss << (std::filesystem::perms::none == (perm & pperms) ? '-' : op);
          };
          show('r', std::filesystem::perms::owner_read);
          show('w', std::filesystem::perms::owner_write);
          show('x', std::filesystem::perms::owner_exec);
          show('r', std::filesystem::perms::group_read);
          show('w', std::filesystem::perms::group_write);
          show('x', std::filesystem::perms::group_exec);
          show('r', std::filesystem::perms::others_read);
          show('w', std::filesystem::perms::others_write);
          show('x', std::filesystem::perms::others_exec);

          str = ss.str();

          return;
        };

        perm2str(permissions_lst.at(idx),perms);
      }

      ++idx;
    }

    //---

    size_t maxl = 0;
    for(auto const& str:permissions_lst){
      size_t tmp = str.size();
      if(maxl < tmp){
        maxl = tmp;
      }
    }

    //---

    for(auto& str:permissions_lst){
      str_extend(str,maxl);
    }
  } //scope
  return permissions_lst;
}

std::vector<std::string> File_Fkt::List_Format::get_what_entry_lst(std::set<fs::directory_entry> const& list){
  std::vector<std::string> what_entry_lst(list.size());
  {
    size_t idx = 0;
    for(auto const& entry:list){

      if(entry.is_block_file()){
        what_entry_lst.at(idx) = "BlockFile";
      }
      if(entry.is_character_file()){
        what_entry_lst.at(idx) = "CharacterFile";
      }
      if(entry.is_regular_file()){
        what_entry_lst.at(idx) = std::string(Utility::AnsiColor::fghighred) + "File" + std::string(Utility::AnsiColor::reset_all);
      }

      if(entry.is_directory()){
        what_entry_lst.at(idx) = std::string(Utility::AnsiColor::fgcyan) + "Directory" + std::string(Utility::AnsiColor::reset_all);
      }
      if(entry.is_fifo()){
        what_entry_lst.at(idx) = "FiFo";
      }
      if(entry.is_socket()){
        what_entry_lst.at(idx) = std::string(Utility::AnsiColor::fghighyellow) + "Socket" + std::string(Utility::AnsiColor::reset_all);
      }

      if(entry.is_symlink()){
        what_entry_lst.at(idx) = std::string(Utility::AnsiColor::fgmagenta) + "Symlink" + std::string(Utility::AnsiColor::reset_all) ;
      }
      ++idx;
    }

    //---

    size_t maxl = 0;
    for(auto const& str:what_entry_lst){
      size_t tmp = str.size();
      if(maxl < tmp){
        maxl = tmp;
      }
    }

    //---

    for(auto& str:what_entry_lst){
      str_extend(str,maxl);
    }
  } //scope
  return what_entry_lst;
}

std::vector<std::string> File_Fkt::List_Format::get_file_size_lst(std::set<fs::directory_entry> const& list){
  std::vector<std::string> file_size_lst(1 + list.size());
  {
    auto const fsize_fmt = [](size_t const fsize)->std::string{
      auto lfsize = fsize;
      size_t ii=0;
      static constexpr size_t const onekB = 1024;
      while(onekB <= lfsize){
        lfsize /= onekB;
        ++ii;
      }

      std::string file_size = std::to_string(lfsize);
      switch (ii) {
      case 0:
        file_size += " Byte";
        break;
      case 1:
        file_size += " kB";
        break;
      case 2:
        file_size += " MB";
        break;
      case 3:
        file_size += " GB";
        break;
      case 4:
        file_size += " TB";
        break;
      case 5:
        file_size += " PB";
        break;
      default:
        file_size = std::to_string(fsize) + " Byte";
        break;
      }
      return file_size;
    };

    size_t size_all = 0;
    size_t idx = 0;
    for(auto const& entry:list){

      if(!entry.is_directory() &&
         !entry.is_socket() &&
         !entry.is_symlink() &&
         !entry.is_other()){

        auto fsize = entry.file_size();
        size_all += fsize;

        auto const file_size = fsize_fmt(fsize);

        file_size_lst.at(idx) = file_size;
      }

      ++idx;
    }

    {
      auto const file_size = fsize_fmt(size_all);
      file_size_lst.at(idx) = file_size;
    }

    //---

    size_t maxl = 0;
    for(auto const& str:file_size_lst){
      size_t tmp = str.size();
      if(maxl < tmp){
        maxl = tmp;
      }
    }

    //---

    for(auto& str:file_size_lst){
      str_extend(str,maxl);
    }

    //---

    for(auto& str:file_size_lst){
      str = std::string(Utility::AnsiColor::fghighgreen )+ str + std::string(Utility::AnsiColor::reset_all);
    }

  } //scope
  return file_size_lst;
}

std::vector<std::string> File_Fkt::List_Format::get_path_lst(std::set<fs::directory_entry> const& list){
  auto const path_maxl = 4 + File_Fkt::List_Format::get_max_path_length(list);
  std::vector<std::string> path_lst(list.size());
  {
    size_t idx = 0;
    for(auto const& entry:list){
      auto path = entry.path().string();
      auto file = std::string("    ") + entry.path().filename().string();

      if(!entry.is_directory()){
        str_extend(file,path_maxl);
        file = std::string(Utility::AnsiColor::fgwhite) + std::string(Utility::AnsiColor::bold_on) + file + std::string(Utility::AnsiColor::reset_all);
        path_lst.at(idx) = file;
      }else{
        str_extend(path,path_maxl);
        path = std::string(Utility::AnsiColor::fgcyan) + path + std::string(Utility::AnsiColor::reset_all);
        path_lst.at(idx) = path;
      }

      ++idx;
    }
  } //scope
  return path_lst;
}
