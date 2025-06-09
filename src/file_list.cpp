#include "file_list.h"

#include <algorithm>
#include <set>

void File_Lst::list_worker(fs::path const fs_target,
                           std::set<fs::directory_entry>& file_lst,
                           std::vector<std::function<bool(fs::directory_entry)>> const workers,
                           bool const recursive){
  if(recursive){
    std::ranges::for_each(
      std::filesystem::recursive_directory_iterator{fs_target},
      [&file_lst,&workers](const auto& dir_entry){
        //std::cout << dir_entry << '\n';

        bool acceptance = true;

        for(auto const& worker:workers){
          if(worker){
            acceptance &= worker(dir_entry);
          }
        }
        if(acceptance){
          file_lst.insert(dir_entry);
        }
        return;
      });
  }else{
    std::ranges::for_each(
      std::filesystem::directory_iterator{fs_target},
      [&file_lst,&workers](const auto& dir_entry){
        //std::cout << dir_entry << '\n';

        bool acceptance = true;

        for(auto const& worker:workers){
          if(worker){
            acceptance &= worker(dir_entry);
          }
        }
        if(acceptance){
          file_lst.insert(dir_entry);
        }
        return;
      });
  }
return;
}
