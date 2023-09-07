
#ifndef FORTRAN_INTERFACE_H
#define FORTRAN_INTERFACE_H

#include "beam.h"
#include "cooler.h"
#include "force.h"
#include "ions.h"
#include "ring.h"

#include <string>

enum class JSPEC_Class {BEAM};

extern "C" {
//Ions_MonteCarlo* ions_montecarlo_new(int n) {
//    return new Ions_MonteCarlo(n);
//}

Beam* beam_bunched_new(int charge_number, double mass_number, double kinetic_energy, double emit_nx, double emit_ny,
                       double dp_p, double sigma_s, double n_particle) {
    return new Beam(charge_number, mass_number, kinetic_energy, emit_nx, emit_ny, dp_p, sigma_s, n_particle);
}

Beam* beam_coasting_new(int charge_number, double mass_number, double kinetic_energy, double emit_nx, double emit_ny,
                        double dp_p, double n_particle) {
    return new Beam(charge_number, mass_number, kinetic_energy, emit_nx, emit_ny, dp_p, n_particle);
}
//
Lattice* lattice_new(char* filename, int length) {
   assert(filename && "Error: Invalid filename for lattice!");
   std::string file(filename, length);
   return new Lattice(file);
}

//Ring* ring_new(double circ, Beam* beam_defined) {
//    return new Ring(circ, *beam_defined);
//}
//
//Ring* ring_lattice_new(Lattice* lattice_defined, Beam* beam_defined) {
//    return new Ring(*lattice_defined, *beam_defined);
//}
//
//Cooler* cooler_new(double length, double section_number, double magnetic_field, double beta_h, double beta_v, double disp_h=0,
//           double disp_v=0, double alpha_h=0, double alpha_v=0, double der_disp_h=0, double der_disp_v=0) {
//    return new Cooler(length, section_number, magnetic_field, beta_h, beta_v, disp_h, disp_v, alpha_h, alpha_v,
//                      der_disp_h, der_disp_v);
//}

//FrictionForceSolver* force_solver_new(ForceFormula formula, int limit=100) {
//    switch(formula) {
//         case ForceFormula::PARKHOMCHUK: {
//            return new ForcePark();
//         }
//         case ForceFormula::MESHKOV: {
//            return new ForceMeshkov();
//         }
//         case ForceFormula::NONMAG_MESHKOV: {
//            return new ForceNonMagMeshkov();
//         }
//         case ForceFormula::NONMAG_DERBENEV: {
//            return new ForceNonMagDerbenev();
//         }
//         case ForceFormula::NONMAG_NUM1D: {
//            assert(limit && "Error: Parameter limit is needed for 1D numerical formula of non-magnetic friction force!");
//            return new ForceNonMagNumeric1D(limit);
//         }
//         case ForceFormula::NONMAG_NUM3D: {
//            return new ForceNonMagNumeric3D(limit);
//         }
//         case ForceFormula::DSM: {
//            return new ForceDSM();
//         }
//         default: {
//            assert(false&&"WRONG FRICTION FORCE FORMULA SELECTED!");
//         }
//    }
//}
//
//EBeam* uniform_cylinder_new(double current, double radius) {
//    return new UniformCylinder(current, radius);
//}
//
//EBeam* uniform_hollow_new(double current, double in_radius, double out_radius) {
//    return new UniformHollow(current, in_radius, out_radius);
//}
//
//EBeam* uniform_hollow_bunch_new(double current, double in_radius, double out_radius, double length) {
//    return new UniformHollowBunch(current, in_radius, out_radius, length);
//}
//
//EBeam* uniform_bunch_new(double current, double radius, double length) {
//    return new UniformBunch(current, radius, length);
//}
//
//EBeam* elliptic_uniform_bunch_new(double current, double rh, double rv, double length) {
//    return new EllipticUniformBunch(current, rh, rv, length);
//}
//
//EBeam* gaussian_bunch_new(double n_electron, double sigma_x, double sigma_y, double sigma_s) {
//    return new GaussianBunch(n_electron, sigma_x, sigma_y, sigma_s);
//}
//
//EBeam* particle_bunch_new(double n_electron, char* filename, double length = 0) {
//    assert(filename && "Error: Invalid file name for particle bunch electron beam!");
//    std::string file(filename);
//    return new ParticleBunch(n_electron, file, length);
//}

//void jspec_delete(Ions* ptr) {delete ptr;}
void jspec_delete_beam(Beam* ptr)  {delete ptr;}
//void jspec_delete(FrictionForceSolver* ptr)  {delete ptr;}
//void jspec_delete(Lattice* ptr)  {delete ptr;}
//void jspec_delete(Ring* ptr)  {delete ptr;}
//void jspec_delete(Cooler* ptr)  {delete ptr;}
//void jspec_delete(EBeam* ptr)  {delete ptr;}

void jspec_delete(void* ptr, JSPEC_Class name);


}



#endif // FORTRAN_INTERFACE_H
