#include <iostream>
#include <iomanip>
#include <array>
#include <string>
#include <sstream>
#include <cstdlib>
#include <vector>
#include <set>
#include <ranges>
#include <cmath>
#include <format>
#include <chrono>
#include <thread>
#include <span>

#include "dtypes.h"
#include "file_fkt.h"
#include "file_list_format.h"
#include "evaluate_args.h"
#include "Stringhelper.h"
#include "ansiconsolecolor.h"
#include "terminal.h"

#include <cxxopts.hpp>

using namespace std;

int main(int argc, char* argv[]){

  //std::cout << __DATE__ << "T" << __TIME__ << "\n";

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
      ("ls,list_dir","List Directory - plain list or add -i to get a nice colored and formated output", cxxopts::value<bool>()->default_value("false"))
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

    auto const twidth = Terminal::size().first;
    auto const maxpath = twidth >> 1;

    std::cout << "List Directory" << "\n";
    fs::path fs_target = fs::current_path();
    if(!target.empty()){
      fs_target = fs::path(target);

      EvalArgs::is_directory_n_exists(fs_target);

    }else{
      if(0<uro.size()){
        std::cout << "testing first unrecognized argument as target" << "\n";
        fs_target = fs::path(uro.at(0));

        EvalArgs::is_directory_n_exists(fs_target);

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
      { //Formated output

        auto const line = Utility::Strings::Smply("─",twidth);
        auto const vdelim = Utility::AnsiColor::colorize(" | ",Utility::AnsiColor::colorsel_e::grey);

        std::vector<std::thread> threads;

        using str_lst_T = std::vector<std::string>;

        str_lst_T permissions_lst;
        {
          auto const pm_fct = [&file_lst,&permissions_lst](){
            permissions_lst = File_Fkt::List_Format::get_permissions_lst(file_lst);
          };
          threads.emplace_back(pm_fct);
        }

        str_lst_T what_entry_lst;
        {
          auto const we_fct = [&file_lst,&what_entry_lst](){
            what_entry_lst = File_Fkt::List_Format::get_what_entry_lst(file_lst);
          };
          threads.emplace_back(we_fct);
        }

        str_lst_T file_size_lst;
        {
          auto const fs_fct = [&file_lst,&file_size_lst](){
            file_size_lst = File_Fkt::List_Format::get_file_size_lst(file_lst);
          };
          threads.emplace_back(fs_fct);
        }

        str_lst_T path_lst;
        {
          auto const pl_fct = [&file_lst,&path_lst,maxpath](){
            path_lst = File_Fkt::List_Format::get_path_lst(file_lst,maxpath);
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
      } //scope
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

    std::cout << "Welcome to the Filetwiddleing Tool" << "\n";

    size_t idx=0;
    auto const argspan = std::span(argv,argc);
    for(auto cstr:argspan){
      std::cout << "[" << idx << "]: " << cstr << "\n";
    }

    std::cout << options.help() << std::endl;
    exit(EXIT_SUCCESS);
    break;
  }

  return EXIT_SUCCESS;
}
