module jspec
    use, intrinsic :: iso_c_binding

    implicit none

    ! Declare the C++ class as a type in Fortran
    type, bind(c) :: Beam
        private
        type(c_ptr) :: object = c_null_ptr
    end type Beam

    type, bind(c) :: Lattice
        private
        type(c_ptr) :: object = c_null_ptr
    end type Lattice

    type, bind(c) :: Ring
        private
        type(c_ptr) :: object = c_null_ptr
    end type Ring

    type, bind(c) :: Cooler
        private
        type(c_ptr) :: object = c_null_ptr
    end type Cooler

    type, bind(c) :: FrictionForceSolver
        private
        type(c_ptr) :: object = c_null_ptr
    end type FrictionForceSolver

    type, bind(c) :: EBeam
        private
        type(c_ptr) :: obj_ptr = c_null_ptr
    end type EBeam

    type, bind(c) :: IonSamples
        private
        type(c_ptr) :: obj_ptr = c_null_ptr
    end type IonSamples

    type, bind(c) :: ECoolRate
        private
        type(c_ptr) :: obj_ptr = c_null_ptr
    end type ECoolRate
  
    enum, bind(c)
        enumerator :: JSPEC_Beam = 0
        enumerator :: JSPEC_Lattice 
        enumerator :: JSPEC_Ring
        enumerator :: JSPEC_COOELR
        enumerator :: JSPEC_FRICTION_FORCE_SOLVER
        enumerator :: JSPEC_EBEAM
        enumerator :: JSPEC_IONS
        enumerator :: JSPEC_ECOOLRATE
    end enum

    enum, bind(c)
        enumerator :: PARKHOMCHUK = 0
        enumerator :: NONMAG_DERBENEV
        enumerator :: NONMAG_MESHKOV
        enumerator :: NONMAG_NUM1D
        enumerator :: NONMAG_NUM3D
        enumerator :: MESHKOV
        enumerator :: DSM
    end enum

    interface

        ! interface for beam_bunched_new
        function beam_bunched_new_c(charge_number, mass_number, kinetic_energy, emit_nx, emit_ny, dp_p, sigma_s, &
            n_particle) bind(c, name="beam_bunched_new")
            use iso_c_binding
            import Beam
            type(Beam) :: beam_bunched_new_c
            integer(c_int), value :: charge_number
            real(c_double), value :: mass_number
            real(c_double), value :: kinetic_energy
            real(c_double), value :: emit_nx
            real(c_double), value :: emit_ny
            real(c_double), value :: dp_p
            real(c_double), value :: sigma_s
            real(c_double), value :: n_particle
        end function beam_bunched_new_c

        ! interface for beam_coasting_new
        function beam_coasting_new_c(charge_number, mass_number, kinetic_energy, emit_nx, emit_ny, dp_p, n_particle) &
            bind(c, name="beam_coasting_new")
            use iso_c_binding
            import Beam
            type(Beam) :: beam_coasting_new_c
            integer(c_int), value :: charge_number
            real(c_double), value :: mass_number
            real(c_double), value :: kinetic_energy
            real(c_double), value :: emit_nx
            real(c_double), value :: emit_ny
            real(c_double), value :: dp_p
            real(c_double), value :: n_particle
        end function beam_coasting_new_c

        function lattice_new_c(filename, length) bind(c, name="lattice_new")
            use iso_c_binding
            import Lattice
            type(Lattice) :: lattice_new_c
            character(kind=c_char, len=1), dimension(*), intent(in) :: filename
            integer(kind=c_int), value, intent(in) :: length
        end function lattice_new_c

        function ring_new_c(circ, beam_defined) bind(c, name="ring_new")
            use iso_c_binding
            import Ring, Beam
            type(Ring) :: ring_new_c
            real(c_double), value :: circ
            type(Beam), value :: beam_defined
        end function ring_new_c

        function ring_lattice_new_c(lattice_defined, beam_defined) bind(c, name="ring_lattice_new")
            use iso_c_binding
            import Ring, Beam, Lattice
            type(Ring) :: ring_lattice_new_c
            type(Lattice), value :: lattice_defined
            type(Beam), value :: beam_defined
        end function ring_lattice_new_c

        function cooler_new_c(length, section_number, magnetic_field, beta_h, beta_v, disp_h, disp_v, &
            alpha_h, alpha_v, der_disp_h, der_disp_v) bind(c, name="cooler_new")   
            use iso_c_binding
            import Cooler
            type(Cooler) :: cooler_new_c
            real(c_double), value :: length
            real(c_double), value :: section_number
            real(c_double), value :: magnetic_field
            real(c_double), value :: beta_h
            real(c_double), value :: beta_v
            real(c_double), value :: disp_h
            real(c_double), value :: disp_v
            real(c_double), value :: alpha_h
            real(c_double), value :: alpha_v
            real(c_double), value :: der_disp_h
            real(c_double), value :: der_disp_v
        end function cooler_new_c

        function force_solver_new_c(formula, limit) bind(c, name="force_solver_new")
            use iso_c_binding
            import FrictionForceSolver
            type(FrictionForceSolver) :: force_solver_new_c
            integer(c_int), value :: formula
            integer(c_int), value :: limit
        end function force_solver_new_c

        function create_uniform_cylinder(current, radius) bind(c, name="uniform_cylinder_new")
            use iso_c_binding
            import EBeam
            type(EBeam) :: create_uniform_cylinder
            real(c_double), value :: current
            real(c_double), value :: radius
        end function create_uniform_cylinder

        function create_uniform_hollow(current, in_radius, out_radius) bind(c, name="uniform_hollow_new")
            use iso_c_binding
            import EBeam
            type(EBeam) :: create_uniform_hollow
            real(c_double), value :: current
            real(c_double), value :: in_radius
            real(c_double), value :: out_radius
        end function create_uniform_hollow

        function create_uniform_hollow_bunch(current, in_radius, out_radius, length) bind(c, name="uniform_hollow_bunch_new")
            use iso_c_binding
            import EBeam
            type(EBeam) :: create_uniform_hollow_bunch
            real(c_double), value :: current
            real(c_double), value :: in_radius
            real(c_double), value :: out_radius
            real(c_double), value :: length
        end function create_uniform_hollow_bunch

        function create_uniform_bunch(current, radius, length) bind(c, name="uniform_bunch_new")
            use iso_c_binding
            import EBeam
            type(EBeam) :: create_uniform_bunch
            real(c_double), value :: current
            real(c_double), value :: radius
            real(c_double), value :: length
        end function create_uniform_bunch

        function create_elliptic_uniform_bunch(current, rh, rv, length) bind(c, name="elliptic_uniform_bunch_new")
            use iso_c_binding
            import EBeam
            type(EBeam) :: create_elliptic_uniform_bunch
            real(c_double), value :: current
            real(c_double), value :: rh
            real(c_double), value :: rv
            real(c_double), value :: length
        end function create_elliptic_uniform_bunch

        function create_gaussian_bunch(n_electron, sigma_x, sigma_y, sigma_s) bind(c, name="gaussian_bunch_new")
            use iso_c_binding
            import EBeam
            type(EBeam) :: create_gaussian_bunch
            real(c_double), value :: n_electron
            real(c_double), value :: sigma_x
            real(c_double), value :: sigma_y
            real(c_double), value :: sigma_s
        end function create_gaussian_bunch

        function particle_bunch_new_c(n_electron, filename, str_length, length) bind(c, name="particle_bunch_new")
            use iso_c_binding
            import EBeam
            type(EBeam) :: particle_bunch_new_c
            integer(c_int), value :: str_length
            real(c_double), value :: n_electron
            character(kind=c_char, len=1), dimension(*), intent(in) :: filename
            real(c_double), value :: length
        end function particle_bunch_new_c

        function create_ions(n) bind(c, name="ions_montecarlo_new")
            use iso_c_binding
            import IonSamples
            type(IonSamples) :: create_ions  ! Return a Fortran representation of the C++ object
            integer(c_int), value :: n
        end function create_ions

        function create_ecool_rate_calculator() bind(c, name="ecool_rate_calculator_new")
            use iso_c_binding
            import ECoolRate
            type(ECoolRate) :: create_ecool_rate_calculator
        end function create_ecool_rate_calculator

        subroutine jspec_delete(ptr, name) bind(c, name="jspec_delete")
            use iso_c_binding
            type(c_ptr), value :: ptr
            integer(c_int), value :: name
        end subroutine jspec_delete

        subroutine ecool_rate1(ecool_obj, force, ion, n_sample, e_cooler, e_beam, ion_ring, rx, ry, rs) bind(c, name="ecool_rate1")
            use iso_c_binding
            import ECoolRate, FrictionForceSolver, Beam, Cooler, EBeam, Ring
            type(ECoolRate), intent(in) :: ecool_obj
            type(FrictionForceSolver), intent(in) :: force
            type(Beam), intent(in) :: ion
            type(Cooler), intent(in) :: e_cooler
            type(EBeam), intent(in) :: e_beam
            type(Ring), intent(in) :: ion_ring
            integer(c_int), value :: n_sample
            real(c_double) :: rx, ry, rs
        end subroutine ecool_rate1

        subroutine ecool_rate2(ecool_obj, force, ion, ptcl, e_cooler, e_beam, ion_ring, rx, ry, rs) bind(c, name="ecool_rate2")
            use iso_c_binding
            import ECoolRate, FrictionForceSolver, Beam, IonSamples, Cooler, EBeam, Ring
            type(ECoolRate), intent(in) :: ecool_obj
            type(FrictionForceSolver), intent(in) :: force
            type(Beam), intent(in) :: ion
            type(IonSamples), intent(in) :: ptcl
            type(Cooler), intent(in) :: e_cooler
            type(EBeam), intent(in) :: e_beam
            type(Ring), intent(in) :: ion_ring
            real(c_double) :: rx, ry, rs
        end subroutine ecool_rate2

        subroutine ebeam_set_gamma(ebeam_ptr, gamma) bind(C, name="ebeam_set_gamma")
            use iso_c_binding
            import EBeam
            type(EBeam), intent(in), value :: ebeam_ptr
            real(c_double), intent(in) :: gamma
        end subroutine ebeam_set_gamma
    
        subroutine ebeam_set_temperature(ebeam_ptr, tr, tl) bind(C, name="ebeam_set_temperature")
            use iso_c_binding
            import EBeam
            type(EBeam), intent(in), value :: ebeam_ptr
            real(c_double), intent(in) :: tr, tl
        end subroutine ebeam_set_temperature
    end interface

    ! Generic interface
    interface create_beam
        procedure beam_coasting_new_c
        procedure beam_bunched_new_c
    end interface create_beam

    interface create_ring
        procedure ring_new_c
        procedure ring_lattice_new_c
    end interface create_ring

    interface ecool_rate
        procedure ecool_rate1
        procedure ecool_rate2
    end interface ecool_rate

    ! interface 
    !     function create_lattice(filename) result(ptr)
    !         import Lattice
    !         character(len=*), intent(in) :: filename
    !         type(Lattice) :: ptr
    !     end function create_lattice
    ! end interface

    contains

    ! Fortran-friendly wrapper
        function create_lattice(filename) result(ptr)
            character(len=*), intent(in) :: filename
            type(Lattice) :: ptr
            ptr = lattice_new_c(filename, len_trim(filename))
        end function create_lattice

        function create_cooler(length, section_number, magnetic_field, beta_h, beta_v, &
            disp_h, disp_v, alpha_h, alpha_v, der_disp_h, der_disp_v) result(cool)
            use, intrinsic :: iso_c_binding, only: c_double

            real(c_double), intent(in) :: length
            real(c_double), intent(in) :: section_number
            real(c_double), intent(in) :: magnetic_field
            real(c_double), intent(in) :: beta_h
            real(c_double), intent(in) :: beta_v
            real(c_double), intent(in), optional :: disp_h
            real(c_double), intent(in), optional :: disp_v
            real(c_double), intent(in), optional :: alpha_h
            real(c_double), intent(in), optional :: alpha_v
            real(c_double), intent(in), optional :: der_disp_h
            real(c_double), intent(in), optional :: der_disp_v

            type(Cooler) :: cool

            ! Check for optional arguments and assign default values if they are not present
            if (present(disp_h) .AND. present(alpha_h) .AND. present(der_disp_h)) then
                cool = cooler_new_c(length, section_number, magnetic_field, beta_h, beta_v, &
                                    disp_h, disp_v, alpha_h, alpha_v, der_disp_h, der_disp_v)
            else if (present(disp_h) .AND. present(alpha_h)) then
                cool = cooler_new_c(length, section_number, magnetic_field, beta_h, beta_v, &
                                    disp_h, disp_v, alpha_h, alpha_v, 0.0_C_DOUBLE, 0.0_C_DOUBLE)
            else if (present(disp_h)) then
                cool = cooler_new_c(length, section_number, magnetic_field, beta_h, beta_v, &
                                    disp_h, disp_v, 0.0_C_DOUBLE, 0.0_C_DOUBLE, 0.0_C_DOUBLE, 0.0_C_DOUBLE)
            else
                cool = cooler_new_c(length, section_number, magnetic_field, beta_h, beta_v, & 
                    0.0_C_DOUBLE, 0.0_C_DOUBLE, 0.0_C_DOUBLE, 0.0_C_DOUBLE, 0.0_C_DOUBLE, 0.0_C_DOUBLE)
            end if
        end function create_cooler

        function create_force_solver(formula, limit) result(force)
            use, intrinsic :: iso_c_binding, only: c_int

            integer(c_int), intent(in) :: formula
            integer(c_int), intent(in), optional :: limit

            type(FrictionForceSolver) :: force

            ! Check for optional arguments and assign default values if they are not present
             if (present(limit)) then
                force = force_solver_new_c(formula, limit)
            else
                force = force_solver_new_c(formula, 100)
            end if
        end function create_force_solver

        function create_particle_bunch(n_electron, filename, length) result(e_beam)
            use iso_c_binding
            type(EBeam) :: e_beam
            real(c_double), value :: n_electron
            character(len=*), intent(in) :: filename
            real(c_double), intent(in), optional :: length

            if (present(length)) then
                e_beam = particle_bunch_new_c(n_electron, filename, len_trim(filename), 0.0_C_DOUBLE)
            else
                e_beam = particle_bunch_new_c(n_electron, filename, len_trim(filename), length)
            end if
        end function create_particle_bunch

        function ebeam_get_ptr(e) result(ptr)
            type(EBeam), intent(in) :: e
            type(c_ptr) :: ptr
            ptr = e%obj_ptr
        end function ebeam_get_ptr
end module jspec
