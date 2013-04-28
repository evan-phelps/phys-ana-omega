#include "h10t3pi_sel.h"

class SelRenameMe : public h10t3pi_sel {
 public:
 SelRenameMe() : h10t3pi_sel() {
    printf("in SelEc::SelEc()\n");
  }
  ~SelRenameMe() {
    printf("in SelEc::SelEc()\n");
  }
  void Setup() {
    printf("in SelEc::Setup()\n");
  }
  void Process() {
    printf("in SelEc::Process()\n");
  }
  void Finalize() {
    printf("in SelEc::Finalize()\n");
  }
};
