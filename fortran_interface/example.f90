
program main
    use jspec, only: create_beam, create_lattice, create_ring, create_cooler, &
                     create_force_solver, create_gaussian_bunch, create_ecool_rate_calculator, &
                     ebeam_set_gamma, ebeam_set_temperature, ebeam_get_ptr,&
                     ecool_rate, JSPEC_Beam, JSPEC_Lattice, JSPEC_Ring, JSPEC_COOELR, &
                     JSPEC_FRICTION_FORCE_SOLVER, JSPEC_EBEAM, JSPEC_ECOOLRATE, PARKHOMCHUK, &
                     Beam, Lattice, Ring, Cooler, FrictionForceSolver, EBeam, ECoolRate
    ! use jspec, only: create_beam, Beam
    use iso_c_binding
    implicit none

    ! import Beam, Lattice, Ring, Cooler, FrictionForceSolver, EBeam

    type(Beam) :: my_beam
    type(Lattice) :: my_lattice
    type(Ring) :: my_ring
    type(Cooler) :: my_cooler
    type(FrictionForceSolver) :: my_force_solver
    type(EBeam) :: my_ebeam
    real(8) :: rx, ry, rs
    type(ECoolRate) :: rate_ec

    ! Create the beam
    integer(c_int) :: charge = 1
    real(c_double) :: mass = 938.272
    real(c_double) :: ke = 1.0d5
    real(c_double) :: np = 1.06d10
    real(c_double) :: dp = 0.0006
    real(c_double) :: ds = 0.025
    real(c_double) :: ex = 0.65d-6
    real(c_double) :: ey = 0.13d-6

    real(c_double) :: length = 60
    real(c_double) :: n_section = 1
    real(c_double) :: mag_field = 1
    real(c_double) :: twiss_beta = 100
    ! real(c_double) :: dx = 1.8
    ! real(c_double) :: dy = 0.7

    real(8) :: gamma, beta, sigma_x, sigma_y
    real(c_double) :: ne = 7.39d9

    integer(c_int) :: n_sample = 10000

    real(c_double) :: tmp_tr = 0.246
    real(c_double) :: tmp_l = 0.184

    integer(c_intptr_t) :: address

        gamma = ke/mass + 1
    beta = sqrt(1-gamma ** (-2))
    sigma_x = sqrt(twiss_beta*ex/(beta*gamma))
    sigma_y = sqrt(twiss_beta*ey/(beta*gamma))

    print*, "beta: ", beta
    print*, "gamma: ", gamma



    

    my_beam = create_beam(charge, mass, ke, ex, ey, dp, ds, np)

    ! Create the lattice from some file (example filename "lattice.txt")
    my_lattice = create_lattice("lattice.txt")

    ! Create a ring
    my_ring = create_ring(my_lattice, my_beam)

    ! Create a cooler
    my_cooler = create_cooler(length, n_section, mag_field, twiss_beta, twiss_beta)
    ! my_cooler = create_cooler(length, n_section, mag_field, twiss_beta, twiss_beta, dx, dy)

    ! Create friction force solver
    my_force_solver = create_force_solver(PARKHOMCHUK)

    ! Create electron beam with Gaussian distribution
    my_ebeam = create_gaussian_bunch(ne, sigma_x, sigma_y, ds)

    ! address = transfer(ebeam_get_ptr(my_ebeam), address)
    ! print *, "Address (c_ptr): "
    ! write(*, '(Z16)') address
    ! print *, c_loc(ebeam_get_ptr(my_ebeam))

    call ebeam_set_gamma(my_ebeam, gamma)
    call ebeam_set_temperature(my_ebeam, tmp_tr, tmp_l)

    ! ! Call ecool_rate function using the created objects and some example values for rx, ry, and rs
    ! ! rx = 1.0d-3
    ! ! ry = 1.0d-3
    ! ! rs = 1.0d-2
    rate_ec = create_ecool_rate_calculator()

    
    call ecool_rate(rate_ec, my_force_solver, my_beam, n_sample, my_cooler, my_ebeam, my_ring, rx, ry, rs)

    print *, 'Rx:', rx
    print *, 'Ry:', ry
    print *, 'Rs:', rs

    ! Clean up: One might want to call jspec_delete for the created objects to free the memory (if required by the underlying C++ library)
    ! call jspec_delete(c_loc(my_beam%object), JSPEC_Beam)
    ! call jspec_delete(c_loc(my_lattice%object), JSPEC_Lattice)
    ! call jspec_delete(c_loc(my_ring%object), JSPEC_Ring)
    ! call jspec_delete(c_loc(my_cooler%object), JSPEC_COOELR)
    ! call jspec_delete(c_loc(my_force_solver%object), JSPEC_FRICTION_FORCE_SOLVER)
    ! call jspec_delete(c_loc(my_ebeam%obj_ptr), JSPEC_EBEAM)

end program main
