#include "../include/ecooling.h"
#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstring>
#include "../include/constants.h"
#include "../include/cooler.h"
#include "../include/force.h"
#include "../include/functions.h"
#include "../include/ring.h"

#include <fstream>

void ECoolRate::electron_density(Ions& ion_sample, EBeam &ebeam) {
    int n_sample = ion_sample.n_sample();
    vector<double>& x = ion_sample.cdnt(Phase::X);
    vector<double>& y = ion_sample.cdnt(Phase::Y);
    vector<double>& ds = ion_sample.cdnt(Phase::DS);
    if(p_shift_) {
        double cx, cy, cz;
        ion_sample.center(cx, cy, cz);
        ebeam.density(x, y, ds, ne, n_sample, cx, cy, cz);

    }
    else {
        ebeam.density(x, y, ds, ne, n_sample);
    }
}

void ECoolRate::space_to_dynamic(int n_sample, Beam &ion, Ions &ion_sample) {
    double v = ion.beta()*k_c;
    vector<double>& xp = ion_sample.cdnt(Phase::XP);
    vector<double>& yp = ion_sample.cdnt(Phase::YP);
    vector<double>&dp_p = ion_sample.cdnt(Phase::DP_P);
    for(int i=0; i<n_sample; ++i) {
//        v_long[i] = dp_p[i]*v/(ion.gamma()*ion.gamma());  //Convert from dp/p to dv/v
//        v_long[i] /= (1-(v_long[i]+v)*ion.beta()/k_c);    //Convert to beam frame, when v_long<<v, canceled with the above line.
        v_long[i] = dp_p[i]*v;
        v_tr[i] = sqrt(xp[i]*xp[i]+yp[i]*yp[i])*v;
    }
}

void ECoolRate::init_scratch(int n_sample) {
    ne.resize(n_sample);
    xp_bet.resize(n_sample);
    yp_bet.resize(n_sample);
    x_bet.resize(n_sample);
    y_bet.resize(n_sample);
    xp.resize(n_sample);
    yp.resize(n_sample);
    x.resize(n_sample);
    y.resize(n_sample);
    dp_p.resize(n_sample);
    v_tr.resize(n_sample);
    v_long.resize(n_sample);
    force_x.resize(n_sample);
    force_y.resize(n_sample);
    force_z.resize(n_sample);
    scratch_size = n_sample;
}

void ECoolRate::beam_frame(int n_sample, double gamma_e) {
    double gamma_e_inv = 1/gamma_e;
    for(int i=0; i<n_sample; ++i){
        v_tr[i] *= gamma_e;
        ne[i] *= gamma_e_inv;
    }
    t_cooler_ /= gamma_e;
}

//Calculate friction force
void ECoolRate::force(int n_sample, Beam &ion, EBeam &ebeam, Cooler &cooler, FrictionForceSolver &force_solver) {
    //set parameters for friction force calculation
    force_solver.set_mag_field(cooler.magnetic_field());
    force_solver.set_time_cooler(t_cooler_);
    force_solver.friction_force(ion.charge_number(), n_sample, v_tr, v_long, ne, ebeam, force_x, force_z);
}

void ECoolRate::bunched_to_coasting(Beam &ion, Ions& ion_sample, EBeam &ebeam, Cooler &cooler,
                        FrictionForceSolver &force_solver){
    int n_sample = ion_sample.n_sample();
    int count = 1;
    vector<double> force_tr_rcd(n_sample);
    vector<double> force_long_rcd(n_sample);

    double cz_rcd = ion.center(2);

    p_shift_ = true;
    int n_long = n_long_sample_;
    double length = ebeam.length();
    double step = length/n_long;
    double gamma_e_inv = 1/ebeam.gamma();
    for(double cz = cz_rcd-0.5*length; cz <= cz_rcd+0.5*length; cz += step) {
        ion.set_center(2,cz);
        electron_density(ion_sample, ebeam);
        for(int i=0; i<n_sample; ++i) ne[i] *= gamma_e_inv;
        force(n_sample, ion, ebeam, cooler, force_solver);
        for(int i=0; i<n_sample; ++i) {
            force_tr_rcd[i] += force_x[i];
            force_long_rcd[i] += force_z[i];
        }
        ++count;
    }

    ion.set_center(2, cz_rcd);
    p_shift_ = false;
    double count_inv = 1.0/static_cast<double>(count);
    for(int i=0; i<n_sample; ++i) {
        force_x[i] = force_tr_rcd[i]*count_inv;
        force_z[i] = force_long_rcd[i]*count_inv;
    }
}

void ECoolRate::lab_frame(int n_sample, double gamma_e) {
    double gamma_e_inv = 1/gamma_e;
    for(unsigned int i=0; i<n_sample; ++i){
            force_x[i] *= gamma_e_inv;
            v_tr[i] *= gamma_e_inv;
    }
    t_cooler_ *= gamma_e;
}

//Distribute to x and y direction
void ECoolRate::force_distribute(int n_sample, Beam &ion, Ions &ion_sample) {
    double v0 = ion.beta()*k_c;
    vector<double>& xp = ion_sample.cdnt(Phase::XP);
    vector<double>& yp = ion_sample.cdnt(Phase::YP);
    for(int i=0; i<n_sample; ++i){
        force_y[i] = yp[i]!=0?force_x[i]*yp[i]*v0/v_tr[i]:0;
        force_x[i] = xp[i]!=0?force_x[i]*xp[i]*v0/v_tr[i]:0;
    }
}

