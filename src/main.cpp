#include <iostream>
#include <array>

#include "dtypes.h"
#include "file_fkt.h"

using namespace std;

int main(){

  std::cout << __DATE__ << "T" << __TIME__ << "\n";

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

  return 0;
}
