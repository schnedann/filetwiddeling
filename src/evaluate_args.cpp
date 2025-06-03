#include "evaluate_args.h"

#include <iostream>
#include <cstdlib>

#include "dtypes.h"
#include "file_fkt.h"


bool EvalArgs::is_file_n_exists(fs::path d_n_e){
  if(!d_n_e.has_stem()){
    std::cerr << "is Not a Filename by Syntax" << "\n";
    std::cerr << d_n_e << "\n";
    exit(EXIT_FAILURE);
  }

  if(!fs::exists(d_n_e)){
    std::cerr << "does not exist in filesystem" << "\n";
    std::cerr << d_n_e << "\n";
    exit(EXIT_FAILURE);
  }
  return true;
}

bool EvalArgs::is_directory_n_exists(fs::path d_n_e){
  if(d_n_e.has_extension()){
    std::cerr << "is a Not a Directory by Syntax" << "\n";
    std::cerr << d_n_e << "\n";
    exit(EXIT_FAILURE);
  }

  if(!fs::exists(d_n_e)){
    std::cerr << "does not exist in filesystem" << "\n";
    std::cerr << d_n_e << "\n";
    exit(EXIT_FAILURE);
  }
  return true;
}
