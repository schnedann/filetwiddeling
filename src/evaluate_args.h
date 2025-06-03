#ifndef EVALUATE_ARGS_H
#define EVALUATE_ARGS_H

#include "dtypes.h"
#include "file_fkt.h"

namespace EvalArgs {

bool is_file_n_exists(fs::path d_n_e);
bool is_directory_n_exists(fs::path d_n_e);

} //namespace

#endif // EVALUATE_ARGS_H
