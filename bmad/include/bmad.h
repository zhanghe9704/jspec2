#ifndef BMAD_H
#define BMAD_H

#include "beam.h"
#include "ions.h"
#include "ring.h"

class Ions_BMAD: public Ions{
private:
//    double emit(vector<double>& x, vector<double>&xp, int n);
//    double emit_p(vector<double>& dp_p, int n);
public:
    Ions_BMAD(int n);
};

void load_bmad_coord();

#endif // BMAD_H
