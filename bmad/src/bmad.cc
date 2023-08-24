#include "bmad.h"

Ions_BMAD::Ions_BMAD(int n_sample){
    n_=n_sample;
    x_bet.resize(n_sample,0);
    y_bet.resize(n_sample,0);
    xp_bet.resize(n_sample,0);
    yp_bet.resize(n_sample,0);
    ds.resize(n_sample,0);
    dp_p.resize(n_sample,0);
    x.resize(n_sample,0);
    y.resize(n_sample,0);
    xp.resize(n_sample,0);
    yp.resize(n_sample,0);
}