void ECoolRate::apply_kick(int n_sample, Beam &ion, Ions& ion_sample) {
    double p0 = ion.p0_SI();
    vector<double>& ixp = ion_sample.cdnt(Phase::XP);
    vector<double>& iyp = ion_sample.cdnt(Phase::YP);
    vector<double>& idp_p = ion_sample.cdnt(Phase::DP_P);
    for(int i=0; i<n_sample; ++i){
        xp[i] = !iszero(ixp[i])?ixp[i]*exp(force_x[i]*t_cooler_/(p0*ixp[i])):ixp[i];
        yp[i] = !iszero(iyp[i])?iyp[i]*exp(force_y[i]*t_cooler_/(p0*iyp[i])):iyp[i];
        dp_p[i] = !iszero(idp_p[i])?idp_p[i]*exp(force_z[i]*t_cooler_/(p0*idp_p[i])):idp_p[i];
    }
}

void ECoolRate::adjust_rate(Beam &ion, EBeam &ebeam, initializer_list<double*> func) {
    if(ebeam.bunched()&&(!ion.bunched())) {
        double sample_length = ebeam.length();
        if(sample_length<0) perror("electron bunch length must be positive!");
        if(bunch_separate_>=sample_length) {
            double duty_factor = sample_length/bunch_separate_;
            for(auto& f: func) *f *= duty_factor;
        }
        else {
            perror("Electron bunch length is larger than the distance between electron bunches");
        }
    }
}

void ECoolRate::ecool_rate(FrictionForceSolver &force_solver, Beam &ion,
                Ions &ion_sample, Cooler &cooler, EBeam &ebeam,
                Ring &ring, double &rate_x, double &rate_y, double &rate_s) {

    int n_sample = ion_sample.n_sample();
    if(n_sample>scratch_size) init_scratch(n_sample);

    electron_density(ion_sample,ebeam);
    space_to_dynamic(n_sample, ion, ion_sample);

    //Time through the cooler
    t_cooler_ = cooler.length()/(ion.beta()*k_c);
    //Transfer into e- beam frame
    beam_frame(n_sample, ebeam.gamma());
    //Calculate friction force
    force(n_sample, ion, ebeam, cooler, force_solver);
//    //Restore the longitudinal velocity if it has been changed due to electron velocity gradient
//    restore_velocity(n_sample, ebeam);

    //Special treatment for bunched electron beam to cool coasting ion beam
    if(!ion.bunched()&&ebeam.bunched()) {
        bunched_to_coasting(ion, ion_sample, ebeam, cooler, force_solver);
        force(n_sample, ion, ebeam, cooler, force_solver);
    }

    //Transfer back to lab frame
    lab_frame(n_sample, ebeam.gamma());

    //Distribute the friction force into x,y direction.
    force_distribute(n_sample,ion, ion_sample);
    //Original emittance
    double emit_x0, emit_y0, emit_z0;
    ion_sample.emit(emit_x0, emit_y0, emit_z0);

    //Apply kick
    apply_kick(n_sample, ion, ion_sample);

    //New emittance
    double emit_x, emit_y, emit_z;
    auto t = ion_sample.get_twiss();
    adjust_disp_inv(t.disp_x, x_bet, dp_p, ion_sample.cdnt(Phase::X), n_sample);
    adjust_disp_inv(t.disp_dx, xp_bet, dp_p, xp, n_sample);
    adjust_disp_inv(t.disp_y, y_bet, dp_p, ion_sample.cdnt(Phase::Y), n_sample);
    adjust_disp_inv(t.disp_dy, yp_bet, dp_p, yp, n_sample);

    ion_sample.emit(x_bet, xp_bet, y_bet, yp_bet, dp_p, emit_x, emit_y, emit_z);

    rate_x = emit_x/emit_x0-1;
    rate_y = emit_y/emit_y0-1;
    rate_s = emit_z/emit_z0-1;
    double freq = k_c*ion.beta()/ring.circ()*cooler.section_number();
    rate_x *= freq;
    rate_y *= freq;
    rate_s *= freq;

    adjust_rate(ion, ebeam, {&rate_x, &rate_y, &rate_s});

}

vector<double>& ECoolRate::scratch(ECoolRateScratch s) {
    switch(s) {
        case ECoolRateScratch::XP_BET: {
            return xp_bet;
        }
        case ECoolRateScratch::YP_BET: {
            return yp_bet;
        }
        case ECoolRateScratch::XP: {
            return xp;
        }
        case ECoolRateScratch::DP_P: {
            return dp_p;
        }
        case ECoolRateScratch::V_TR: {
            return v_tr;
        }
        case ECoolRateScratch::V_LONG: {
            return v_long;
        }
        case ECoolRateScratch::YP: {
            return yp;
        }
        case ECoolRateScratch::FORCE_X: {
            return force_x;
        }
        case ECoolRateScratch::FORCE_Y: {
            return force_y;
        }
        case ECoolRateScratch::FORCE_Z: {
            return force_z;
        }
        default: {
            assert(false&&"Wrong scratch variable selected in ECoolRate!");
            return xp;
        }
    }
}
