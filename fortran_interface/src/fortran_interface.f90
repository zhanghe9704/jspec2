module JSPEC
    use, intrinsic :: iso_c_binding
    implicit none
    private
    type Ions_Montecarlo
        private
        type(c_ptr) :: object = c_null_ptr
    end type Ions_Montecarlo

    type Beam
        private
        type(c_ptr) :: object = c_null_ptr
    end type Beam

    type Ring
        private
        type(c_ptr) :: object = c_null_ptr
    end type Ring

    type Cooler
        private
        type (c_ptr) :: object = c_null_ptr
    end type Cooler

    type EBeam
        private
        type (c_ptr) :: object = c_null_ptr
    end type EBeam

    type Friction_Force_Solver
        private
        type (c_ptr) :: object = c_null_ptr
    end type Friction_Force_Solver

    type Lattice
        private
        type (c_ptr) :: object = c_null_ptr
    end type Lattice

    enum, bind(c) :: Force_Formula
        enumerator :: PARKHOMCHUK=0, NONMAG_DERBENEV, NONMAG_MESHKOV, NONMAG_NUM1D, NONMAG_NUM3D, MESHKOV, DSM
    end enum Force_Formula

  interface
    function c_ions_montecarlo_new (n) result (this) bind(c, name="ions_montecarlo_new")
        import
        type(c_ptr) :: this
        integer(c_int), value :: n
    end function c_ions_montecarlo_new

    function c_beam_bunched_new (charge_number, mass_number, kinectic_energy, emit_nx, emit_ny, dp_p, sigma_s, &
        n_particle) result (this) bind(c, name="beam_bunched_new")
        import
        type(c_ptr) :: this
        integer(c_int), value :: charge_number
        real(c_double), value :: mass_number, kinectic_energy, emit_nx, emit_ny, dp_p, sigma_s, n_particle
    end function c_beam_bunched_new

    function c_beam_coasting_new (charge_number, mass_number, kinectic_energy, emit_nx, emit_ny, dp_p, n_particle) &
        result (this) bind(c, name="beam_coasting_new")
        import
        type(c_ptr) :: this
        integer(c_int), value :: charge_number
        real(c_double), value :: mass_number, kinectic_energy, emit_nx, emit_ny, dp_p, n_particle
    end function c_beam_bunched_new

    function c_lattice_new (filename) result (this) bind(c, name="lattice_new")
        import
        type(c_ptr) :: this
        character(c_char) :: filename(*)
    end function c_lattice_new

    function c_ring_new (circ, beam) result (this) bind(c, name="ring_new")
        import
        type(c_ptr) :: this
        real(c_double), value :: circ
        type(c_ptr),  intent(in), value :: beam
    end function c_ring_new

    function c_ring_lattice_new (lattice, beam) result (this) bind(c, name="ring_lattice_new")
        import
        type(c_ptr) :: this
        type(c_ptr) :: lattice
        type(c_ptr) :: beam
    end function c_ring_lattice_new

    function c_cooler_new (length, section_number, magnetic_field, beta_h, beta_v, disp_h, disp_v, alpha_h, alpha_v, &
        der_disp_h, der_disp_v) result (this) bind(c, name="cooler_new")
        import
        type(c_ptr) :: this
        integer(c_int), value :: section_number
        real(c_double), value :: length, magnetic_field, beta_h, beta_v, disp_h, disp_v, alpha_h, alpha_v, der_disp_h, &
        der_disp_v
    end function c_cooler_new

    function c_force_solver_new(formula, limit) result (this) bind(c, name="force_solver_new")
        import
        type(c_ptr) :: this
        integer(c_int) :: formula, limit
    end function c_force_solver_new

    function c_uniform_cylinder_new(current, radius) result (this) bind(c, name="uniform_cylinder_new")
        import
        type(c_ptr) :: this
        real(c_double) :: current, radius
    end function c_uniform_cylinder_new

    function c_uniform_hollow_new(current, in_radius, out_radius) result (this) bind(c, name="uniform_hollow_new")
        import
        type(c_ptr) :: this
        real(c_double) :: current, in_radius, out_radius
    end function c_uniform_hollow_new

    function c_uniform_bunch_new(current, radius, length) result (this) bind(c, name="uniform_bunch_new")
        import
        type(c_ptr) :: this
        real(c_double) :: current, radius, length
    end function c_uniform_bunch_new

    function c_elliptic_uniform_bunch_new(current, rh, rv, length) result (this) bind(c, name="elliptic_uniform_bunch_new")
        import
        type(c_ptr) :: this
        real(c_double) :: current, rh, rv, length
    end function c_elliptic_uniform_bunch_new

    function c_particle_bunch_new(n_electron, filename, length) result (this) bind(c, name="particle_bunch_new")
        import
        type(c_ptr) :: this
        character(c_char) :: filename(*)
        real(c_double) :: n_electron, length
    end function c_particle_bunch_new

    function c_gaussian_bunch_new(n_electron, sigma_x, sigma_y, sigma_s) result (this) bind(c, name="gaussian_bunch_new")
        import
        type(c_ptr) :: this
        real(c_double) :: n_electron, sigma_x, sigma_y, sigma_s
    end function c_gaussian_bunch_new

    subroutine c_jspec_delete (this) bind(c, name="jspec_delete")
        import
        type(c_ptr), value :: this
    end subroutine c_ions_delete
  end interface

  interface new
    module procedure ions_montecarlo_new, beam_bunch_new, beam_coasting_new, c_lattice_new, c_ring_lattice_new, &
        c_ring_new, c_cooler_new, c_force_solver_new
  end interface

  interface delete
    module procedure ion_delete, beam_delete, ring_delete, lattice_delete, cooler_delete, ebeam_delete, &
        friction_force_solver_delete
  end interface
  public :: new, delete

  contains
    subroutine ions_montecarlo_new(this, n)
        type(BIons_Montecarlo), intent(inout) :: this
        integer :: n
        this%object = c_ions_montecarlo_new(n)
    end subroutine ions_montecarlo_new

    subroutine beam_coasting_new (this, charge_number, mass_number, kinectic_energy, emit_nx, emit_ny, dp_p, n_particle)
        type(Beam), intent(inout) :: this
        integer :: charge_number
        real :: mass_number, kinectic_energy, emit_nx, emit_ny, dp_p, n_particle
        this%object = c_beam_coasting_new (charge_number, mass_number, kinectic_energy, emit_nx, emit_ny, dp_p, sigma_s, n_particle)
    end subroutine beam_coasting_new

    subroutine beam_bunched_new (this, charge_number, mass_number, kinectic_energy, emit_nx, emit_ny, dp_p, n_particle)
        type(Beam), intent(inout) :: this
        integer :: charge_number
        real :: mass_number, kinectic_energy, emit_nx, emit_ny, dp_p, sigma_s, n_particle
        this%object = c_beam_bunched_new (charge_number, mass_number, kinectic_energy, emit_nx, emit_ny, dp_p, sigma_s, n_particle)
    end subroutine beam_bunched_new

    subroutine lattice_new (this, filename)
        type(Lattice), intent(inout) :: this
        character(len=*), parameter :: filename
        this%object = c_lattice_new(filename)
    end subroutine lattice_new

    subroutine ring_new (this, circ, beam)
        type(Ring), intent(inout) :: this
        real :: circ
        type(Beam) :: beam
        this%object = c_ring_new(circ, beam%object)
    end subroutine ring_new

    subroutine ring_lattice_new(this, lattice, beam)
        type(Ring), intent(inout) :: this
        type(Lattice) :: lattice
        type(Beam) :: beam
        this%object = c_ring_lattice_new(lattice%object, beam%object)
    end subroutine ring_lattice_new

    subroutine cooler_new (this, length, section_number, magnetic_field, beta_h, beta_v, disp_h, disp_v, alpha_h, &
        alpha_v, der_disp_h, der_disp_v)
        type(Cooler), intent(inout) :: this
        integer :: section_number
        real :: length, magnetic_field, beta_h, beta_v
        real, optional :: disp_h, disp_v, alpha_h, alpha_v, der_disp_h, der_disp_v
        real :: disp_h_, disp_v_, alpha_h_, alpha_v_, der_disp_h_, der_disp_v_
        disp_h_ = 0
        disp_v_ = 0
        alpha_h_ = 0
        alpha_v_ = 0
        der_disp_h_ = 0
        der_disp_v_ = 0
        if (present(disp_h)) disp_h_  = disp_h
        if (present(disp_v)) disp_v_  = disp_v
        if (present(der_disp_h)) der_disp_h_  = der_disp_h
        if (present(der_disp_v)) der_disp_v_  = der_disp_v
        if (present(alpha_h)) alpha_h_  = alpha_h
        if (present(alpha_v)) alpha_v_  = alpha_v
        this%object = c_cooler_new(length, section_number, magnetic_field, beta_h, beta_v, disp_h_, disp_v_, alpha_h_, &
        alpha_v_, der_disp_h_, der_disp_v_)
    end subroutine cooler_new

    subroutine force_solver_new (this, formula, limit)
        type(Friction_Force_Solver), intent(inout) :: this
        type(Force_Formula) :: formula
        integer, optional :: limit
        integer :: limit_ = 0
        if (present(limit)) limit_ = limit
        this%object = c_force_solver_new(formula, limit)
    end subroutine force_solver_new

    subroutine uniform_cylinder_new(this, current, radius)
        type(EBeam), intent(inout) :: this
        real :: current, radius
        this%object = c_uniform_cylinder_new(current, radius)
    end subroutine uniform_cylinder_new

    subroutine uniform_hollow_new(this, current, in_radius, out_radius)
        type(EBeam), intent(inout) :: this
        real :: current, in_radius, out_radius
        this%object = c_uniform_hollow_new(current, in_radius, out_radius)
    end subroutine uniform_hollow_new

    subroutine elliptic_uniform_bunch_new(this, current, rh, rv, length)
        type(EBeam), intent(inout) :: this
        real :: current, rh, rv, length
        this%object = c_elliptic_uniform_bunch_new(current, rh, rv, length)
    end subroutine elliptic_uniform_bunch_new

    subroutine uniform_bunch_new(this, current, radius, length)
        type(EBeam), intent(inout) :: this
        real :: current, radius, length
        this%object = c_uniform_bunch_new(current, radius, length)
    end subroutine uniform_bunch_new

    subroutine particle_bunch_new(this, n_electron, filename, length)
        type(EBeam), intent(inout) :: this
        real :: n_electron, length
        character(len=*), parameter :: filename
        this%object = c_particle_bunch_new(n_electron, filename, length)
    end subroutine particle_bunch_new

    subroutine gaussian_bunch_new(this, n_electron, sigma_x, sigma_y, sigma_s)
        type(EBeam), intent(inout) :: this
        real :: n_electron, sigma_x, sigma_y, sigma_s
        this%object = c_gaussian_bunch_new(n_electron, sigma_x, sigma_y, sigma_s)
    end subroutine gaussian_bunch_new

    subroutine ions_delete(this)
        type(Ions_Montecarlo), intent(inout) :: this
        call c_jspec_delete(this%object)
        this%object = c_null_ptr
    end subroutine ions_delete

    subroutine beam_delete(this)
        type(Beam), intent(inout) :: this
        call c_jspec_delete(this%object)
        this%object = c_null_ptr
    end subroutine beam_delete

    subroutine ring_delete(this)
        type(Ring), intent(inout) :: this
        call c_jspec_delete(this%object)
        this%object = c_null_ptr
    end subroutine ring_delete

    subroutine lattice_delete(this)
        type(Lattice), intent(inout) :: this
        call c_jspec_delete(this%object)
        this%object = c_null_ptr
    end subroutine lattice_delete

    subroutine cooler_delete(this)
        type(Cooler), intent(inout) :: this
        call c_jspec_delete(this%object)
        this%object = c_null_ptr
    end subroutine cooler_delete

    subroutine ebeam_delete(this)
        type(EBeam), intent(inout) :: this
        call c_jspec_delete(this%object)
        this%object = c_null_ptr
    end subroutine ebeam_delete

    subroutine friction_force_solver_delete(this)
        type(Friction_Force_Solver), intent(inout) :: this
        call c_jspec_delete(this%object)
        this%object = c_null_ptr
    end subroutine friction_force_solver_delete

end module JSPEC
