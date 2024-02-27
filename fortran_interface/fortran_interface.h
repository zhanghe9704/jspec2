
#ifndef FORTRAN_INTERFACE_H
#define FORTRAN_INTERFACE_H

#include "beam.h"
#include "cooler.h"
#include "ecooling.h"
#include "force.h"
#include "ions.h"
#include "ring.h"
#include "simulator.h"
#include "rms_dynamic.h"
#include "particle_model.h"
#include "turn_by_turn.h"

#include <string>

enum class JSPEC_Class {BEAM, LATTICE, RING, COOLER, FRICTION_FORCE_SOLVER, EBEAM, IONS, ECOOLRATE, SIMULATOR, IBSSOLVER};

extern "C" {
Ions_MonteCarlo* ions_montecarlo_new(int n) {
   return new Ions_MonteCarlo(n);
}

Beam* beam_bunched_new(int charge_number, double mass, double kinetic_energy, double emit_nx, double emit_ny,
                       double dp_p, double sigma_s, double n_particle) {
    return new Beam(charge_number, mass/k_u, kinetic_energy, emit_nx, emit_ny, dp_p, sigma_s, n_particle);
}

Beam* beam_coasting_new(int charge_number, double mass, double kinetic_energy, double emit_nx, double emit_ny,
                        double dp_p, double n_particle) {
    return new Beam(charge_number, mass/k_u, kinetic_energy, emit_nx, emit_ny, dp_p, n_particle);
}
//
Lattice* lattice_new(char* filename, int length) {
   assert(filename && "Error: Invalid filename for lattice!");
   std::string file(filename, length);
   return new Lattice(file);
}

Ring* ring_new(double circ, Beam* beam_defined) {
   return new Ring(circ, *beam_defined);
}

Ring* ring_lattice_new(Lattice* lattice_defined, Beam* beam_defined) {
   return new Ring(*lattice_defined, *beam_defined);
}

Cooler* cooler_new(double length, double section_number, double magnetic_field, double beta_h, double beta_v, double disp_h=0,
          double disp_v=0, double alpha_h=0, double alpha_v=0, double der_disp_h=0, double der_disp_v=0) {
   return new Cooler(length, section_number, magnetic_field, beta_h, beta_v, disp_h, disp_v, alpha_h, alpha_v,
                     der_disp_h, der_disp_v);
}

FrictionForceSolver* force_solver_new(ForceFormula formula, int limit=100) {
   switch(formula) {
        case ForceFormula::PARKHOMCHUK: {
           return new ForcePark();
        }
        case ForceFormula::MESHKOV: {
           return new ForceMeshkov();
        }
        case ForceFormula::NONMAG_MESHKOV: {
           return new ForceNonMagMeshkov();
        }
        case ForceFormula::NONMAG_DERBENEV: {
           return new ForceNonMagDerbenev();
        }
        case ForceFormula::NONMAG_NUM1D: {
           return new ForceNonMagNumeric1D(limit);
        }
        case ForceFormula::NONMAG_NUM3D: {
           return new ForceNonMagNumeric3D(limit);
        }
        case ForceFormula::DSM: {
           return new ForceDSM();
        }
        default: {
           assert(false&&"WRONG FRICTION FORCE FORMULA SELECTED!");
        }
   }
}

IBSSolver* ibs_solver_new(IBSModel model, double log_c, double k, int nt = 0) {
    switch(model) {
        case IBSModel::BM: {
            return new IBSSolver_BM(log_c, k);
        }
        case IBSModel::BMC: {
            return new IBSSolver_BM_Complete(nt, log_c, k);
        }
        case IBSModel::BMZ: {
            return new IBSSolver_BMZ(nt, log_c, k);
        }
        default: {
           assert(false&&"WRONG IBS MODEL SELECTED!");
        }
    }
}

Simulator* simulator_new(DynamicModel dynamic_model, double time, int n_step) {
   switch(dynamic_model) {
        case DynamicModel::RMS: {
           return new RMSModel(time, n_step);
        }
        case DynamicModel::PARTICLE: {
           return new ParticleModel(time, n_step);
        }
        case DynamicModel::TURN_BY_TURN: {
           return new TurnByTurnModel(time, n_step);
        }
        default: {
           assert(false&&"WRONG DYNAMIC MODEL FOR SIMULATION SELECTED!");
        }
   }
}

void simulation_run(Simulator* simulator, Beam& ion, Ions& ion_sample, Cooler& cooler, EBeam& ebeam,
                     Ring& ring, IBSSolver* ibs_solver, ECoolRate* ecool_solver,
                     FrictionForceSolver* force_solver) {
    LuminositySolver* lum_solver = nullptr;
    simulator->run(ion, ion_sample, cooler, ebeam, ring, ibs_solver, ecool_solver, force_solver, lum_solver);
}

//
EBeam* uniform_cylinder_new(double current, double radius) {
   return new UniformCylinder(current, radius);
}

EBeam* uniform_hollow_new(double current, double in_radius, double out_radius) {
   return new UniformHollow(current, in_radius, out_radius);
}

EBeam* uniform_hollow_bunch_new(double current, double in_radius, double out_radius, double length) {
   return new UniformHollowBunch(current, in_radius, out_radius, length);
}

EBeam* uniform_bunch_new(double current, double radius, double length) {
   return new UniformBunch(current, radius, length);
}

EBeam* elliptic_uniform_bunch_new(double current, double rh, double rv, double length) {
   return new EllipticUniformBunch(current, rh, rv, length);
}

EBeam* gaussian_bunch_new(double n_electron, double sigma_x, double sigma_y, double sigma_s) {
   auto ptr = new GaussianBunch(n_electron, sigma_x, sigma_y, sigma_s);
   return ptr;
}

EBeam* particle_bunch_new(double n_electron, char* filename, int str_length, double length = 0) {
   assert(filename && "Error: Invalid file name for particle bunch electron beam!");
   std::string file(filename, str_length);
   return new ParticleBunch(n_electron, file, length);
}

ECoolRate* ecool_rate_calculator_new() {
    return new ECoolRate();
}

void jspec_delete(void* ptr, JSPEC_Class name);

void ecool_rate1(ECoolRate* ecool_obj, FrictionForceSolver &force, Beam &ion, int n_sample, Cooler &cooler,
           EBeam &ebeam, Ring &ring, double &rx, double &ry, double &rs) {
    ecool_obj->ecool_rate(force, ion, n_sample, cooler, ebeam, ring, rx, ry, rs);
}

void ecool_rate2(ECoolRate* ecool_obj, FrictionForceSolver &force, Beam &ion, Ions &ptcl, Cooler &cooler,
           EBeam &ebeam, Ring &ring, double &rx, double &ry, double &rs) {
    ecool_obj->ecool_rate(force, ion, ptcl, cooler, ebeam, ring, rx, ry, rs);
}

void ebeam_set_gamma(EBeam* ebeam, double gamma) {
   ebeam->set_gamma(gamma);
}

void ebeam_set_temperature(EBeam* ebeam, double tr, double tl) {
   ebeam->set_tpr(tr, tl);
}

}



#endif // FORTRAN_INTERFACE_H
