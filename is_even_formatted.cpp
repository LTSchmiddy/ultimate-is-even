#include <climits>
#include <fstream>
#include <iostream>
#include <stdlib.h>
using namespace std;
const char *fname = "evens.bin";
int32_t imin = INT_MIN;
int32_t imax = INT_MAX - 1; /*seems to cause an underflow w/o the minus 1*/
fstream *get_even_file() {
  fstream *retVal = new fstream(fname, ios::binary | ios::in);
  if (retVal->is_open()) {
    return retVal;
  } else {
    cout
        << "Needs to generate even integer list file. This will require approx "
        << ((imax / 1000) - (imin / 1000)) * 2 << "kb on disk...\n";
    retVal->open(fname, ios::binary | ios::out);
    for (int32_t i = imin; i < imax; i += 2) {
      retVal->write(reinterpret_cast<const char *>(&i), sizeof(i));
    }
    retVal->close();
    retVal->open(fname, ios::binary | ios::in);
  }
  return retVal;
}
bool binary_search_evens(fstream *evens, int32_t num, uint64_t low,
                         uint64_t high, bool debug = false) {
  uint64_t midpoint = ((high - low) / 2) + low;
  evens->seekg(midpoint * 4, evens->beg);
  int32_t cp;
  evens->read(reinterpret_cast<char *>(&cp), sizeof(cp));
  if (debug) {
    cout << "number: " << num << " current number: " << cp << "\n"
         << "low position: " << low << " high position: " << high << "\n\n";
  }
  if (num == cp) {
    return true;
  } else if (high - 1 == low) {
    return false;
  } else {
    if (num > cp) {
      low = midpoint;
    } else {
      high = midpoint;
    }
    return binary_search_evens(evens, num, low, high, debug);
  }
}
int main(int argc, char *argv[]) {
  fstream *evens = get_even_file();
  evens->seekg(0, evens->end);
  uint64_t size = evens->tellg();
  cout << (binary_search_evens(get_even_file(), atoi(argv[1]), 0, size / 4,
                               (argc > 2))
               ? "true"
               : "false")
       << "\n";
}