#include <iostream>
#include <array>

#include "dtypes.h"
#include "file_fkt.h"

using namespace std;

int main()
{
  array<u8,12> buff{};
  cout << File_Fkt::read_from_File<12>("dummy.txt",buff) << "\n";
  cout << std::string(buff.begin(),buff.end()) << endl;

  cout << File_Fkt::read_File2String("dummy.txt") << "\n";

  auto fsize = File_Fkt::getBlockSize("dummy.txt");
  if(!fsize.first){
    cout << fsize.second << "\n";
  }

  File_Fkt::copy_File("dummy.txt","dummy.txt~");

  File_Fkt::rename_File("dummy.txt~","dummy.bak");

  auto rd_data = File_Fkt::read_block("dummy.bak", 0, 5);
  if(!rd_data.first){
    char* bgn = reinterpret_cast<char*>(rd_data.second.first.get());
    char* end = &reinterpret_cast<char*>(rd_data.second.first.get())[rd_data.second.second];
    cout << std::string(bgn,end) << "\n";
  }

  return 0;
}
