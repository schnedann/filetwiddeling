#include <iostream>
#include <iomanip>
#include <array>
#include <string>
#include <cstdlib>
#include <vector>
#include <set>
#include <ranges>
#include <cmath>

#include "dtypes.h"
#include "file_fkt.h"

#include <cxxopts.hpp>

using namespace std;

int main(int argc, char* argv[]){

  std::cout << __DATE__ << "T" << __TIME__ << "\n";

  //---

  enum class main_mode_e:u8{
    help = 0,
    list_dir,
    copy,
    playground
  };
  using main_mode_T = enum main_mode_e;

  main_mode_T mmode = main_mode_e::help;

  //---

  bool verbose     = false;
  bool recursive   = false;
  bool information = false;

  std::string target;
  std::string source;
  std::vector<std::string> uro;

  //---

  cxxopts::Options options("filetwiddeling", "Collecion of File handling functions");
  {

    options.add_options()
      ("ls,list_dir","List Directory", cxxopts::value<bool>()->default_value("false"))
      ("playground", "Test Code", cxxopts::value<bool>()) // a bool parameter
      ("cp,copy","copy File or Directory", cxxopts::value<bool>()->default_value("false"))
      ("i,information","decorate output with information", cxxopts::value<bool>()->default_value("false"))
      ("r,recursive","goto subdirectories also", cxxopts::value<bool>()->default_value("false"))
      ("s,source", "Input Source File or Directory", cxxopts::value<std::string>())
      ("t,target", "Modification Target File or Directory", cxxopts::value<std::string>())
      ("h,help","Display Help", cxxopts::value<bool>()->default_value("false"))
      ("v,verbose", "Verbose output", cxxopts::value<bool>()->default_value("false"))
      /*
       * ("i,integer", "Int param", cxxopts::value<int>())
       */
    ;
    options.allow_unrecognised_options();

    auto result = options.parse(argc, argv);

    if(0<result.count("help")){
      if(result["help"].as<bool>()){
        mmode = main_mode_e::help;
      }
    }

    if(0<result.count("playground")){
      if(result["playground"].as<bool>()){
        mmode = main_mode_e::playground;
      }
    }

    if(0<result.count("list_dir")){
      if(result["list_dir"].as<bool>()){
        mmode = main_mode_e::list_dir;
      }
    }

    if((0<result.count("cp")) ||
       (0<result.count("copy"))){
      if(result["copy"].as<bool>()){
        mmode = main_mode_e::copy;
      }
    }

    if((0<result.count("i")) ||
       (0<result.count("information")) ){
      information = result["information"].as<bool>();
    }

    if((0<result.count("r")) ||
       (0<result.count("recursive")) ){
      recursive = result["recursive"].as<bool>();
    }

    if((0<result.count("s")) ||
       (0<result.count("source")) ){
      source = result["source"].as<std::string>();
    }

    if((0<result.count("t")) ||
       (0<result.count("target")) ){
      target = result["target"].as<std::string>();
    }

    if((0<result.count("v")) ||
       (0<result.count("verbose")) ){
      verbose = result["verbose"].as<bool>();
    }

    uro = result.unmatched();
  }

  if(verbose){
    for(int ii=0; ii<argc; ++ii){
      std::cout << "[" << ii << "]: " << argv[ii] << "\n";
    }
  }

  switch (mmode) {
  case main_mode_e::list_dir:{
    std::cout << "List Directory" << "\n";
    fs::path fs_target = fs::current_path();
    if(!target.empty()){
      auto const tmp = fs::path(target);

      if(tmp.has_extension()){
        std::cerr << "Target is a Filename" << "\n";
        std::cerr << tmp << "\n";
        exit(EXIT_FAILURE);
      }

      if(!fs::exists(tmp)){
        std::cerr << "Target does not exist in filesystem" << "\n";
        std::cerr << tmp << "\n";
        exit(EXIT_FAILURE);
      }

      fs_target = tmp;
    }else{
      if(0<uro.size()){
        std::cout << "testing first unrecognized argument as target" << "\n";
        auto const tmp = fs::path(uro.at(0));

        if(tmp.has_extension()){
          std::cerr << "Target is a Filename" << "\n";
          std::cerr << tmp << "\n";
          exit(EXIT_FAILURE);
        }

        if(!fs::exists(tmp)){
          std::cerr << "Target does not exist in filesystem" << "\n";
          std::cerr << tmp << "\n";
          exit(EXIT_FAILURE);
        }

        fs_target = tmp;
      }
    }
    std::cout << "Target: " << fs_target << "\n";

    std::set<fs::directory_entry> file_lst;

    if(recursive){
      std::ranges::for_each(
        std::filesystem::recursive_directory_iterator{fs_target},
        [&file_lst](const auto& dir_entry){
          //std::cout << dir_entry << '\n';
          file_lst.insert(dir_entry);
          return;
        });
    }else{
      std::ranges::for_each(
        std::filesystem::directory_iterator{fs_target},
        [&file_lst](const auto& dir_entry){
          //std::cout << dir_entry << '\n';
          file_lst.insert(dir_entry);
          return;
        });
    }

    //Informative File List
    if(information){

      size_t path_maxl = 0;
      //longest path
      for(auto const& entry:file_lst){
        size_t tmp = entry.path().string().size();
        if(path_maxl < tmp){
          path_maxl = tmp;
        }
      }

      auto str_extend = [](std::string& str, size_t target_length){
        size_t pos = str.size();
        str.resize(target_length);
        for(;pos<target_length;++pos){
          str.at(pos) = ' ';
        }
        return;
      };

      std::vector<std::string> what_entry_lst(file_lst.size());
      {
        size_t idx = 0;
        for(auto const& entry:file_lst){

          if(entry.is_block_file()){
            what_entry_lst.at(idx) = "BlockFile";
          }
          if(entry.is_character_file()){
            what_entry_lst.at(idx) = "CharacterFile";
          }
          if(entry.is_regular_file()){
            what_entry_lst.at(idx) = "File";
          }

          if(entry.is_directory()){
            what_entry_lst.at(idx) = "Directory";
          }
          if(entry.is_fifo()){
            what_entry_lst.at(idx) = "FiFo";
          }
          if(entry.is_socket()){
            what_entry_lst.at(idx) = "Socket";
          }

          if(entry.is_symlink()){
            what_entry_lst.at(idx) = "Symlink";
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
      }

      std::vector<std::string> file_size_lst(file_lst.size());
      {
        size_t idx = 0;
        for(auto const& entry:file_lst){

          if(!entry.is_directory() && !entry.is_socket()){

            auto fsize = entry.file_size();
            size_t ii=0;
            while(1024 <= fsize){
              fsize /= 1024;
              ++ii;
            }
            std::string file_size = std::to_string(fsize);
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
              file_size = std::to_string(entry.file_size()) + " Byte";
              break;
            }

            file_size_lst.at(idx) = file_size;
          }

          ++idx;
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
      }

      {
        size_t idx_width = 1 + std::log10(file_lst.size());

        size_t idx = 0;
        for(auto const& entry:file_lst){
          auto path = entry.path().string();
          str_extend(path,path_maxl);
          std::cout << "[" << std::setw(idx_width) << idx << "]  " << path << "| " << file_size_lst.at(idx) << "| " << what_entry_lst.at(idx) << "| " << '\n';

          ++idx;
        }
      }

      std::cout << file_lst.size() << " Entries found" << '\n';

    //Basic File List
    }else{
      for(auto const& entry:file_lst){
        std::cout << entry.path() << '\n';
      }
    }

    break;
  }
  case main_mode_e::copy:{

    break;
  }
  case main_mode_e::playground:{
    array<u8,12> buff{};

    bool err = File_Fkt::read_from_File<12>("dummy.txt",buff);

    std::cout << ((err)?("Error"):("no Error")) << "\n";
    std::cout << "via Buffer: " << std::string(buff.begin(),buff.end()) << endl;

    std::cout << "via String: " << File_Fkt::read_File2String("dummy.txt") << "\n";

    auto fsize = File_Fkt::getBlockSize("dummy.txt");
    if(!fsize.first){
      std::cout << "Blocksize on Filesystem: " << fsize.second << "\n";
    }

    File_Fkt::copy_File("dummy.txt","dummy.txt~");

    File_Fkt::rename_File("dummy.txt~","dummy.bak");

    auto rd_data = File_Fkt::read_block("dummy.bak", 0, 5);
    if(!rd_data.first){
      char* bgn = reinterpret_cast<char*>(rd_data.second.first.get());
      char* end = &reinterpret_cast<char*>(rd_data.second.first.get())[rd_data.second.second];
      std::cout << std::string(bgn,end) << "\n";
    }
    break;
  }
  default:
    std::cout << options.help() << std::endl;
    exit(EXIT_SUCCESS);
    break;
  }

  return EXIT_SUCCESS;
}
