#include "Parse_submission.h"
PragmaEntry::PragmaEntry(double _price,
                         double _amount,
                         std::string _timestamp,
                         std::string _product,
                         PragmaType _PragmaType)
    : price(_price),
      amount(_amount),
      timestamp(_timestamp),
      product(_product),
      PragmaType(PragmaType)
{
}