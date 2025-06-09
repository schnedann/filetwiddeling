#include "file_list_format.h"

#include <filesystem>
#include <string>
#include <sstream>
#include <vector>
#include <set>
#include <thread>
#include <cmath>
#include <chrono>

#include "dtypes.h"
#include "file_fkt.h"
#include "Stringhelper.h"
#include "ansiconsolecolor.h"
#include "enviroment_detection.h"

#include "terminal.h"

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

namespace {

auto const str_extend = [](std::string& str, size_t target_length){
  size_t pos = str.size();
  str.resize(target_length);
  for(;pos<target_length;++pos){
    str.at(pos) = ' ';
  }
  return;
};

//---

/* currently unused
size_t get_max_path_length(std::set<fs::directory_entry> const& list){
  size_t path_maxl = 0;
  //longest path
  for(auto const& entry:list){
    size_t tmp = entry.path().string().size();
    if(path_maxl < tmp){
      path_maxl = tmp;
    }
  }
  return path_maxl;
}*/

//---

std::vector<std::string> get_permissions_lst(std::set<fs::directory_entry> const& list){
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

//---

std::vector<std::string> get_what_entry_lst(std::set<fs::directory_entry> const& list){

  static constexpr std::array<std::string_view,7> const Texts = {
    "BlockFile",
    "CharacterFile",
    "File",
    "Directory",
    "FiFo",
    "Socket",
    "Symlink"
  };

  //---

  size_t all_maxl = 0;
  for(auto const& str:Texts){
    size_t tmp = str.size();
    if(all_maxl < tmp){
      all_maxl = tmp;
    }
  }

  std::vector<std::string> what_entry_lst(list.size(),std::string(all_maxl+2*Utility::AnsiColor::orange.size(),' '));
  {
    {
      size_t idx = 0;
      for(auto const& entry:list){

        if(entry.is_block_file()){
          what_entry_lst.at(idx) = std::string(Texts.at(0));
        }
        if(entry.is_character_file()){
          what_entry_lst.at(idx) = std::string(Texts.at(1));
        }
        if(entry.is_regular_file()){
          what_entry_lst.at(idx) = std::string(Texts.at(2));
        }

        if(entry.is_directory()){
          what_entry_lst.at(idx) = std::string(Texts.at(3));
        }
        if(entry.is_fifo()){
          what_entry_lst.at(idx) = std::string(Texts.at(4));
        }
        if(entry.is_socket()){
          what_entry_lst.at(idx) = std::string(Texts.at(5));
        }

        if(entry.is_symlink()){
          what_entry_lst.at(idx) = std::string(Texts.at(6));
        }
        ++idx;
      }
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

    //---

    {
      size_t idx = 0;
      for(auto const& entry:list){

        /*if(entry.is_block_file()){
          what_entry_lst.at(idx) = std::string(Texts.at(0));
        }*/
        /*if(entry.is_character_file()){
          what_entry_lst.at(idx) = std::string(Texts.at(1));
        }*/
        if(entry.is_regular_file()){
          std::array<std::string_view,2> tmp_arr = {Utility::AnsiColor::light_grey,what_entry_lst.at(idx)};
          what_entry_lst.at(idx) = Utility::AnsiColor::colorize(std::span(tmp_arr.data(),tmp_arr.size()),Utility::AnsiColor::colorsel_e::none);
        }

        if(entry.is_directory()){
          std::array<std::string_view,2> tmp_arr = {Utility::AnsiColor::light_brownish,what_entry_lst.at(idx)};
          what_entry_lst.at(idx) = Utility::AnsiColor::colorize(std::span(tmp_arr.data(),tmp_arr.size()),Utility::AnsiColor::colorsel_e::none);
        }
        if(entry.is_fifo()){
          what_entry_lst.at(idx) = std::string(Texts.at(4));
        }
        if(entry.is_socket()){
          what_entry_lst.at(idx) = Utility::AnsiColor::colorize(what_entry_lst.at(idx),Utility::AnsiColor::colorsel_e::high_yellow);
        }

        if(entry.is_symlink()){
          what_entry_lst.at(idx) = Utility::AnsiColor::colorize(what_entry_lst.at(idx),Utility::AnsiColor::colorsel_e::magenta) ;
        }
        ++idx;
      }
    }

  } //scope
  return what_entry_lst;
}

//---

std::vector<std::string> get_file_size_lst(std::set<fs::directory_entry> const& list){
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
      str = Utility::AnsiColor::colorize(str,Utility::AnsiColor::colorsel_e::high_green);
    }

  } //scope
  return file_size_lst;
}

//---

std::vector<std::string> get_path_lst(std::set<fs::directory_entry> const& list, size_t const maxpath){
  std::vector<std::string> path_lst(list.size());
  {
    {
      size_t idx = 0;
      for(auto const& entry:list){

        if(!entry.is_directory()){

          auto file = entry.path().filename().string();
          if(maxpath < file.size()){
            size_t const len = (maxpath-4);
            size_t const pos = (file.size()-len);
            file = std::string("...") + file.substr(pos,len);
          }
          file = std::string("    ") + file; //indent
          path_lst.at(idx) = file;
        }else{

          auto path = entry.path().string();
          if(maxpath < path.size()){
            path = std::string("...") + path.substr((path.size()-maxpath),maxpath);
          }
          path_lst.at(idx) = path;
        }

        ++idx;
      }
    } //scope

    //---

    size_t maxl = 0;
    for(auto const& str:path_lst){
      size_t tmp = str.size();
      if(maxl < tmp){
        maxl = tmp;
      }
    }

    //---

    for(auto& str:path_lst){
      str_extend(str,maxl);
    }

    //---

    {
      size_t idx = 0;
      for(auto const& entry:list){
        if(!entry.is_directory()){
          std::array<std::string_view,2> tmp_arr = {Utility::AnsiColor::bright_yellow1,path_lst.at(idx)};
          path_lst.at(idx) = Utility::AnsiColor::colorize(std::span(tmp_arr.data(),tmp_arr.size()),
                                                          Utility::AnsiColor::colorsel_e::none,
                                                          Utility::AnsiColor::modify_e::bold);
        }else{
          std::array<std::string_view,2> tmp_arr = {Utility::AnsiColor::mint_green,path_lst.at(idx)};
          path_lst.at(idx) = Utility::AnsiColor::colorize(std::span(tmp_arr.data(),tmp_arr.size()),Utility::AnsiColor::colorsel_e::none);
        }

        ++idx;
      }
    } //scope

  } //scope
  return path_lst;
}

//---

} //namespace

