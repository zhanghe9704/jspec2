module jspec
    use, intrinsic :: iso_c_binding

    implicit none

    ! Declare the C++ class as a type in Fortran
    type, bind(c) :: Beam
        private
        type(c_ptr) :: object = c_null_ptr
    end type Beam

  
    enum, bind(c)
        enumerator :: JSPEC_Beam = 0
        enumerator :: JSPEC_Lattice = 1
    end enum

    interface

        ! interface for beam_bunched_new

        function beam_bunched_new_c(charge_number, mass_number, kinetic_energy, emit_nx, emit_ny, dp_p, sigma_s, &
            n_particle) bind(c, name="beam_bunched_new")
            use iso_c_binding
            type(c_ptr) :: beam_bunched_new_c
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
            type(c_ptr) :: beam_coasting_new_c
            integer(c_int), value :: charge_number
            real(c_double), value :: mass_number
            real(c_double), value :: kinetic_energy
            real(c_double), value :: emit_nx
            real(c_double), value :: emit_ny
            real(c_double), value :: dp_p
            real(c_double), value :: n_particle
        end function beam_coasting_new_c

        function lattice_new_c(filename, length) bind(c, name="lattice_new")
            import :: c_ptr, c_char, c_int

            type(c_ptr) :: lattice_new_c
            character(kind=c_char, len=1), dimension(*), intent(in) :: filename
            integer(kind=c_int), value, intent(in) :: length
        end function lattice_new_c

        ! interface for jspec_delete_beam
        subroutine jspec_delete_beam(ptr) bind(c, name="jspec_delete_beam")
            use iso_c_binding
            type(c_ptr), value :: ptr
        end subroutine jspec_delete_beam

        subroutine jspec_delete(ptr, name) bind(c, name="jspec_delete")
            use iso_c_binding
            type(c_ptr), value :: ptr
            integer(c_int), value :: name
        end subroutine jspec_delete
    end interface

    ! Generic interface
    interface create_beam
        procedure beam_coasting_new_c
        procedure beam_bunched_new_c
    end interface create_beam

    contains

    ! Fortran-friendly wrapper
        function lattice_new(filename) result(ptr)
            character(len=*), intent(in) :: filename
            type(c_ptr) :: ptr
            ptr = lattice_new_c(filename, len_trim(filename))
        end function lattice_new

end module jspec
