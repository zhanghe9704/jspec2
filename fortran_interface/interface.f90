module jspec
    use, intrinsic :: iso_c_binding

    implicit none

    ! Declare the C++ class as a type in Fortran
    type, bind(c) :: Beam
        private
        type(c_ptr) :: object = c_null_ptr
    end type Beam

    interface

        ! interface for beam_bunched_new
        function c_beam_bunched_new(charge_number, mass_number, kinetic_energy, emit_nx, emit_ny, dp_p, sigma_s, &
            n_particle) bind(c, name="beam_bunched_new")
            use iso_c_binding
            type(c_ptr) :: c_beam_bunched_new
            integer(c_int), value :: charge_number
            real(c_double), value :: mass_number
            real(c_double), value :: kinetic_energy
            real(c_double), value :: emit_nx
            real(c_double), value :: emit_ny
            real(c_double), value :: dp_p
            real(c_double), value :: sigma_s
            real(c_double), value :: n_particle
        end function c_beam_bunched_new

        ! interface for beam_coasting_new
        function c_beam_coasting_new(charge_number, mass_number, kinetic_energy, emit_nx, emit_ny, dp_p, n_particle) &
            bind(c, name="beam_coasting_new")
            use iso_c_binding
            type(c_ptr) :: c_beam_coasting_new
            integer(c_int), value :: charge_number
            real(c_double), value :: mass_number
            real(c_double), value :: kinetic_energy
            real(c_double), value :: emit_nx
            real(c_double), value :: emit_ny
            real(c_double), value :: dp_p
            real(c_double), value :: n_particle
        end function c_beam_coasting_new

        ! interface for jspec_delete_beam
        subroutine jspec_delete_beam(ptr) bind(c, name="jspec_delete_beam")
            use iso_c_binding
            type(c_ptr), value :: ptr
        end subroutine jspec_delete_beam

    end interface

end module jspec