void File_Lst::Format::print_informative_list(std::set<fs::directory_entry> const file_lst,
                                                   size_t const term_width,
                                                   size_t const maxpath){
  auto const line = Utility::Strings::Smply("─",term_width);
  auto const vdelim = Utility::AnsiColor::colorize(" | ",Utility::AnsiColor::colorsel_e::grey);

  std::vector<std::thread> threads;

  using str_lst_T = std::vector<std::string>;

  str_lst_T permissions_lst;
  {
    auto const pm_fct = [&file_lst,&permissions_lst](){
      permissions_lst = get_permissions_lst(file_lst);
    };
    threads.emplace_back(pm_fct);
  }

  str_lst_T what_entry_lst;
  {
    auto const we_fct = [&file_lst,&what_entry_lst](){
      what_entry_lst = get_what_entry_lst(file_lst);
    };
    threads.emplace_back(we_fct);
  }

  str_lst_T file_size_lst;
  {
    auto const fs_fct = [&file_lst,&file_size_lst](){
      file_size_lst = get_file_size_lst(file_lst);
    };
    threads.emplace_back(fs_fct);
  }

  str_lst_T path_lst;
  {
    auto const pl_fct = [&file_lst,&path_lst,maxpath](){
      path_lst = get_path_lst(file_lst,maxpath);
    };
    threads.emplace_back(pl_fct);
  }

  // Join all threads
  for (auto& t : threads) {
    t.join();
  }

  size_t idx_width = 1 + std::log10(file_lst.size());

  size_t idx = 0;
  for(auto const& entry:file_lst){

    if(entry.is_directory()){
      std::cout << line << '\n';
    }

    std::filesystem::file_time_type ftime = std::filesystem::last_write_time(entry);
    std::cout << Utility::AnsiColor::fggrey << "[" << std::setw(idx_width) << idx << "] " << Utility::AnsiColor::reset_all
              << path_lst.at(idx) << vdelim
              << file_size_lst.at(idx) << vdelim
              << what_entry_lst.at(idx) << vdelim
              << permissions_lst.at(idx) << vdelim
              << Utility::AnsiColor::colorize(std::format("{0:%F}T{0:%R},{0:%S}", ftime).substr(0,21),Utility::AnsiColor::colorsel_e::high_blue)
              << '\n';

    ++idx;
  }

  std::cout << file_lst.size() << " Entries found | acc. Size: " << file_size_lst.at(idx) << '\n';

  return;
}
