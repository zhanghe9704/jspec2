#include "fortran_interface.h"

void jspec_delete(void* ptr, JSPEC_Class name) {
    switch (name){
    case JSPEC_Class::BEAM:{
        delete static_cast<Beam*>(ptr);
        break;
    }
    case JSPEC_Class::LATTICE:{
        delete static_cast<Lattice*>(ptr);
        break;
    }
    case JSPEC_Class::RING:{
        delete static_cast<Ring*>(ptr);
        break;
    }
    case JSPEC_Class::COOLER:{
        delete static_cast<Cooler*>(ptr);
        break;
    }
    case JSPEC_Class::FRICTION_FORCE_SOLVER:{
        delete static_cast<FrictionForceSolver*>(ptr);
        break;           
    }
    case JSPEC_Class::EBEAM:{
        delete static_cast<EBeam*>(ptr);
        break;           
    }
    case JSPEC_Class::IONS:{
        delete static_cast<Ions_MonteCarlo*>(ptr);
        break;           
    }
    case JSPEC_Class::ECOOLRATE:{
        delete static_cast<ECoolRate*>(ptr);
        break;           
    }
    default:
        break;
    }
    return;
}

