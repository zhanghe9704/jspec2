#include "ecooling.h"
#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstring>



#ifdef _OPENMP
    #include <omp.h>
#endif // _OPENMP
#include "constants.h"
#include "cooler.h"
#include "force.h"
#include "functions.h"
#include "ring.h"

#include <fstream>

void ECoolRate::electron_density(Ions& ion_sample, EBeam &ebeam) {
    int n_sample = ion_sample.n_sample();
//    vector<double>& x, y;

    vector<double>& x = n_piece==0?ion_sample.cdnt(Phase::X):this->x;
    vector<double>& y = n_piece==0?ion_sample.cdnt(Phase::Y):this->y;
    vector<double>& ds = ion_sample.cdnt(Phase::DS);

    if(ebeam.p_shift()) {
        double cx, cy, cz;
        ion_sample.center(cx, cy, cz);
        if(ebeam.multi_bunches()) ebeam.multi_density(x, y, ds, ne, n_sample, cx, cy, cz);
        else ebeam.density(x, y, ds, ne, n_sample, cx, cy, cz);

    }
    else {
        if(ebeam.multi_bunches()) ebeam.multi_density(x, y, ds, ne, n_sample);
        else ebeam.density(x, y, ds, ne, n_sample);
    }
}

void ECoolRate::space_to_dynamic(int n_sample, Beam &ion, Ions &ion_sample) {
    double v = ion.beta()*k_c;
    vector<double>& xp = n_piece==0?ion_sample.cdnt(Phase::XP):this->xp;
    vector<double>& yp =  n_piece==0?ion_sample.cdnt(Phase::YP):this->yp;
    vector<double>& dp_p =  n_piece==0?ion_sample.cdnt(Phase::DP_P):this->dp_p;

    for(int i=0; i<n_sample; ++i) {
//        v_long[i] = dp_p[i]*v/(ion.gamma()*ion.gamma());  //Convert from dp/p to dv/v
//        v_long[i] /= (1-(v_long[i]+v)*ion.beta()/k_c);    //Convert to beam frame, when v_long<<v, canceled with the above line.
        v_long[i] = dp_p[i]*v;
    }
    if(symmetry_vtr) {
        for(int i=0; i<n_sample; ++i)
            v_tr[i] = sqrt(xp[i]*xp[i]+yp[i]*yp[i])*v;
    }
    else {
        for(int i=0; i<n_sample; ++i){
            v_tr[i] = xp[i]*v;
            v_y[i] = yp[i]*v;
        }
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
    if (!symmetry_vtr)
        v_y.resize(n_sample);   //Cooling with e- dispersion. Use v_tr for horizontal. Cylindrical symmetry broken.
}

void ECoolRate::beam_frame(int n_sample, double gamma_e) {
    double gamma_e_inv = 1/gamma_e;
    for(int i=0; i<n_sample; ++i){
        v_tr[i] *= gamma_e;
        ne[i] *= gamma_e_inv;
    }
    if (!symmetry_vtr)
        for(int i=0; i<n_sample; ++i)
            v_y[i] *= gamma_e;
    t_cooler_ /= gamma_e;
}

//Calculate friction force
void ECoolRate::force(int n_sample, Beam &ion, EBeam &ebeam, Cooler &cooler, FrictionForceSolver &force_solver) {
    //set parameters for friction force calculation
    force_solver.set_mag_field(cooler.magnetic_field());
    force_solver.set_time_cooler(t_cooler_);
    double beta_i = ion.beta();
    double beta_e = ebeam.beta();
    double d_beta = beta_e-beta_i;
    double cv_l = 0;
    if(!iszero(d_beta, 1e-6)) cv_l = d_beta*k_c;
    if(!iszero(ebeam.cv_l(), 1e-6)) cv_l += ebeam.cv_l();

    if(!iszero(cv_l, 1e-6)) {
        for(auto& v: v_long) {
            v -= cv_l;
        }
    }

    if(!symmetry_vtr) {
        if(ebeam.velocity() == Velocity::VARY) {
            double gamma_e = ebeam.gamma();
            vector<double>& v_avg_l = ebeam.get_v(EBeamV::V_AVG_L);
            vector<double>& v_avg_h = ebeam.get_v(EBeamV::V_AVG_X);
            vector<double>& v_avg_v = ebeam.get_v(EBeamV::V_AVG_Y);

            for(int i=0; i<n_sample; ++i){
                v_long.at(i) -= v_avg_l.at(i);
//                v_tr.at(i) -= v_avg_h.at(i)*gamma_e; //Transfer to beam frame.
//                v_y.at(i) -= v_avg_v.at(i)*gamma_e;
            }

            if(!iszero(ebeam.twiss().alf_x)){
                for(int i=0; i<n_sample; ++i)
                    v_tr.at(i) -= v_avg_h.at(i)*gamma_e; //Transfer to beam frame.
            }

            if(!iszero(ebeam.twiss().alf_y)){
                for(int i=0; i<n_sample; ++i)
                    v_y.at(i) -= v_avg_v.at(i)*gamma_e; //Transfer to beam frame.
            }
        }
        force_solver.friction_force(ion.charge_number(), n_sample, v_tr, v_y, v_long, ne, ebeam, cooler,
                                    force_x, force_y, force_z);
        return;
    }
    if(ebeam.velocity() == Velocity::VARY_Z) {
        vector<double>& v_avg_l = ebeam.get_v(EBeamV::V_AVG_L);
        for(int i=0; i<n_sample; ++i){
            v_long.at(i) -= v_avg_l.at(i);
        }
    }

    force_solver.friction_force(ion.charge_number(), n_sample, v_tr, v_long, ne, ebeam, force_x, force_z);

    if(dual_force_solver){
        force_solver_l->set_mag_field(cooler.magnetic_field());
        force_solver_l->set_time_cooler(t_cooler_);
        force_solver_l->friction_force(ion.charge_number(), n_sample, v_tr, v_long, ne, ebeam, force_y, force_z); //force_y will be ignored in the following.
    }

    if(save_force) {
        string filename = time_to_filename();
        filename = "Friction_force_on_ions_" + filename + ".sdds";
        std::ofstream of;
        of.open (filename);
        if(of.is_open()){
            std::cout<<"File opened: "<<filename<<" ."<<std::endl;
            save_force_sdds_head(of, n_sample);
            of.precision(10);
            of<<std::showpos;
            of<<std::scientific;
            for(int i=0; i<n_sample; ++i) {
                of<<ne.at(i)<<' '<<v_tr.at(i)<<' '<<v_long.at(i)<<' '<<force_x.at(i)<<' '<<force_z.at(i);
                of<<std::endl;
            }
            of.close();
        }
        else {
            std::cout<<"Failed to open file: "<<filename<<" !"<<std::endl;
        }
    }
}

void ECoolRate::bunched_to_coasting(Beam &ion, Ions& ion_sample, EBeam &ebeam, Cooler &cooler,
                        FrictionForceSolver &force_solver){
    int n_sample = ion_sample.n_sample();
    int count = 1;
    vector<double> force_tr_rcd(n_sample);
    vector<double> force_long_rcd(n_sample);

    double cz_rcd = ion.center(2);

    ebeam.set_p_shift(true);
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
    ebeam.set_p_shift(false);
    double count_inv = 1.0/static_cast<double>(count);
    for(int i=0; i<n_sample; ++i) {
        force_x[i] = force_tr_rcd[i]*count_inv;
        force_z[i] = force_long_rcd[i]*count_inv;
    }
}

void ECoolRate::lab_frame(int n_sample, double gamma_e) {
    double gamma_e_inv = 1/gamma_e;
    for(int i=0; i<n_sample; ++i){
            force_x[i] *= gamma_e_inv;
            v_tr[i] *= gamma_e_inv;
    }
    if(!symmetry_vtr) {
        for(int i=0; i<n_sample; ++i){
            force_y[i] *= gamma_e_inv;
            v_y[i] *= gamma_e_inv;
        }
    }
    t_cooler_ *= gamma_e;
}

//Distribute to x and y direction
void ECoolRate::force_distribute(int n_sample, Beam &ion, Ions &ion_sample) {
    if(!symmetry_vtr) return;
    double v0 = ion.beta()*k_c;
    vector<double>& xp = n_piece==0?ion_sample.cdnt(Phase::XP):this->xp;
    vector<double>& yp = n_piece==0?ion_sample.cdnt(Phase::YP):this->yp;

    #ifdef _OPENMP
        #pragma omp parallel for
    #endif // _OPENMP
    for(int i=0; i<n_sample; ++i){
        force_y[i] = yp[i]!=0?force_x[i]*yp[i]*v0/v_tr[i]:0;
        force_x[i] = xp[i]!=0?force_x[i]*xp[i]*v0/v_tr[i]:0;
    }
}

void ECoolRate::apply_kick(int n_sample, Beam &ion, Ions& ion_sample) {
    double p0 = ion.p0_SI();
    vector<double>& ixp = n_piece==0?ion_sample.cdnt(Phase::XP):xp;
    vector<double>& iyp = n_piece==0?ion_sample.cdnt(Phase::YP):yp;
    vector<double>& idp_p = n_piece==0?ion_sample.cdnt(Phase::DP_P):dp_p;

    if(n_piece>1) t_cooler_ /= n_piece;

    #ifdef _OPENMP
        #pragma omp parallel for
    #endif // _OPENMP
    for(int i=0; i<n_sample; ++i){
        xp[i] = !iszero(ixp[i])?ixp[i]*exp(force_x[i]*t_cooler_/(p0*ixp[i])):ixp[i];
        yp[i] = !iszero(iyp[i])?iyp[i]*exp(force_y[i]*t_cooler_/(p0*iyp[i])):iyp[i];
//        dp_p[i] = !iszero(idp_p[i])?idp_p[i]*exp(force_z[i]*t_cooler_/(p0*idp_p[i])):idp_p[i];
        double dp = force_z[i]*t_cooler_/(idp_p[i]*p0);
        if(!iszero(idp_p[i],1e-7)) {
            dp_p[i] = dp>0.15?idp_p[i]*(1+dp):idp_p[i]*exp(dp);
        }
        else {
            dp_p[i] = idp_p[i];
        }
    }

    if(n_piece>1) t_cooler_ *= n_piece;
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

void ECoolRate::ecool_ions(FrictionForceSolver &force_solver, Beam &ion, Ions &ion_sample, Cooler &cooler, EBeam &ebeam,
                Ring &ring){
    int n_sample = ion_sample.n_sample();
//    if(ebeam.disp() && ebeam.shape()!=Shape::GAUSSIAN_BUNCH_DISP) {
    if(ebeam.generate_samples()) {
        electron_density(ion_sample,*ebeam.samples);
    }
    else {
        electron_density(ion_sample,ebeam);
    }
    space_to_dynamic(n_sample, ion, ion_sample);

    if (cooling_count) {
        double ne_max = 0;
        for(double d:ne) {
            if (d>ne_max) ne_max = d;
        }
        double ne_tol = ne_max/100;
        for(int i=0; i<n_sample; ++i) {
            ion_sample.density_count(i, ne[i]);
            if (ne[i]>ne_tol) ion_sample.cooling_count(i);
        }
    }

    //Time through the cooler
    t_cooler_ = cooler.length()/(ion.beta()*k_c);

    //Transfer into e- beam frame
    beam_frame(n_sample, ebeam.gamma());

    //Calculate friction force
//    if(ebeam.disp() && ebeam.shape()!=Shape::GAUSSIAN_BUNCH_DISP) {
    if(ebeam.generate_samples()) {
//        ParticleBunch* ptr = dynamic_cast<ParticleBunch*>(ebeam.samples.get());
//        force(n_sample, ion, *ptr, cooler, force_solver);
//        auto p = ebeam.samples.get();

        force(n_sample, ion, *ebeam.samples, cooler, force_solver);
    }
    else {
        force(n_sample, ion, ebeam, cooler, force_solver);
    }
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

//    //Original emittance
//    double emit_x0, emit_y0, emit_z0;
//    ion_sample.emit(emit_x0, emit_y0, emit_z0);

    //Apply kick
    apply_kick(n_sample, ion, ion_sample);
}

void ECoolRate::ecool_rate(FrictionForceSolver &force_solver, Beam &ion,
                Ions &ion_sample, Cooler &cooler, EBeam &ebeam,
                Ring &ring, double &rate_x, double &rate_y, double &rate_s) {

    int n_sample = ion_sample.n_sample();
    symmetry_vtr = ebeam.symmetry_vtr();
    if(n_sample>scratch_size) init_scratch(n_sample);
    n_piece = cooler.piece_number();

//    if(ebeam.disp() && ebeam.shape()!=Shape::BLASKIEWICZ) {
//        electron_density(ion_sample,*ebeam.samples);
//    }
//    else {
//        electron_density(ion_sample,ebeam);
//    }
//    space_to_dynamic(n_sample, ion, ion_sample);
//
//    if (cooling_count) {
//        double ne_max = 0;
//        for(double d:ne) {
//            if (d>ne_max) ne_max = d;
//        }
//        double ne_tol = ne_max/100;
//        for(int i=0; i<n_sample; ++i) {
//            ion_sample.density_count(i, ne[i]);
//            if (ne[i]>ne_tol) ion_sample.cooling_count(i);
//        }
//    }
//
//
//    //Time through the cooler
//    t_cooler_ = cooler.length()/(ion.beta()*k_c);
//
//    //Transfer into e- beam frame
//    beam_frame(n_sample, ebeam.gamma());
//
//
//    //Calculate friction force
//    if(ebeam.disp() && ebeam.shape()!=Shape::BLASKIEWICZ) {
////        ParticleBunch* ptr = dynamic_cast<ParticleBunch*>(ebeam.samples.get());
////        force(n_sample, ion, *ptr, cooler, force_solver);
////        auto p = ebeam.samples.get();
//
//        force(n_sample, ion, *ebeam.samples, cooler, force_solver);
//    }
//    else {
//        force(n_sample, ion, ebeam, cooler, force_solver);
//    }
////    //Restore the longitudinal velocity if it has been changed due to electron velocity gradient
////    restore_velocity(n_sample, ebeam);
//
//    //Special treatment for bunched electron beam to cool coasting ion beam
//    if(!ion.bunched()&&ebeam.bunched()) {
//        bunched_to_coasting(ion, ion_sample, ebeam, cooler, force_solver);
//        force(n_sample, ion, ebeam, cooler, force_solver);
//    }
//
//    //Transfer back to lab frame
//    lab_frame(n_sample, ebeam.gamma());
//
//    //Distribute the friction force into x,y direction.
//    force_distribute(n_sample,ion, ion_sample);

    //Original emittance
    double emit_x0, emit_y0, emit_z0;
    ion_sample.emit(emit_x0, emit_y0, emit_z0);

    if (n_piece==0)
        ecool_ions(force_solver, ion, ion_sample, cooler, ebeam, ring);
    else {
        ions_copy(ion_sample);
        double l = cooler.length()/n_piece;
        cooler.set_middle_beta_e(ebeam.twiss().bet_x, ebeam.twiss().bet_y);
        if (!force_solver.magnetized()) {
//        if (ebeam.shape() == Shape::GAUSSIAN_BUNCH_DISP) {
            ebeam.set_twiss(cooler.beta_he(), cooler.beta_ve(), cooler.alpha_he(), cooler.alpha_ve());
            ebeam.twiss_drift(l/2);
            ebeam.updates();
        }
        ions_drift(n_sample, l/2);
        ecool_ions(force_solver, ion, ion_sample, cooler, ebeam, ring);
        for (int i=1; i<n_piece; ++i) {
            ions_drift(n_sample, l);
            ebeam.twiss_drift(l);
            if (!force_solver.magnetized()) {
//            if (ebeam.shape() == Shape::GAUSSIAN_BUNCH_DISP) {
                ebeam.updates();
            }
             ecool_ions(force_solver, ion, ion_sample, cooler, ebeam, ring);
        }
        ions_drift(n_sample, l/2);
    }

//    //Apply kick
//    apply_kick(n_sample, ion, ion_sample);

    //New emittance
    double emit_x, emit_y, emit_z;
    auto t = ion_sample.get_twiss();

    adjust_disp_inv(t.disp_x, x_bet, dp_p, ion_sample.cdnt(Phase::X), n_sample);
    adjust_disp_inv(t.disp_dx, xp_bet, dp_p, xp, n_sample);
    adjust_disp_inv(t.disp_y, y_bet, dp_p, ion_sample.cdnt(Phase::Y), n_sample);
    adjust_disp_inv(t.disp_dy, yp_bet, dp_p, yp, n_sample);

    ion_sample.emit(x_bet, xp_bet, y_bet, yp_bet, dp_p, ion_sample.cdnt(Phase::DS), emit_x, emit_y, emit_z);

    rate_x = emit_x/emit_x0-1;
    rate_y = emit_y/emit_y0-1;
    rate_s = emit_z/emit_z0-1;
    double freq = k_c*ion.beta()/ring.circ()*cooler.section_number();
    rate_x *= freq;
    rate_y *= freq;
    rate_s *= freq;

    adjust_rate(ion, ebeam, {&rate_x, &rate_y, &rate_s});
}

void ECoolRate::ions_drift(int n, double l) {
    for(int i=0; i<n; ++i) x.at(i) += xp.at(i)*l;
    for(int i=0; i<n; ++i) y.at(i) += yp.at(i)*l;
}

void ECoolRate::ions_copy(Ions& ion_sample) {
    vector<double>& ions_x = ion_sample.cdnt(Phase::X);
    std::copy(ions_x.begin(), ions_x.end(), x.begin());

    vector<double>& ions_xp = ion_sample.cdnt(Phase::XP);
    std::copy(ions_xp.begin(), ions_xp.end(), xp.begin());

    vector<double>& ions_y = ion_sample.cdnt(Phase::Y);
    std::copy(ions_y.begin(), ions_y.end(), y.begin());

    vector<double>& ions_yp = ion_sample.cdnt(Phase::YP);
    std::copy(ions_yp.begin(), ions_yp.end(), yp.begin());

    vector<double>& ions_dpp = ion_sample.cdnt(Phase::DP_P);
    std::copy(ions_dpp.begin(), ions_dpp.end(), dp_p.begin());
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

void ECoolRate::save_force_sdds_head(std::ofstream& of, int n) {
    using std::endl;
    of<<"SDDS1"<<endl;
    of<<"! Define colums:"<<endl
        <<"&column name=ne, type=double, units=1/m^3, description=\"electron density\", &end"<<endl
        <<"&column name=v_tr, type=double, units=m/s, description=\"relative transverse velocity\", &end"<<endl
        <<"&column name=v_long, type=double, units=m/s, description=\"relative longitudinal velocity\", &end"<<endl
        <<"&column name=force_tr, type=double, units=N, description=\"transverse friction force\", &end"<<endl
        <<"&column name=force_l, type=double, units=N, description=\"longitudinal friction force\", &end"<<endl
        <<"!Declare ASCII data and end the header"<<endl
        <<"&data mode=ascii, &end"<<endl
        <<n<<endl;
}

void ForceCurve::force_to_file(FrictionForceSolver &force_solver, Beam &ion, Cooler &cooler, EBeam &ebeam){
    save_force = true;
    if(iszero(dp_p, 1e-14)) n_l = 0;
    if(iszero(angle, 1e-14)) n_tr = 0;
    int n_sample = (n_tr+1) * (2*n_l+1);
    if(n_sample>scratch_size) init_scratch(n_sample);
    Ions_MonteCarlo ion_sample(n_sample);
    if(density_e>0) ne.assign(n_sample, density_e*ebeam.gamma());
    else electron_density(ion_sample,ebeam);

    // space to dynamic
    double v = ion.beta()*k_c;
    if (dp_p<0) dp_p *= -1;
    if (angle<0) angle *= -1;
    double da = angle/n_tr;
    double dp = v*dp_p/n_l;
    double dpp = dp_p*v;
    for(int i=0; i<2*n_l+1; ++i) {
        double a = angle;
        for(int j=0; j<n_tr+1; ++j) {
            v_long[i*(n_tr+1)+j] = dpp;
            v_tr[i*(n_tr+1)+j] = v*sin(a);
            a -= da;
        }
        dpp -= dp;
    }

    //Time through the cooler
    t_cooler_ = cooler.length()/v;
    // Transfer into e- beam frame
    beam_frame(n_sample, ebeam.gamma());
    //Calculate friction force
    force(n_sample, ion, ebeam, cooler, force_solver);
}

