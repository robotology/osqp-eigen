#include <osqp_api_functions.h>

int main()
{
  // This function is only available in OSQP >= 1.0.0
  OSQPCscMatrix_set_data(nullptr, 0, 0, 0, nullptr, 0, 0);
  return 0;
}
