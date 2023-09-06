# include "fortran_interface.h"

void jspec_delete(void* ptr, JSPEC_Class name) {
    switch (name)
    {
    case JSPEC_Class::BEAM:
        delete static_cast<Beam*>(ptr);
        break;
    
    default:
        break;
    }
    return;
}

