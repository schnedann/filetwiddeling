#include <iostream>
#include <array>
#include <string>
#include <cstdlib>
#include <vector>
#include <set>
#include <regex>
#include <span>

#include "dtypes.h"
#include "file_fkt.h"
#include "file_list.h"
#include "file_list_format.h"
#include "evaluate_args.h"
#include "terminal.h"
#include "Stringhelper.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wall" // Ignore all warnings
#include <cxxopts.hpp>
#pragma GCC diagnostic pop

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
      ("fs,filtersize","Filter for Size - only Opbjects in Range are processed", cxxopts::value<std::string>()->no_implicit_value())
      ("fn,filtername","Filter for Name - only matching Opbjects are processed", cxxopts::value<std::string>()->no_implicit_value())
      ("i,information","decorate output with information", cxxopts::value<bool>()->default_value("false"))
      ("r,recursive","goto subdirectories also", cxxopts::value<bool>()->default_value("false"))
      ("s,source", "Input Source File or Directory", cxxopts::value<std::string>())
      ("t,target", "Target File or Directory to work on (including Modifications)", cxxopts::value<std::string>())
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

    //---

    enum class sizefilter_mode_e:u8{
      none = 0,
      range,
      greater_or_equal,
      smaller_or_equal
    };
    using sizefilter_mode_T = enum sizefilter_mode_e;

    sizefilter_mode_T sfm = sizefilter_mode_e::greater_or_equal;
    size_t sf_para_1 = 100*1024;
    size_t sf_para_2 = 0;

    //---

    std::string name_regex;

    //---

    File_Lst::list_worker(fs_target,
                          file_lst,
                          {
                          //Dummy Worker (at least one must exist!)
                          [](fs::directory_entry const de)->bool{
                            return true;},

                          //File-Size Worker
                          [sfm,sf_para_1,sf_para_2](fs::directory_entry const de)->bool{
                            bool acceptance = false;

                            if(de.is_regular_file()){
                              size_t file_size = fs::file_size(de);
                              switch (sfm) {
                              case sizefilter_mode_e::range:{
                                acceptance = (file_size >= sf_para_1) && (sf_para_1 <= sf_para_2);
                                break;
                              }
                              case sizefilter_mode_e::greater_or_equal:{
                                acceptance = (file_size >= sf_para_1);
                                break;
                              }
                              case sizefilter_mode_e::smaller_or_equal:{
                                acceptance = (file_size <= sf_para_1);
                                break;
                              }
                              default:
                                acceptance = true;
                                break;
                              }
                            }
                            return acceptance;},

                          //File-Name Worker
                          [name_regex](fs::directory_entry const de)->bool{
                            bool acceptance = true;
                            if(!name_regex.empty()){
                              std::regex filter(name_regex);

                              acceptance = std::regex_match(de.path().string(),filter);
                            }
                            return acceptance;}
                          },recursive);

    //Informative File List
    if(information){
      File_Lst::Format::print_informative_list(file_lst,twidth,maxpath);
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

    static constexpr std::string_view const welcome = "Welcome to the Filetwiddleing Tool";
    auto const line = Utility::Strings::Smply("─",welcome.size());

    std::cout << line << "\n";
    std::cout << welcome.data() << "\n";
    std::cout << line << "\n";

    std::cout << "Cmd-Args:" << "\n";
    size_t idx=0;
    auto const argspan = std::span(argv,argc);
    for(auto cstr:argspan){
      std::cout << "[" << idx << "]: " << cstr << "\n";
    }

    std::cout << line << "\n";

    std::cout << options.help() << std::endl;
    exit(EXIT_SUCCESS);
    break;
  }

  return EXIT_SUCCESS;
}
