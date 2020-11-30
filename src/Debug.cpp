
#include <OsqpEigen/Debug.hpp>

// Taken from https://stackoverflow.com/a/8243866/2702753
class NullStream : public std::ostream {
public:
  NullStream() : std::ostream(nullptr) {}
  NullStream(const NullStream &) : std::ostream(nullptr) {}
};

template <class T>
const NullStream &operator<<(NullStream &&os, const T &value) {
  return os;
}

namespace OsqpEigen {
#ifdef DEBUG_OUTPUT
	std::ostream &debugStream = std::cerr;
#else
	NullStream theStream;
	std::ostream &debugStream = theStream;
#endif
}
