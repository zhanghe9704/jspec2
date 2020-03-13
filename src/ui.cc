#include "../include/ui.h"
#include <algorithm>
#include <cassert>
#include <iomanip>
#include <iostream>
#include <cmath>

#include "../include/constants.h"


using std::string;

//extern DynamicParas *dynamic_paras;
//extern IBSSolver *ibs_solver;
//extern EcoolRateParas *ecool_paras;
//extern ForceParas *force_paras;
//extern Luminosity *luminosity_paras;

//double vl_emit_nx = 0;
//double vl_emit_ny = 0;
//double vl_dp_p = 0;
//double vl_sigma_s = 0;
//double vl_rx_ibs = 0;
//double vl_ry_ibs = 0;
//double vl_rs_ibs = 0;
//double vl_rx_ecool = 0;
//double vl_ry_ecool = 0;
//double vl_rs_ecool = 0;
//double vl_rx_total = 0;
//double vl_ry_total = 0;
//double vl_rs_total = 0;
//double vl_t = 0;

//Simulator* simulator = nullptr;
//IBSSolver* ibs_solver = nullptr;
//ECoolRate* ecool_solver = nullptr;
//FrictionForceSolver* force_solver = nullptr;
//LuminositySolver* lum_solver = nullptr;
//Ions* ion_sample = nullptr;

std::unique_ptr<Simulator> simulator = nullptr;
std::unique_ptr<IBSSolver> ibs_solver = nullptr;
std::unique_ptr<ECoolRate> ecool_solver = nullptr;
std::unique_ptr<FrictionForceSolver> force_solver= nullptr;
std::unique_ptr<LuminositySolver> lum_solver = nullptr;
std::unique_ptr<Ions> ion_sample = nullptr;

Record uircd;

muParserHandle_t math_parser = NULL;
//std::vector<std::string> sections = {"SECTION_ION", "SECTION_RING", "SECTION_COOLER"};
std::vector<string> ION_ARGS = {"CHARGE_NUMBER", "MASS", "KINETIC_ENERGY", "NORM_EMIT_X", "NORM_EMIT_Y",
    "MOMENTUM_SPREAD", "PARTICLE_NUMBER", "RMS_BUNCH_LENGTH"};
std::vector<string> RUN_COMMANDS = {"CREATE_ION_BEAM", "CREATE_RING", "CREATE_E_BEAM", "CREATE_COOLER",
    "CALCULATE_IBS", "CALCULATE_ECOOL", "TOTAL_EXPANSION_RATE", "RUN_SIMULATION", "CALCULATE_LUMINOSITY", "SRAND"};
std::vector<string> RING_ARGS = {"LATTICE", "QX", "QY", "QS", "GAMMA_TR", "RF_V", "RF_H", "RF_PHI"};
std::vector<string> IBS_ARGS = {"NU","NV","NZ","LOG_C","COUPLING","MODEL","IBS_BY_ELEMENT"};
std::vector<string> COOLER_ARGS = {"LENGTH", "SECTION_NUMBER", "MAGNETIC_FIELD", "BET_X", "BET_Y", "DISP_X", "DISP_Y",
    "ALPHA_X", "ALPHA_Y", "DISP_DX", "DISP_DY"};
//std::vector<string> SCRATCH_COMMANDS = {"PRINT", "LIST_VAR", "LIST_CONST"};
//std::vector<string> E_BEAM_SHAPE_ARGS = {"SHAPE", "RADIUS", "CURRENT", "SIMGA_X", "SIGMA_Y", "SIGMA_Z", "LENGTH", "E_NUMBER"};
std::vector<string> E_BEAM_SHAPE_TYPES = {"DC_UNIFORM", "BUNCHED_GAUSSIAN", "BUNCHED_UNIFORM", "BUNCHED_UNIFORM_ELLIPTIC",
    "DC_UNIFORM_HOLLOW", "BUNCHED_UNIFORM_HOLLOW", "BUNCHED_USER_DEFINED"};
//std::vector<string> E_BEAM_ARGS = {"GAMMA", "TMP_TR", "TMP_L"};
std::vector<string> E_BEAM_ARGS = {"GAMMA", "TMP_TR", "TMP_L", "SHAPE", "RADIUS", "CURRENT", "SIGMA_X", "SIGMA_Y",
    "SIGMA_Z", "LENGTH", "E_NUMBER", "RH", "RV", "R_INNER", "R_OUTTER", "PARTICLE_FILE", "TOTAL_PARTICLE_NUMBER",
    "BOX_PARTICLE_NUMBER", "LINE_SKIP", "VEL_POS_CORR","BINARY_FILE","BUFFER_SIZE"};
std::vector<string> ECOOL_ARGS = {"SAMPLE_NUMBER", "FORCE_FORMULA"};
std::vector<string> FRICTION_FORCE_FORMULA = {"PARKHOMCHUK"};
std::vector<string> SIMULATION_ARGS = {"TIME", "STEP_NUMBER", "SAMPLE_NUMBER", "IBS", "E_COOL", "OUTPUT_INTERVAL",
    "SAVE_PARTICLE_INTERVAL", "OUTPUT_FILE", "MODEL", "REF_BET_X", "REF_BET_Y", "REF_ALF_X", "REF_ALF_Y",
    "REF_DISP_X", "REF_DISP_Y", "REF_DISP_DX", "REF_DISP_DY", "FIXED_BUNCH_LENGTH", "RESET_TIME", "OVERWRITE",
    "CALC_LUMINOSITY","INI_TIME"};
std::vector<string> DYNAMIC_VALUE = {"RMS", "PARTICLE", "MODEL_BEAM", "TURN_BY_TURN"};
std::vector<string> SCRATCH_ARGS = {"VL_EMIT_NX", "VL_EMIT_NY", "VL_MOMENTUM_SPREAD", "VL_BUNCH_LENGTH", "VL_RATE_IBS_X",
    "VL_RATE_IBS_Y", "VL_RATE_IBS_S", "VL_RATE_ECOOL_X", "VL_RATE_ECOOL_Y", "VL_RATE_ECOOL_S", "VL_RATE_TOTAL_X",
    "VL_RATE_TOTAL_Y", "VL_RATE_TOTAL_S", "VL_T"};
std::vector<string> LUMINOSITY_ARGS = {"DISTANCE_X", "DISTANCE_Y", "PARTICLE_NUMBER_1", "PARTICLE_NUMBER_2", "FREQUENCY",
    "BEAM_SIZE_X_1", "BEAM_SIZE_X_2", "BEAM_SIZE_Y_1", "BEAM_SIZE_Y_2", "BET_X_1", "BET_X_2", "BET_Y_1",
    "BET_Y_2", "GEO_EMIT_X_1", "GEO_EMIT_X_2", "GEO_EMIT_Y_1", "GEO_EMIT_Y_2", "USE_ION_EMITTANCE"};

std::map<std::string, Section> sections{
    {"SECTION_ION",Section::SECTION_ION},
    {"SECTION_RING",Section::SECTION_RING},
    {"SECTION_COOLER",Section::SECTION_COOLER},
    {"SECTION_RUN",Section::SECTION_RUN},
    {"SECTION_IBS",Section::SECTION_IBS},
    {"SECTION_SCRATCH", Section::SECTION_SCRATCH},
    {"SECTION_E_BEAM", Section::SECTION_E_BEAM},
    {"SECTION_ECOOL", Section::SECTION_ECOOL},
    {"SECTION_SIMULATION",Section::SECTION_SIMULATION},
    {"SECTION_LUMINOSITY",Section::SECTION_LUMINOSITY}
};

// Remove everything from the first "#" in the string
std::string remove_comments(std::string input_line) {
    return input_line.substr(0,input_line.find('#'));
}

// Trim the spaces at the head and the tail of the string
std::string trim_blank(std::string input_line) {
    using std::string;
    if (input_line.empty()) return input_line;
    string::size_type st = input_line.find_first_not_of(" ");
    if (st == string::npos) return "";
    string::size_type fi = input_line.find_last_not_of(" ")+1;
    return input_line.substr(st, fi);
}


std::string trim_tab(std::string input_line) {
    using std::string;
    if (input_line.empty()) return input_line;
    string::size_type st = input_line.find_first_not_of("\t");
    if (st == string::npos) return "";
    string::size_type fi = input_line.find_last_not_of("\t")+1;
    return input_line.substr(st, fi);
}

void str_toupper(std::string &str) {
    for (auto & c: str) c = toupper(c);
}

void define_e_beam(string &str, Set_e_beam *e_beam_args) {
    assert(e_beam_args!=nullptr && "SECTION_E_BEAM MUST BE CLAIMED!");
    string::size_type idx = str.find("=");
    assert(idx!=string::npos && "WRONG COMMAND IN SECTION_E_BEAM!");
    string var = str.substr(0, idx);
    string val = str.substr(idx+1);
    var = trim_blank(var);
    var = trim_tab(var);
    val = trim_blank(val);
    val = trim_tab(val);
    str_toupper(var);
    assert(std::find(E_BEAM_ARGS.begin(),E_BEAM_ARGS.end(),var)!=E_BEAM_ARGS.end()
           && "WRONG COMMANDS IN SECTION_E_BEAM!");

    if (var != "PARTICLE_FILE") {
        str_toupper(val);
    }

    if (var == "SHAPE") {
        assert(std::find(E_BEAM_SHAPE_TYPES.begin(),E_BEAM_SHAPE_TYPES.end(),val)!=E_BEAM_SHAPE_TYPES.end()
           && "UNDEFINED ELECTRON BEAM SHAPE!");
        e_beam_args->shape = val;
    }
    else if (var == "PARTICLE_FILE") {
        e_beam_args->particle_file = val;
    }
    else if (var == "VEL_POS_CORR") {
        if (val == "TRUE") {
            e_beam_args->corr = true;
        }
        else if (val == "FALSE") {
            e_beam_args->corr = false;
        }
        else {
            assert(false&& "WRONG VALUE FOR VEL_POS_CORR FOR E_BEAM!");
        }
    }
    else if (var == "BINARY_FILE") {
        if (val == "TRUE") {
            e_beam_args->binary = true;
        }
        else if (val == "FALSE") {
            e_beam_args->binary = false;
        }
        else {
            assert(false&& "WRONG VALUE FOR VEL_POS_CORR FOR E_BEAM!");
        }
    }
    else {
        if (math_parser == NULL) {
            if(var == "E_NUMBER") {
                e_beam_args->n = std::stod(val);
            }
            else if (var == "RADIUS") {
                e_beam_args->radius = std::stod(val);
            }
            else if (var == "CURRENT") {
                e_beam_args->current = std::stod(val);
            }
            else if (var == "SIMGA_X") {
                e_beam_args->sigma_x = std::stod(val);
            }
            else if (var == "SIGMA_Y") {
                e_beam_args->sigma_y = std::stod(val);
            }
            else if (var == "SIGMA_Z") {
                e_beam_args->sigma_z = std::stod(val);
            }
            else if (var == "RH") {
                e_beam_args->rh = std::stod(val);
            }
            else if (var == "RV") {
                e_beam_args->rv = std::stod(val);
            }
            else if (var == "LENGTH") {
                e_beam_args->length = std::stod(val);
            }
            else if (var == "GAMMA") {
                e_beam_args->gamma = std::stod(val);
            }
            else if (var == "TMP_TR") {
                e_beam_args->tmp_tr = std::stod(val);
            }
            else if (var == "TMP_L") {
                e_beam_args->tmp_l = std::stod(val);
            }
            else if (var == "R_INNER") {
                e_beam_args->r_inner = std::stod(val);
            }
            else if (var == "R_OUTTER") {
                e_beam_args->r_outter = std::stod(val);
            }
            else if (var == "TOTAL_PARTICLE_NUMBER") {
                e_beam_args->n_particle = std::stoi(val);
            }
            else if (var == "LINE_SKIP") {
                e_beam_args->line_skip = std::stoi(val);
            }
            else if (var == "BOX_PARTICLE_NUMBER") {
                e_beam_args->particle_perbox = std::stoi(val);
            }
            else if (var == "BUFFER_SIZE") {
                e_beam_args->buffer = std::stoi(val);
            }
            else {
                assert(false&&"Wrong arguments in section_e_beam!");
            }
        }
        else {
            mupSetExpr(math_parser, val.c_str());
            if(var == "E_NUMBER") {
                e_beam_args->n = mupEval(math_parser);
            }
            else if (var == "RADIUS") {
                e_beam_args->radius = mupEval(math_parser);
            }
            else if (var == "CURRENT") {
                e_beam_args->current = mupEval(math_parser);
            }
            else if (var == "SIGMA_X") {
                e_beam_args->sigma_x = mupEval(math_parser);
            }
            else if (var == "SIGMA_Y") {
                e_beam_args->sigma_y = mupEval(math_parser);
            }
            else if (var == "SIGMA_Z") {
                e_beam_args->sigma_z = mupEval(math_parser);
            }
            else if (var == "RH") {
                e_beam_args->rh = mupEval(math_parser);
            }
            else if (var == "RV") {
                e_beam_args->rv = mupEval(math_parser);
            }
            else if (var == "LENGTH") {
                e_beam_args->length = mupEval(math_parser);
            }
            else if (var == "GAMMA") {
                e_beam_args->gamma = mupEval(math_parser);
            }
            else if (var == "TMP_TR") {
                e_beam_args->tmp_tr = mupEval(math_parser);
            }
            else if (var == "TMP_L") {
                e_beam_args->tmp_l = mupEval(math_parser);
            }
            else if (var == "R_INNER") {
                e_beam_args->r_inner = mupEval(math_parser);
            }
            else if (var == "R_OUTTER") {
                e_beam_args->r_outter = mupEval(math_parser);
            }
            else if (var == "TOTAL_PARTICLE_NUMBER") {
                e_beam_args->n_particle = mupEval(math_parser);
            }
            else if (var == "LINE_SKIP") {
                e_beam_args->line_skip = mupEval(math_parser);
            }
            else if (var == "BOX_PARTICLE_NUMBER") {
                e_beam_args->particle_perbox = mupEval(math_parser);
            }
            else if (var == "BUFFER_SIZE") {
                e_beam_args->buffer = mupEval(math_parser);
            }
            else {
                assert(false&&"Wrong arguments in section_e_beam!");
            }
        }
    }
}

void create_e_beam(Set_ptrs &ptrs) {
    assert(ptrs.e_beam_ptr.get()!=nullptr && "MUST DEFINE THE ELECTRON BEAM BEFORE CREATE THE ELECTRON BEAM!");
    std::string shape = ptrs.e_beam_ptr->shape;
    assert(std::find(E_BEAM_SHAPE_TYPES.begin(),E_BEAM_SHAPE_TYPES.end(),shape)!=E_BEAM_SHAPE_TYPES.end()
           && "WRONG ELECTRON BEAM SHAPE!");
    double gamma = ptrs.e_beam_ptr->gamma;
    double tmp_tr = ptrs.e_beam_ptr->tmp_tr;
    double tmp_l = ptrs.e_beam_ptr->tmp_l;
    assert(gamma>0 && tmp_tr >= 0 && tmp_l >= 0 && "WRONG PARAMETER VALUE FOR ELECTRON BEAM!");

    if (shape == "DC_UNIFORM") {
        double current = ptrs.e_beam_ptr->current;
        double radius = ptrs.e_beam_ptr->radius;
        assert(current >= 0 && radius > 0 && "WRONG PARAMETER VALUE FOR DC_UNIFORM SHAPE");
        ptrs.e_beam.reset(new UniformCylinder(current, radius));
    }
    else if(shape == "BUNCHED_GAUSSIAN") {
        double n = ptrs.e_beam_ptr->n;
        double sigma_x = ptrs.e_beam_ptr->sigma_x;
        double sigma_y = ptrs.e_beam_ptr->sigma_y;
        double sigma_z = ptrs.e_beam_ptr->sigma_z;
        assert(sigma_x > 0 && sigma_y > 0 && sigma_z > 0 && n > 0 && "WRONG PARAMETER VALUE FOR BUNCHED_GAUSSIAN SHAPE");
        ptrs.e_beam.reset(new GaussianBunch(n, sigma_x, sigma_y, sigma_z));
    }
    else if(shape == "BUNCHED_UNIFORM") {
        double current = ptrs.e_beam_ptr->current;
        double radius = ptrs.e_beam_ptr->radius;
        double length = ptrs.e_beam_ptr->length;
        assert(current >= 0 && radius > 0 && length > 0 && "WRONG PARAMETER VALUE FOR BUNCHED_UNIFORM SHAPE");
        ptrs.e_beam.reset(new UniformBunch(current, radius, length));
    }
    else if(shape == "BUNCHED_UNIFORM_ELLIPTIC") {
        double current = ptrs.e_beam_ptr->current;
        double rh = ptrs.e_beam_ptr->rh;
        double rv = ptrs.e_beam_ptr->rv;
        double length = ptrs.e_beam_ptr->length;
        assert(current >= 0 && rh > 0 && rv > 0 && length > 0 && "WRONG PARAMETER VALUE FOR BUNCHED_UNIFORM_ELLIPTIC SHAPE");
        ptrs.e_beam.reset(new EllipticUniformBunch(current, rh, rv, length));
    }
    else if(shape == "DC_UNIFORM_HOLLOW") {
        double current = ptrs.e_beam_ptr->current;
        double r_inner = ptrs.e_beam_ptr->r_inner;
        double r_outter = ptrs.e_beam_ptr->r_outter;
        assert(r_inner>0 && r_outter>0 && current>=0 && r_outter>r_inner && "WRONG PARAMETER VALUE FOR DC_UNIFORM_HOLLOW SHAPE");
        ptrs.e_beam.reset(new UniformHollow(current, r_inner, r_outter));
    }
    else if(shape == "BUNCHED_UNIFORM_HOLLOW") {
        double current = ptrs.e_beam_ptr->current;
        double r_inner = ptrs.e_beam_ptr->r_inner;
        double r_outter = ptrs.e_beam_ptr->r_outter;
        double length = ptrs.e_beam_ptr->length;
        assert(r_inner>0 && r_outter>0 && current>=0 && r_outter>r_inner && length>0 && "WRONG PARAMETER VALUE FOR DC_UNIFORM_HOLLOW SHAPE");
        ptrs.e_beam.reset(new UniformHollowBunch(current, r_inner, r_outter, length));
    }
    else if(shape == "BUNCHED_USER_DEFINED") {
        double n_electron = ptrs.e_beam_ptr->n;
        std::string filename = ptrs.e_beam_ptr->particle_file;
        int line_skip = ptrs.e_beam_ptr->line_skip;
        int n_particle = ptrs.e_beam_ptr->n_particle;
        int s = ptrs.e_beam_ptr->particle_perbox;
        int buffer = ptrs.e_beam_ptr->buffer;
        double length = ptrs.e_beam_ptr->length;
        assert(n_electron>0 && line_skip>=0 && n_particle>=0 && s>0 && length>=0 && buffer>0 && "WRONG PARAMETER VALUE FOR BUNCHED_USER_DEFINED SHAPE");
        if(length>0)
            ptrs.e_beam.reset(new ParticleBunch(n_electron, filename, length));
        else
            ptrs.e_beam.reset(new ParticleBunch(n_electron, filename));
        ParticleBunch* prtl_bunch = nullptr;
        prtl_bunch = dynamic_cast<ParticleBunch*>(ptrs.e_beam.get());
        if(ptrs.e_beam_ptr->binary)
            prtl_bunch->set_binary(ptrs.e_beam_ptr->binary);
        prtl_bunch->set_s(s);
        prtl_bunch->set_skip(line_skip);
        prtl_bunch->set_buffer(buffer);
        if(n_particle>0)
            prtl_bunch->load_particle(n_particle);
        else
            prtl_bunch->load_particle();
        if(ptrs.e_beam_ptr->corr) {
            prtl_bunch->set_corr(true);
        }
    }

    ptrs.e_beam->set_gamma(gamma);
    if(shape != "BUNCHED_USER_DEFINED") {
        ptrs.e_beam->set_tpr(tmp_tr, tmp_l);
    }
    std::cout<<"Electron beam created!"<<std::endl;
}

void define_ion_beam(std::string &str, Set_ion *ion_args){
    assert(ion_args!=nullptr && "SECTION_ION MUST BE CLAIMED!");
    string::size_type idx = str.find("=");
    assert(idx!=string::npos && "WRONG COMMAND IN SECTION_ION!");
    string var = str.substr(0, idx);
    string val = str.substr(idx+1);
    var = trim_blank(var);
    var = trim_tab(var);
    val = trim_blank(val);
    val = trim_tab(val);
    assert(std::find(ION_ARGS.begin(),ION_ARGS.end(),var)!=ION_ARGS.end() && "WRONG COMMANDS IN SECTION_ION!");
//    std::cout<<var<<" "<<val<<std::endl;
    if(math_parser==NULL) {
        if (var=="CHARGE_NUMBER") {
            ion_args->n_charge = std::stoi(val);
        }
        else if (var=="MASS") {
            ion_args->mass = std::stod(val);
        }
        else if (var=="KINETIC_ENERGY") {
            ion_args->k_energy = std::stod(val);
        }
        else if (var=="NORM_EMIT_X") {
            ion_args->emit_nx = std::stod(val);
        }
        else if (var=="NORM_EMIT_Y") {
            ion_args->emit_ny = std::stod(val);
        }
        else if (var=="MOMENTUM_SPREAD") {
            ion_args->dp_p = std::stod(val);
        }
        else if (var=="PARTICLE_NUMBER") {
            ion_args->n_ptcl = std::stod(val);
        }
        else if (var=="RMS_BUNCH_LENGTH") {
            ion_args->ds = std::stod(val);
        }
        else {
            assert(false&&"Wrong arguments in section_ion!");
        }
    }
    else {
        mupSetExpr(math_parser, val.c_str());
        if (var=="CHARGE_NUMBER") {
            ion_args->n_charge = static_cast<int>(mupEval(math_parser));
        }
        else if (var=="MASS") {
            ion_args->mass = mupEval(math_parser);
        }
        else if (var=="KINETIC_ENERGY") {
            ion_args->k_energy = mupEval(math_parser);
        }
        else if (var=="NORM_EMIT_X") {
            ion_args->emit_nx = mupEval(math_parser);
        }
        else if (var=="NORM_EMIT_Y") {
            ion_args->emit_ny = mupEval(math_parser);
        }
        else if (var=="MOMENTUM_SPREAD") {
            ion_args->dp_p = mupEval(math_parser);
        }
        else if (var=="PARTICLE_NUMBER") {
            ion_args->n_ptcl = mupEval(math_parser);
        }
        else if (var=="RMS_BUNCH_LENGTH") {
            ion_args->ds = mupEval(math_parser);
        }
        else {
            assert(false&&"Wrong arguments in section_ion!");
        }
    }
}

void create_ion_beam(Set_ptrs &ptrs) {
    int n_charge = ptrs.ion_ptr->n_charge;
    double mass = ptrs.ion_ptr->mass;
    double k_energy = ptrs.ion_ptr->k_energy;
    double emit_nx = ptrs.ion_ptr->emit_nx;
    double emit_ny = ptrs.ion_ptr->emit_ny;
    double dp_p = ptrs.ion_ptr->dp_p;
    double n_ptcl = ptrs.ion_ptr->n_ptcl;
    double ds = ptrs.ion_ptr->ds;
    assert(n_charge>0 && mass>0 && k_energy>0 && emit_nx>0 && emit_ny>0 && dp_p>=0 && n_ptcl>0
           && "WRONG PARAMETER VALUE FOR ION BEAM!");
    if (ds>0) {
        ptrs.ion_beam.reset(new Beam(n_charge, mass/k_u, k_energy, emit_nx, emit_ny, dp_p, ds, n_ptcl));
        std::cout<<"Bunched ion beam created!"<<std::endl;
    }
    else {
        ptrs.ion_beam.reset(new Beam(n_charge, mass/k_u, k_energy, emit_nx, emit_ny, dp_p, n_ptcl));
        std::cout<<"Coasting ion beam created!"<<std::endl;
    }
    uircd.emit_nx = emit_nx;
    uircd.emit_ny = emit_ny;
    uircd.dp_p = dp_p;
    if(ds>0) uircd.sigma_s = ds;
    else uircd.sigma_s = 0;
}

void create_ring(Set_ptrs &ptrs) {
    string lattice_file = ptrs.ring_ptr->lattice_file;
    assert(!lattice_file.empty() && "WRONG PARAMETER VALUE FOR RING");
    ptrs.lattice.reset(new Lattice(lattice_file));
    assert(ptrs.ion_beam.get()!=nullptr && "MUST DEFINE THE ION BEFORE CREATE THE RING!");
    ptrs.ring.reset(new Ring(*ptrs.lattice, *ptrs.ion_beam));

    if(ptrs.ring_ptr->qx>0 || ptrs.ring_ptr->qy>0 || ptrs.ring_ptr->qs>0) {
        assert(ptrs.ring_ptr->qx>0 && ptrs.ring_ptr->qy>0 && "Transverse tunes must be greater than zero!");
        ptrs.ring->tunes.qx = ptrs.ring_ptr->qx;
        ptrs.ring->tunes.qy = ptrs.ring_ptr->qy;
        ptrs.ring->tunes.qs = ptrs.ring_ptr->qs;
    }

    ptrs.ring->rf.v = ptrs.ring_ptr->rf_v;
    ptrs.ring->rf.h = ptrs.ring_ptr->rf_h;
    ptrs.ring->rf.phi = ptrs.ring_ptr->rf_phi;
    ptrs.ring->rf.gamma_tr = ptrs.ring_ptr->gamma_tr;

    if(ptrs.ring_ptr->rf_v>0) {
        assert(ptrs.ring_ptr->gamma_tr>0 && "When RF cavity is defined, the transition gamma should be greater than zero");
    }
    ptrs.ring->set_rf();
    std::cout<<"Ring created!"<<std::endl;
}

void create_cooler(Set_ptrs &ptrs) {
    double length = ptrs.cooler_ptr->length;
    int section_number = ptrs.cooler_ptr->section_number;
    double magnetic_field = ptrs.cooler_ptr->magnetic_field;
    double bet_x = ptrs.cooler_ptr->bet_x;
    double bet_y = ptrs.cooler_ptr->bet_y;
    double disp_x = ptrs.cooler_ptr->disp_x;
    double disp_y = ptrs.cooler_ptr->disp_y;
    double alpha_x = ptrs.cooler_ptr->alpha_x;
    double alpha_y = ptrs.cooler_ptr->alpha_y;
    double disp_dx = ptrs.cooler_ptr->disp_dx;
    double disp_dy = ptrs.cooler_ptr->disp_dy;
    assert(length>0 && section_number>0 && bet_x>0 && bet_y>0 && "WRONG PARAMETER VALUE FOR COOLER!");
    ptrs.cooler.reset(new Cooler(length, section_number, magnetic_field, bet_x, bet_y, disp_x, disp_y, alpha_x, alpha_y,
                                 disp_dx, disp_dy));
    std::cout<<"Cooler created!"<<std::endl;
}

void calculate_ibs(Set_ptrs &ptrs, bool calc = true) {
    assert(ptrs.ring.get()!=nullptr && "MUST CREATE THE RING BEFORE CALCULATING IBS RATE!");
    assert(ptrs.ibs_ptr.get()!=nullptr && "PLEASE SET UP THE PARAMETERS FOR IBS RATE CALCULATION!");

    const int nu = ptrs.ibs_ptr->nu;
    const int nv = ptrs.ibs_ptr->nv;
    const int nz = ptrs.ibs_ptr->nz;
    const double log_c = ptrs.ibs_ptr->log_c;
    const double k = ptrs.ibs_ptr->coupling;
    double rx, ry, rz;
    IBSModel model = ptrs.ibs_ptr->model;
    bool ibs_by_element = ptrs.ibs_ptr->ibs_by_element;


    if(model == IBSModel::MARTINI) {
        assert(nu>0 && nv>0 && (k <= 1) && (k >= 0) && ((log_c > 0) || (nz > 0)) && "WRONG PARAMETER VALUE FOR IBS RATE CALCULATION!");
        ibs_solver.reset(new IBSSolver_Martini(nu, nv, nz, log_c, k));
//        delete ibs_solver;
//        ibs_solver = new IBSSolver_Martini(nu, nv, nz, log_c, k);
        ibs_solver->set_ibs_by_element(ibs_by_element);
    } else if (model == IBSModel::BM) {
        assert(log_c>0 && "WRONG VALUE FOR COULOMB LOGARITHM IN IBS CALCULATION WITH BM MODEL!");
        ibs_solver.reset(new IBSSolver_BM(log_c, k));
//        delete ibs_solver;
//        ibs_solver = new IBSSolver_BM(log_c, k);
        ibs_solver->set_ibs_by_element(ibs_by_element);
    }
    if(calc) {
        ibs_solver->rate(ptrs.ring->lattice(), *ptrs.ion_beam, rx, ry, rz);
//        ibs_solver->rate(*ptrs.ring->lattice_, *ptrs.ion_beam, rx, ry, rz);

        ptrs.ibs_rate.at(0) = rx;
        ptrs.ibs_rate.at(1) = ry;
        ptrs.ibs_rate.at(2) = rz;
        uircd.rx_ibs = rx;
        uircd.ry_ibs = ry;
        uircd.rs_ibs = rz;
        std::cout<<std::scientific;
        std::cout << std::setprecision(3);
        std::cout<<"IBS rate (1/s): "<<rx<<"  "<<ry<<"  "<<rz<<std::endl;
    }
}

void calculate_ecool(Set_ptrs &ptrs, bool calc = true) {
    assert(ptrs.cooler.get()!=nullptr && "MUST CREATE THE COOLER BEFORE CALCULATE ELECTRON COOLING RATE!");
    assert(ptrs.e_beam.get()!=nullptr && "MUST CREATE THE ELECTRON BEAM BEFORE CALCULATE ELECTRON COOLING RATE!");
    assert(ptrs.ecool_ptr.get()!=nullptr && "PLEASE SET UP THE PARAMETERS FOR ELECTRON COOLING RATE CALCULATION!");
    int n_sample = ptrs.ecool_ptr->n_sample;
    assert(n_sample > 0 && "WRONG PARAMETER VALUE FOR ELECTRON COOLING RATE CALCULATION!");
    assert(ptrs.ion_beam.get()!=nullptr && "MUST CREATE THE ION BEAM BEFORE CALCULATE ELECTRON COOLING RATE!");
    assert(ptrs.ring.get()!=nullptr && "MUST CREATE THE RING BEFORE CALCULATE ELECTRON COOLING RATE!");
    double rx, ry, rz;

    ecool_solver.reset(new ECoolRate());

    switch(ptrs.ecool_ptr->force) {
        case ForceFormula::PARKHOMCHUK: {
//            force_solver = new Force_Park();
            force_solver.reset(new Force_Park());
            break;
        }
        default: {
            assert(false&&"WRONG FRICTION FORCE FORMULA SELECTED!");
        }
    }

//    ion_sample = new Ions_MonteCarlo(n_sample);
    ion_sample.reset(new Ions_MonteCarlo(n_sample));
    ion_sample->set_twiss(*ptrs.cooler);
    ion_sample->create_samples(*ptrs.ion_beam);

    if(calc) {
        ecool_solver->ecool_rate(*force_solver, *ptrs.ion_beam, *ion_sample, *ptrs.cooler, *ptrs.e_beam, *ptrs.ring, rx, ry, rz);
        ptrs.ecool_rate.at(0) = rx;
        ptrs.ecool_rate.at(1) = ry;
        ptrs.ecool_rate.at(2) = rz;
        uircd.rx_ecool = rx;
        uircd.ry_ecool = ry;
        uircd.rs_ecool = rz;
        std::cout<<std::scientific;
        std::cout << std::setprecision(3);
        std::cout<<"Electron cooling rate (1/s): "<<rx<<"  "<<ry<<"  "<<rz<<std::endl;
    }
}
//
//void calculate_ecool(Set_ptrs &ptrs) {
//    assert(ptrs.cooler.get()!=nullptr && "MUST CREATE THE COOLER BEFORE CALCULATE ELECTRON COOLING RATE!");
//    assert(ptrs.e_beam.get()!=nullptr && "MUST CREATE THE ELECTRON BEAM BEFORE CALCULATE ELECTRON COOLING RATE!");
//    assert(ptrs.ecool_ptr.get()!=nullptr && "PLEASE SET UP THE PARAMETERS FOR ELECTRON COOLING RATE CALCULATION!");
//    int n_sample = ptrs.ecool_ptr->n_sample;
//    assert(n_sample > 0 && "WRONG PARAMETER VALUE FOR ELECTRON COOLING RATE CALCULATION!");
//    EcoolRateParas ecool_paras(n_sample);
//
//    ForceParas force_paras(ptrs.ecool_ptr->force);
//    assert(ptrs.ion_beam.get()!=nullptr && "MUST CREATE THE ION BEAM BEFORE CALCULATE ELECTRON COOLING RATE!");
//    assert(ptrs.ring.get()!=nullptr && "MUST CREATE THE RING BEFORE CALCULATE ELECTRON COOLING RATE!");
//    double rx, ry, rz;
//    ecooling_rate(ecool_paras, force_paras, *ptrs.ion_beam, *ptrs.cooler, *ptrs.e_beam, *ptrs.ring, rx, ry, rz);
//    ptrs.ecool_rate.at(0) = rx;
//    ptrs.ecool_rate.at(1) = ry;
//    ptrs.ecool_rate.at(2) = rz;
//    vl_rx_ecool = rx;
//    vl_ry_ecool = ry;
//    vl_rs_ecool = rz;
//    std::cout<<std::scientific;
//    std::cout << std::setprecision(3);
//    std::cout<<"Electron cooling rate (1/s): "<<rx<<"  "<<ry<<"  "<<rz<<std::endl;
//}

void total_expansion_rate(Set_ptrs &ptrs) {
    if (std::all_of(ptrs.ibs_rate.begin(), ptrs.ibs_rate.end(), [](double i) { return i==0; })) calculate_ibs(ptrs);
    if (std::all_of(ptrs.ecool_rate.begin(), ptrs.ecool_rate.end(), [](double i) { return i==0; })) calculate_ecool(ptrs);
    for(int i=0; i<3; ++i) ptrs.total_rate.at(i) = ptrs.ecool_rate.at(i) + ptrs.ibs_rate.at(i);
    std::cout<<std::scientific;
    std::cout << std::setprecision(3);
    std::cout<<"Total expansion rate (1/s): "<<ptrs.total_rate.at(0)<<"  "<<ptrs.total_rate.at(1)<<"  "
        <<ptrs.total_rate.at(2)<<std::endl;
    uircd.rx_total = ptrs.total_rate.at(0);
    uircd.ry_total = ptrs.total_rate.at(1);
    uircd.rs_total = ptrs.total_rate.at(2);
}

void calculate_luminosity(Set_ptrs &ptrs, bool calc=true) {
    assert(ptrs.luminosity_ptr.get()!=nullptr && "MUST SET UP THE PARAMETERS FOR LUMINOSITY CALCULATION.");
//    lum_solver = new LuminositySolver();
    lum_solver.reset(new LuminositySolver());
    lum_solver->set_use_ion_emit(ptrs.luminosity_ptr->use_ion_emittance);
//    Luminosity lum;
//    lum.set_use_ion_emit(ptrs.luminosity_ptr->use_ion_emittance);
    if(ptrs.luminosity_ptr->use_ion_emittance) {
        assert(ptrs.ion_beam.get()!=nullptr && "CREATE THE ION BEAM IF IT IS USED IN THE LUMINOSITY CALCULATION.");
        double geo_emit_x = ptrs.ion_beam->emit_x();
        double geo_emit_y = ptrs.ion_beam->emit_y();
        double bet_x = ptrs.luminosity_ptr->bet_x1;
        double bet_y = ptrs.luminosity_ptr->bet_y1;
        assert(bet_x>0 && bet_y>0 &&  "WRONG VALUE FOR LUMINOSITY: BETA FUNCTIONS OF BEAM 1 SHOULD BE POSITIVE.");
        lum_solver->set_bet_star(bet_x, bet_y, 0);
        lum_solver->set_geo_emit(geo_emit_x, geo_emit_y, 0);
    }
    else {
        double sigma_x = ptrs.luminosity_ptr->sigma_x1;
        double sigma_y = ptrs.luminosity_ptr->sigma_y1;
        double bet_x = ptrs.luminosity_ptr->bet_x1;
        double bet_y = ptrs.luminosity_ptr->bet_y1;
        double geo_emit_x = ptrs.luminosity_ptr->geo_emit_x1;
        double geo_emit_y = ptrs.luminosity_ptr->geo_emit_y1;
        assert((sigma_x>0 && sigma_y>0) || (geo_emit_x>0 && geo_emit_y>0 && bet_x>0 && bet_y>0)  &&
               "WRONG VALUE FOR LUMINOSITY: SIZE OR EMITTANCE (w. BETA FUNCTIONS) OF BEAM 1 SHOULD BE POSITIVE.");
        if(sigma_x>0 && sigma_y>0) {
            lum_solver->set_beam_size(sigma_x, sigma_y, 0);
        }
        else {
            lum_solver->set_bet_star(bet_x, bet_y, 0);
            lum_solver->set_geo_emit(geo_emit_x, geo_emit_y, 0);
        }

    }
    double bet_x = ptrs.luminosity_ptr->bet_x2;
    double bet_y = ptrs.luminosity_ptr->bet_y2;
    double sigma_x = ptrs.luminosity_ptr->sigma_x2;
    double sigma_y = ptrs.luminosity_ptr->sigma_y2;
    double geo_emit_x = ptrs.luminosity_ptr->geo_emit_x2;
    double geo_emit_y = ptrs.luminosity_ptr->geo_emit_y2;
    assert((sigma_x>0 && sigma_y>0) || (geo_emit_x>0 && geo_emit_y>0 && bet_x>0 && bet_y>0) &&
           "WRONG VALUE FOR LUMINOSITY: SIZE OR EMITTANCE (w. BETA FUNCTIONS) OF BEAM 2 SHOULD BE POSITIVE.");
    if(sigma_x>0 && sigma_y>0) {
        lum_solver->set_beam_size(sigma_x, sigma_y, 1);
    }
    else {
        lum_solver->set_bet_star(bet_x, bet_y, 1);
        lum_solver->set_geo_emit(geo_emit_x, geo_emit_y, 1);
    }
    lum_solver->set_distance(ptrs.luminosity_ptr->dx, ptrs.luminosity_ptr->dy);
    double np_1 = ptrs.luminosity_ptr->np_1;
    double np_2 = ptrs.luminosity_ptr->np_2;
    assert(np_1>0 && np_2>0 && "WRONG VALUE FOR LUMINOSITY: PARTICLE NUMBERS SHOULD BE POSTIVE.");
    lum_solver->set_particle_number(np_1, 0);
    lum_solver->set_particle_number(np_2, 1);
    double f = ptrs.luminosity_ptr->freq;
    assert(f>0 && "WRONG VALUE FOR LUMINOSITY: COLLISION FREQUENCY SHOULD BE POSITIVE.");
    lum_solver->set_freq(f);

    if(calc) {
        std::cout<<std::scientific;
        std::cout << std::setprecision(3);
        std::cout<<"Luminosity(1/s*1/cm^2) :"<<lum_solver->luminosity()*10000<<std::endl;
    }
}
//
//void calculate_luminosity(Set_ptrs &ptrs) {
//    assert(ptrs.luminosity_ptr.get()!=nullptr && "MUST SET UP THE PARAMETERS FOR LUMINOSITY CALCULATION.");
//    Luminosity lum;
//    lum.set_use_ion_emit(ptrs.luminosity_ptr->use_ion_emittance);
//    if(ptrs.luminosity_ptr->use_ion_emittance) {
//        assert(ptrs.ion_beam.get()!=nullptr && "CREATE THE ION BEAM IF IT IS USED IN THE LUMINOSITY CALCULATION.");
//        double geo_emit_x = ptrs.ion_beam->emit_x();
//        double geo_emit_y = ptrs.ion_beam->emit_y();
//        double bet_x = ptrs.luminosity_ptr->bet_x1;
//        double bet_y = ptrs.luminosity_ptr->bet_y1;
//        assert(bet_x>0 && bet_y>0 &&  "WRONG VALUE FOR LUMINOSITY: BETA FUNCTIONS OF BEAM 1 SHOULD BE POSITIVE.");
//        lum.set_bet(bet_x, bet_y, 1);
//        lum.set_geo_emit(geo_emit_x, geo_emit_y, 1);
//    }
//    else {
//        double sigma_x = ptrs.luminosity_ptr->sigma_x1;
//        double sigma_y = ptrs.luminosity_ptr->sigma_y1;
//        double bet_x = ptrs.luminosity_ptr->bet_x1;
//        double bet_y = ptrs.luminosity_ptr->bet_y1;
//        double geo_emit_x = ptrs.luminosity_ptr->geo_emit_x1;
//        double geo_emit_y = ptrs.luminosity_ptr->geo_emit_y1;
//        assert((sigma_x>0 && sigma_y>0) || (geo_emit_x>0 && geo_emit_y>0 && bet_x>0 && bet_y>0)  &&
//               "WRONG VALUE FOR LUMINOSITY: SIZE OR EMITTANCE (w. BETA FUNCTIONS) OF BEAM 1 SHOULD BE POSITIVE.");
//        if(sigma_x>0 && sigma_y>0) {
//            lum.set_beam_size(sigma_x, sigma_y, 1);
//        }
//        else {
//            lum.set_bet(bet_x, bet_y, 1);
//            lum.set_geo_emit(geo_emit_x, geo_emit_y, 1);
//        }
//
//    }
//    double bet_x = ptrs.luminosity_ptr->bet_x2;
//    double bet_y = ptrs.luminosity_ptr->bet_y2;
//    double sigma_x = ptrs.luminosity_ptr->sigma_x2;
//    double sigma_y = ptrs.luminosity_ptr->sigma_y2;
//    double geo_emit_x = ptrs.luminosity_ptr->geo_emit_x2;
//    double geo_emit_y = ptrs.luminosity_ptr->geo_emit_y2;
//    assert((sigma_x>0 && sigma_y>0) || (geo_emit_x>0 && geo_emit_y>0 && bet_x>0 && bet_y>0) &&
//           "WRONG VALUE FOR LUMINOSITY: SIZE OR EMITTANCE (w. BETA FUNCTIONS) OF BEAM 2 SHOULD BE POSITIVE.");
//    if(sigma_x>0 && sigma_y>0) {
//        lum.set_beam_size(sigma_x, sigma_y, 2);
//    }
//    else {
//        lum.set_bet(bet_x, bet_y, 2);
//        lum.set_geo_emit(geo_emit_x, geo_emit_y, 2);
//    }
//    lum.set_distance(ptrs.luminosity_ptr->dx, ptrs.luminosity_ptr->dy);
//    double np_1 = ptrs.luminosity_ptr->np_1;
//    double np_2 = ptrs.luminosity_ptr->np_2;
//    assert(np_1>0 && np_2>0 && "WRONG VALUE FOR LUMINOSITY: PARTICLE NUMBERS SHOULD BE POSTIVE.");
//    lum.set_particle_number(np_1, 1);
//    lum.set_particle_number(np_2, 2);
//    double f = ptrs.luminosity_ptr->freq;
//    assert(f>0 && "WRONG VALUE FOR LUMINOSITY: COLLISION FREQUENCY SHOULD BE POSITIVE.");
//    lum.set_freq(f);
//
//    std::cout<<std::scientific;
//    std::cout << std::setprecision(3);
//    std::cout<<"Luminosity(1/s*1/cm^2) :"<<lum.luminosity()*10000<<std::endl;
//}

void run_simulation(Set_ptrs &ptrs) {
    assert(ptrs.dynamic_ptr.get()!=nullptr && "PLEASE SET UP THE PARAMETERS FOR SIMULATION!");
    bool ibs = ptrs.dynamic_ptr->ibs;
    bool ecool = ptrs.dynamic_ptr->ecool;
    bool fixed_bunch_length = ptrs.dynamic_ptr->fixed_bunch_length;

    if(fixed_bunch_length) {
        assert(ptrs.dynamic_ptr->model==DynamicModel::RMS&&"ERROR: THE PARAMETER FIXED_BUNCH_LENGTH WORKS ONLY FOR RMS MODEL");
        assert(ptrs.ring->rf.gamma_tr>0&&"ERROR: DEFINE THE TRANSITION GAMMA OF THE RING WHEN THE PARAMETER FIXED_BUNCH_LENGTH IS CHOSEN");
    }
    double t = ptrs.dynamic_ptr->time;
    double t0 = ptrs.dynamic_ptr->t0;
    int n_step = ptrs.dynamic_ptr->n_step;
    int n_sample = ptrs.dynamic_ptr->n_sample;
    int output_intvl = ptrs.dynamic_ptr->output_intvl;
    int save_ptcl_intvl = ptrs.dynamic_ptr->save_ptcl_intvl;

    if(ptrs.dynamic_ptr->model == DynamicModel::TURN_BY_TURN) {
        double dt = ptrs.ring->circ()/(ptrs.ion_beam->beta()*k_c);
        if (n_step>0) {
            t = n_step * dt;
        }
        else if (t>0) {
            n_step = ceil(t/dt);
            t = n_step * dt;
        }
    }

    assert(t>0 && n_step>0 && output_intvl>0 && "WRONG PARAMETERS FOR SIMULAITON!");
    switch(ptrs.dynamic_ptr->model){
        case DynamicModel::RMS: {
            simulator.reset(new RMSModel(t, n_step));
            break;
        }
        case DynamicModel::PARTICLE: {
            simulator.reset(new ParticleModel(t,n_step));
            break;
        }
        case DynamicModel::TURN_BY_TURN:{
            simulator.reset(new TurnByTurnModel(t, n_step));
            break;
        }
        default: {
            assert(false&&"WRONG DYNAMIC MODEL SELECTED!");
        }
    }

    simulator->set_fixed_bunch_length(fixed_bunch_length);
    if (output_intvl>1) simulator->set_output_intvl(output_intvl);
    if (save_ptcl_intvl>0) simulator->set_ion_save(save_ptcl_intvl);
    simulator->set_output_file(ptrs.dynamic_ptr->filename);
    simulator->set_reset_time(ptrs.dynamic_ptr->reset_time?true:false);
    simulator->set_overwrite(ptrs.dynamic_ptr->overwrite?true:false);
    simulator->set_calc_lum(ptrs.dynamic_ptr->calc_luminosity?true:false);
    simulator->set_ibs(ibs);
    simulator->set_ecool(ecool);
    simulator->set_ini_time(t0);

    if (ptrs.dynamic_ptr->calc_luminosity) {
            assert(ptrs.luminosity_ptr.get()!=nullptr && "PLEASE SET UP THE PARAMETERS FOR LUMINOSITY CALCULATION!");
            calculate_luminosity(ptrs, false);
    }

    if(n_sample>0) {
        ptrs.ecool_ptr->n_sample = n_sample;
    }

    assert(ptrs.ring.get()!=nullptr && "MUST CREATE THE RING BEFORE SIMULATION!");
    if(ecool) {
        assert(ptrs.ecool_ptr.get()!=nullptr && "PLEASE SET UP THE ECOOL PARAMETERS FOR ECOOL CALCULATION!");
        calculate_ecool(ptrs, false);
    }

    if(ibs) {
        assert(ptrs.ibs_ptr.get()!=nullptr && "PLEASE SET UP THE PARAMETERS FOR IBS RATE CALCULATION!");
        calculate_ibs(ptrs, false);
    }

    if (ibs && !ecool && ptrs.dynamic_ptr->model!=DynamicModel::RMS) {
        assert(ptrs.dynamic_ptr->ref_bet_x>0 && ptrs.dynamic_ptr->ref_bet_y>0 && "WRONG VALUE FOR REFERENCE TWISS PARAMETERS");
        assert(n_sample>0 && "SET N_SAMPLE FOR SIMULATION!");
        ion_sample.reset(new Ions_MonteCarlo(n_sample));
        ion_sample->set_twiss(ptrs.dynamic_ptr->twiss_ref);
        ion_sample->create_samples(*ptrs.ion_beam);
    }

    simulator->run(*ptrs.ion_beam, *ion_sample, *ptrs.cooler, *ptrs.e_beam, *ptrs.ring,
                   ibs_solver.get(), ecool_solver.get(), force_solver.get(), lum_solver.get());
}

void run(std::string &str, Set_ptrs &ptrs) {
    str = trim_blank(str);
    str = trim_tab(str);
    assert(std::find(RUN_COMMANDS.begin(),RUN_COMMANDS.end(),str)!=RUN_COMMANDS.end() && "WRONG COMMANDS IN SECTION_RUN!");
    if (str == "CREATE_ION_BEAM") {
        create_ion_beam(ptrs);
    }
    else if(str == "CREATE_RING") {
        create_ring(ptrs);
    }
    else if(str == "CALCULATE_IBS") {
        calculate_ibs(ptrs);
    }
    else if(str == "CREATE_COOLER") {
        create_cooler(ptrs);
    }
    else if(str == "CREATE_E_BEAM") {
        create_e_beam(ptrs);
    }
    else if(str == "CALCULATE_ECOOL") {
        calculate_ecool(ptrs);
    }
    else if(str == "CALCULATE_LUMINOSITY") {
        calculate_luminosity(ptrs);
    }
    else if(str == "TOTAL_EXPANSION_RATE") {
        total_expansion_rate(ptrs);
    }
    else if(str == "RUN_SIMULATION") {
        run_simulation(ptrs);
    }
    else if (str.substr(0,5) == "SRAND") {
        string var = str.substr(6);
        var = trim_blank(var);
        var = trim_tab(var);
        mupSetExpr(math_parser, var.c_str());
        srand(mupEval(math_parser));
    }
    else {
        assert(false&&"Wrong arguments in section_run!");
    }
}

void set_section_run(Set_ptrs &ptrs) {
    std::fill(ptrs.ibs_rate.begin(), ptrs.ibs_rate.end(), 0);
    std::fill(ptrs.ecool_rate.begin(), ptrs.ecool_rate.end(), 0);
    std::fill(ptrs.total_rate.begin(), ptrs.total_rate.end(), 0);
}

void define_ring(string &str, Set_ring *ring_args) {
    assert(ring_args!=nullptr && "SECTION_RING MUST BE CLAIMED!");
    string::size_type idx = str.find("=");
    assert(idx!=string::npos && "WRONG COMMAND IN SECTION_RING!");
    string var = str.substr(0, idx);
    string val = str.substr(idx+1);
    var = trim_blank(var);
    var = trim_tab(var);
    val = trim_blank(val);
    val = trim_tab(val);
    str_toupper(var);
    assert(std::find(RING_ARGS.begin(),RING_ARGS.end(),var)!=RING_ARGS.end() && "WRONG COMMANDS IN SECTION_RING!");
    if (var=="LATTICE") {
        ring_args->lattice_file = val;
    }
    else {
       str_toupper(val);
       if(math_parser==NULL) {
            if (var=="QX") {
                ring_args->qx = std::stod(val);
            }
            else if (var=="QY") {
                ring_args->qy = std::stod(val);
            }
            else if (var=="QS") {
                ring_args->qs = std::stod(val);
            }
            else if (var=="GAMMA_TR") {
                ring_args->gamma_tr = std::stod(val);
            }
            else if (var=="RF_V") {
                ring_args->rf_v = std::stod(val);
            }
            else if (var=="RF_H") {
                ring_args->rf_h = std::stoi(val);
            }
            else if (var=="RF_PHI") {
                ring_args->rf_phi = std::stod(val)*2*k_pi;
            }
            else {
                assert(false&&"Wrong arguments in section_ring!");
            }
       }
       else {
            mupSetExpr(math_parser, val.c_str());
            if (var=="QX") {
                ring_args->qx = mupEval(math_parser);
            }
            else if (var=="QY") {
                ring_args->qy = mupEval(math_parser);
            }
            else if (var=="QS") {
                ring_args->qs = mupEval(math_parser);
            }
            else if (var=="GAMMA_TR") {
                ring_args->gamma_tr = mupEval(math_parser);
            }
            else if (var=="RF_V") {
                ring_args->rf_v = mupEval(math_parser);
            }
            else if (var=="RF_H") {
                ring_args->rf_h = static_cast<int>(mupEval(math_parser));
            }
            else if (var=="RF_PHI") {
                ring_args->rf_phi = mupEval(math_parser)*2*k_pi;
            }
            else {
                assert(false&&"Wrong arguments in section_ring!");
            }
       }

    }
}

void define_cooler(std::string &str, Set_cooler *cooler_args) {
    assert(cooler_args!=nullptr && "SECTION_COOLER MUST BE CLAIMED!");
    string::size_type idx = str.find("=");
    assert(idx!=string::npos && "WRONG COMMAND IN SECTION_RING!");
    string var = str.substr(0, idx);
    string val = str.substr(idx+1);
    var = trim_blank(var);
    var = trim_tab(var);
    val = trim_blank(val);
    val = trim_tab(val);
    assert(std::find(COOLER_ARGS.begin(),COOLER_ARGS.end(),var)!=COOLER_ARGS.end() && "WRONG COMMANDS IN SECTION_RING!");
    if(math_parser==NULL) {
        if (var=="LENGTH") {
            cooler_args->length = std::stod(val);
        }
        else if (var == "SECTION_NUMBER") {
            cooler_args->section_number = std::stoi(val);
        }
        else if (var == "MAGNETIC_FIELD") {
            cooler_args->magnetic_field = std::stod(val);
        }
        else if (var == "BET_X") {
            cooler_args->bet_x = std::stod(val);
        }
        else if (var == "BET_Y") {
            cooler_args->bet_y = std::stod(val);
        }
        else if (var == "DISP_X") {
            cooler_args->disp_x = std::stod(val);
        }
        else if (var == "DISP_Y") {
            cooler_args->disp_y = std::stod(val);
        }
        else if (var == "ALPHA_X") {
            cooler_args->alpha_x = std::stod(val);
        }
        else if (var == "ALPHA_Y") {
            cooler_args->alpha_y = std::stod(val);
        }
        else if (var == "DISP_DX") {
            cooler_args->disp_dx = std::stod(val);
        }
        else if (var == "DISP_DY") {
            cooler_args->disp_dy = std::stod(val);
        }
        else {
            assert(false&&"Wrong arguments in section_cooler!");
        }
    }
    else {
        mupSetExpr(math_parser, val.c_str());
        if (var=="LENGTH") {
            cooler_args->length = mupEval(math_parser);
        }
        else if (var == "SECTION_NUMBER") {
            cooler_args->section_number = static_cast<int>(mupEval(math_parser));
        }
        else if (var == "MAGNETIC_FIELD") {
            cooler_args->magnetic_field = mupEval(math_parser);
        }
        else if (var == "BET_X") {
            cooler_args->bet_x = mupEval(math_parser);
        }
        else if (var == "BET_Y") {
            cooler_args->bet_y = mupEval(math_parser);
        }
        else if (var == "DISP_X") {
            cooler_args->disp_x = mupEval(math_parser);
        }
        else if (var == "DISP_Y") {
            cooler_args->disp_y = mupEval(math_parser);
        }
        else if (var == "ALPHA_X") {
            cooler_args->alpha_x = mupEval(math_parser);
        }
        else if (var == "ALPHA_Y") {
            cooler_args->alpha_y = mupEval(math_parser);
        }
        else if (var == "DISP_DX") {
            cooler_args->disp_dx = mupEval(math_parser);
        }
        else if (var == "DISP_DY") {
            cooler_args->disp_dy = mupEval(math_parser);
        }
        else {
            assert(false&&"Wrong arguments in section_cooler!");
        }
    }
}

void set_ibs(string &str, Set_ibs *ibs_args) {
    assert(ibs_args!=nullptr && "SECTION_IBS MUST BE CLAIMED!");
    string::size_type idx = str.find("=");
    assert(idx!=string::npos && "WRONG COMMAND IN SECTION_IBS!");
    string var = str.substr(0, idx);
    string val = str.substr(idx+1);
    var = trim_blank(var);
    var = trim_tab(var);
    val = trim_blank(val);
    val = trim_tab(val);

    assert(std::find(IBS_ARGS.begin(),IBS_ARGS.end(),var)!=IBS_ARGS.end() && "WRONG COMMANDS IN SECTION_IBS!");
    if (var== "MODEL") {
        if (val == "MARTINI") {
            ibs_args->model = IBSModel::MARTINI;
        }
        else if(val == "BM") {
            ibs_args->model = IBSModel::BM;
        }
        else {
            assert(false&&"WRONG IBS MODEL!");
        }
    }
    else if (var == "IBS_BY_ELEMENT") {
        if (val == "ON" || val == "TRUE") ibs_args->ibs_by_element = true;
        else if (val == "OFF" || val == "FALSE") ibs_args->ibs_by_element = false;
        else assert(false&&"WRONG VALUE FOR THE PARAMETER IBS_BY_ELEMENT IN SECTION_IBS!");
    }
    else if (math_parser == NULL) {
        if (var == "NU") {
            ibs_args->nu = std::stoi(val);
        }
        else if(var == "NV") {
            ibs_args->nv = std::stoi(val);
        }
        else if(var == "NZ") {
            ibs_args->nz = std::stoi(val);
        }
        else if(var == "LOG_C") {
            ibs_args->log_c = std::stod(val);
        }
        else if(var == "COUPLING") {
            ibs_args->coupling = std::stod(val);
        }
        else {
            assert(false&&"Wrong arguments in section_ibs!");
        }
    }
    else {
        mupSetExpr(math_parser, val.c_str());
        if (var == "NU") {
            ibs_args->nu = static_cast<int>(mupEval(math_parser));
        }
        else if(var == "NV") {
            ibs_args->nv = static_cast<int>(mupEval(math_parser));
        }
        else if(var == "NZ") {
            ibs_args->nz = static_cast<int>(mupEval(math_parser));
        }
        else if(var == "LOG_C") {
            ibs_args->log_c = mupEval(math_parser);
        }
        else if(var == "COUPLING") {
            ibs_args->coupling = mupEval(math_parser);
        }
        else {
            assert(false&&"Wrong arguments in section_ibs!");
        }
    }

}

void set_simulation(string &str, Set_dynamic *dynamic_args) {
    assert(dynamic_args!=nullptr && "SECTION_SIMULATION MUST BE CLAIMED!");
    string::size_type idx = str.find("=");
    assert(idx!=string::npos && "WRONG COMMAND IN SECTION_SIMULATION!");
    string var = str.substr(0, idx);
    string val = str.substr(idx+1);
    var = trim_blank(var);
    var = trim_tab(var);
    val = trim_blank(val);
    val = trim_tab(val);
    assert(std::find(SIMULATION_ARGS.begin(),SIMULATION_ARGS.end(),var)!=SIMULATION_ARGS.end() && "WRONG COMMANDS IN SECTION_SIMULATION!");

    if (var == "MODEL") {
        if (val == "RMS") dynamic_args->model = DynamicModel::RMS;
        else if(val == "MODEL_BEAM") dynamic_args->model = DynamicModel::MODEL_BEAM;
        else if(val == "PARTICLE") dynamic_args->model = DynamicModel::PARTICLE;
        else if(val == "TURN_BY_TURN") dynamic_args->model = DynamicModel::TURN_BY_TURN;
        else assert(false&&"DYNAMIC MODEL NOT SUPPORTED IN SIMULATION!");
    }
    else if (var == "OUTPUT_FILE") {
        dynamic_args->filename = val;
    }
    else if (var == "IBS" ) {
        if (val == "ON" || val == "TRUE") dynamic_args->ibs = true;
        else if (val == "OFF" || val == "FALSE") dynamic_args->ibs = false;
        else assert(false&&"WRONG VALUE FOR THE PARAMETER IBS IN SECTION_SIMULATION!");
    }
    else if (var == "E_COOL") {
        if (val == "ON" || val == "TRUE") dynamic_args->ecool = true;
        else if (val == "OFF" || val == "FALSE") dynamic_args->ecool = false;
        else assert(false&&"WRONG VALUE FOR THE PARAMETER E_COOL IN SECTION_SIMULATION!");
    }
    else if (var == "FIXED_BUNCH_LENGTH") {
        if (val == "ON" || val == "TRUE") dynamic_args->fixed_bunch_length = true;
        else if (val == "OFF" || val == "FALSE") dynamic_args->fixed_bunch_length = false;
        else assert(false&&"WRONG VALUE FOR THE PARAMETER FIXED_BUNCH_LENGTH IN SECTION_SIMULATION!");
    }
    else if (var == "RESET_TIME") {
        if (val == "ON" || val == "TRUE") dynamic_args->reset_time = true;
        else if (val == "OFF" || val == "FALSE") dynamic_args->reset_time = false;
        else assert(false&&"WRONG VALUE FOR THE PARAMETER RESET_TIME IN SECTION_SIMULATION!");
    }
    else if (var == "OVERWRITE") {
        if (val == "ON" || val == "TRUE") dynamic_args->overwrite = true;
        else if (val == "OFF" || val == "FALSE") dynamic_args->overwrite = false;
        else assert(false&&"WRONG VALUE FOR THE PARAMETER OVERWRITE IN SECTION_SIMULATION!");
    }
    else if (var == "CALC_LUMINOSITY") {
        if (val == "ON" || val == "TRUE") dynamic_args->calc_luminosity = true;
        else if (val == "OFF" || val == "FALSE") dynamic_args->calc_luminosity = false;
        else assert(false&&"WRONG VALUE FOR THE PARAMETER CALC_LUMINOSITY IN SECTION_SIMULATION!");
    }
    else {
        if (math_parser == NULL) {
            if (var == "TIME") {
                dynamic_args->time = std::stod(val);
            }
            else if (var == "INI_TIME") {
                dynamic_args->t0 = std::stod(val);
            }
            else if (var == "STEP_NUMBER") {
                dynamic_args->n_step = std::stoi(val);
            }
            else if (var == "SAMPLE_NUMBER") {
                dynamic_args->n_sample = std::stoi(val);
            }
            else if (var == "OUTPUT_INTERVAL") {
                dynamic_args->output_intvl = std::stoi(val);
            }
            else if (var == "SAVE_PARTICLE_INTERVAL") {
                dynamic_args->save_ptcl_intvl = std::stoi(val);
            }
            else if (var == "REF_BET_X") {
                dynamic_args->twiss_ref.bet_x = std::stod(val);
            }
            else if (var == "REF_BET_Y") {
                dynamic_args->twiss_ref.bet_y = std::stod(val);
            }
            else if (var == "REF_ALF_X") {
                dynamic_args->twiss_ref.alf_x = std::stod(val);
            }
            else if (var == "REF_ALF_Y") {
                dynamic_args->twiss_ref.alf_y = std::stod(val);
            }
            else if (var == "REF_DISP_X") {
                dynamic_args->twiss_ref.disp_x = std::stod(val);
            }
            else if (var == "REF_DISP_Y") {
                dynamic_args->twiss_ref.disp_y = std::stod(val);
            }
            else if (var == "REF_DISP_DX") {
                dynamic_args->twiss_ref.disp_dx = std::stod(val);
            }
            else if (var == "REF_DISP_DY") {
                dynamic_args->twiss_ref.disp_dy = std::stod(val);
            }
            else {
                assert(false&&"Wrong arguments in section_simulation!");
            }
        }
        else {
            mupSetExpr(math_parser, val.c_str());
            if (var == "TIME") {
                dynamic_args->time = static_cast<double>(mupEval(math_parser));
            }
            else if (var == "INI_TIME") {
                dynamic_args->t0 = static_cast<double>(mupEval(math_parser));
            }
            else if (var == "STEP_NUMBER") {
                dynamic_args->n_step = static_cast<int>(mupEval(math_parser));
            }
            else if (var == "SAMPLE_NUMBER") {
                dynamic_args->n_sample = static_cast<int>(mupEval(math_parser));
            }
            else if (var == "OUTPUT_INTERVAL") {
                dynamic_args->output_intvl = static_cast<int>(mupEval(math_parser));
            }
            else if (var == "SAVE_PARTICLE_INTERVAL") {
                dynamic_args->save_ptcl_intvl = static_cast<int>(mupEval(math_parser));
            }
            else if (var == "REF_BET_X") {
                dynamic_args->twiss_ref.bet_x = static_cast<double>(mupEval(math_parser));
            }
            else if (var == "REF_BET_Y") {
                dynamic_args->twiss_ref.bet_y = static_cast<double>(mupEval(math_parser));
            }
            else if (var == "REF_ALF_X") {
                dynamic_args->twiss_ref.alf_x = static_cast<double>(mupEval(math_parser));
            }
            else if (var == "REF_ALF_Y") {
                dynamic_args->twiss_ref.alf_y = static_cast<double>(mupEval(math_parser));
            }
            else if (var == "REF_DISP_X") {
                dynamic_args->twiss_ref.disp_x = static_cast<double>(mupEval(math_parser));
            }
            else if (var == "REF_DISP_Y") {
                dynamic_args->twiss_ref.disp_y = static_cast<double>(mupEval(math_parser));
            }
            else if (var == "REF_DISP_DX") {
                dynamic_args->twiss_ref.disp_dx = static_cast<double>(mupEval(math_parser));
            }
            else if (var == "REF_DISP_DY") {
                dynamic_args->twiss_ref.disp_dy = static_cast<double>(mupEval(math_parser));
            }
            else {
                assert(false&&"Wrong arguments in section_simulation!");
            }
        }
    }
}

void set_luminosity(string &str, Set_luminosity *lum_args) {
    assert(lum_args!=nullptr && "SECTION_LUMINOSITY MUST BE CLAIMED!");
    string::size_type idx = str.find("=");
    assert(idx!=string::npos && "WRONG COMMAND IN SECTION_LUMINOSITY!");
    string var = str.substr(0, idx);
    string val = str.substr(idx+1);
    var = trim_blank(var);
    var = trim_tab(var);
    val = trim_blank(val);
    val = trim_tab(val);
    assert(std::find(LUMINOSITY_ARGS.begin(),LUMINOSITY_ARGS.end(),var)!=LUMINOSITY_ARGS.end() &&
           "WRONG COMMANDS IN SECTION_LUMINOSITY!");
    if (var == "USE_ION_EMITTANCE" ) {
        if (val == "ON" || val == "TRUE") lum_args->use_ion_emittance = true;
        else if (val == "OFF" || val == "FALSE") lum_args->use_ion_emittance = false;
        else assert(false&&"WRONG VALUE FOR THE PARAMETER USE_ION_PARAMETER IN SECTION_SIMULATION!");
    }
    else {
        if (math_parser == NULL) {
            if (var == "DISTANCE_X") {
                lum_args->dx = std::stod(val);
            }
            else if (var == "DISTANCE_Y") {
                lum_args->dy = std::stod(val);
            }
            else if (var == "PARTICLE_NUMBER_1") {
                lum_args->np_1 = std::stod(val);
            }
            else if (var == "PARTICLE_NUMBER_2") {
                lum_args->np_2 = std::stod(val);
            }
            else if (var == "FREQUENCY") {
                lum_args->freq = std::stod(val);
            }
            else if (var == "BEAM_SIZE_X_1") {
                lum_args->sigma_x1 = std::stod(val);
            }
            else if (var == "BEAM_SIZE_X_2") {
                lum_args->sigma_x2 = std::stod(val);
            }
            else if (var == "BEAM_SIZE_Y_1") {
                lum_args->sigma_y1 = std::stod(val);
            }
            else if (var == "BEAM_SIZE_Y_2") {
                lum_args->sigma_y2 = std::stod(val);
            }
            else if (var == "BET_X_1") {
                lum_args->bet_x1 = std::stod(val);
            }
            else if (var == "BET_X_2") {
                lum_args->bet_x2 = std::stod(val);
            }
            else if (var == "BET_Y_2") {
                lum_args->bet_y2 = std::stod(val);
            }
            else if (var == "BET_Y_1") {
                lum_args->bet_y1 = std::stod(val);
            }
            else if (var == "GEO_EMIT_X_1") {
                lum_args->geo_emit_x1 = std::stod(val);
            }
            else if (var == "GEO_EMIT_X_2") {
                lum_args->geo_emit_x2 = std::stod(val);
            }
            else if (var == "GEO_EMIT_Y_1") {
                lum_args->geo_emit_y1 = std::stod(val);
            }
            else if (var == "GEO_EMIT_Y_2") {
                lum_args->geo_emit_y2 = std::stod(val);
            }
            else {
                assert(false&&"Wrong arguments in section_luminosity!");
            }
        }
        else {
            mupSetExpr(math_parser, val.c_str());
            if (var == "DISTANCE_X") {
                lum_args->dx = static_cast<double>(mupEval(math_parser));
            }
            else if (var == "DISTANCE_Y") {
                lum_args->dy = static_cast<double>(mupEval(math_parser));
            }
            else if (var == "PARTICLE_NUMBER_1") {
                lum_args->np_1 = static_cast<double>(mupEval(math_parser));
            }
            else if (var == "PARTICLE_NUMBER_2") {
                lum_args->np_2 = static_cast<double>(mupEval(math_parser));
            }
            else if (var == "FREQUENCY") {
                lum_args->freq = static_cast<double>(mupEval(math_parser));
            }
            else if (var == "BEAM_SIZE_X_1") {
                lum_args->sigma_x1 = static_cast<double>(mupEval(math_parser));
            }
            else if (var == "BEAM_SIZE_X_2") {
                lum_args->sigma_x2 = static_cast<double>(mupEval(math_parser));
            }
            else if (var == "BEAM_SIZE_Y_1") {
                lum_args->sigma_y1 = static_cast<double>(mupEval(math_parser));
            }
            else if (var == "BEAM_SIZE_Y_2") {
                lum_args->sigma_y2 = static_cast<double>(mupEval(math_parser));
            }
            else if (var == "BET_X_1") {
                lum_args->bet_x1 = static_cast<double>(mupEval(math_parser));
            }
            else if (var == "BET_X_2") {
                lum_args->bet_x2 = static_cast<double>(mupEval(math_parser));
            }
            else if (var == "BET_Y_2") {
                lum_args->bet_y2 = static_cast<double>(mupEval(math_parser));
            }
            else if (var == "BET_Y_1") {
                lum_args->bet_y1 = static_cast<double>(mupEval(math_parser));
            }
            else if (var == "GEO_EMIT_X_1") {
                lum_args->geo_emit_x1 = static_cast<double>(mupEval(math_parser));
            }
            else if (var == "GEO_EMIT_X_2") {
                lum_args->geo_emit_x2 = static_cast<double>(mupEval(math_parser));
            }
            else if (var == "GEO_EMIT_Y_1") {
                lum_args->geo_emit_y1 = static_cast<double>(mupEval(math_parser));
            }
            else if (var == "GEO_EMIT_Y_2") {
                lum_args->geo_emit_y2 = static_cast<double>(mupEval(math_parser));
            }
            else {
                assert(false&&"Wrong arguments in section_luminosity!");
            }
        }
    }
}

void set_ecool(string &str, Set_ecool *ecool_args){
    assert(ecool_args!=nullptr && "SECTION_ECOOL MUST BE CLAIMED!");
    string::size_type idx = str.find("=");
    assert(idx!=string::npos && "WRONG COMMAND IN SECTION_ECOOL!");
    string var = str.substr(0, idx);
    string val = str.substr(idx+1);
    var = trim_blank(var);
    var = trim_tab(var);
    val = trim_blank(val);
    val = trim_tab(val);
    assert(std::find(ECOOL_ARGS.begin(),ECOOL_ARGS.end(),var)!=ECOOL_ARGS.end() && "WRONG COMMANDS IN SECTION_ECOOL!");

    if (var == "FORCE_FORMULA") {
//        ecool_args->force = val;
        if (val=="PARKHOMCHUK") ecool_args->force = ForceFormula::PARKHOMCHUK;
        else assert(false&&"Friction force formula NOT exists!");
    }
    else {
        if (math_parser == NULL) {
            if (var == "SAMPLE_NUMBE") {
                ecool_args->n_sample = std::stod(val);
            }
            else {
                assert(false&&"Wrong arguments in section_ecool!");
            }
        }
        else {
            mupSetExpr(math_parser, val.c_str());
            if (var == "SAMPLE_NUMBER") {
                ecool_args->n_sample = static_cast<double>(mupEval(math_parser));
            }
            else {
                assert(false&&"Wrong arguments in section_ecool!");
            }
        }
    }
}

void parse(std::string &str, muParserHandle_t &math_parser){

    if (str == "LIST_VAR") {
        ListVar(math_parser);
    }
    else if(str == "LIST_CONST") {
        ListConst(math_parser);
    }
    else if (str.substr(0,5) == "PRINT") {
        string var = str.substr(6);
        var = trim_blank(var);
        var = trim_tab(var);
        mupSetExpr(math_parser, var.c_str());
        std::cout<<var<<" = "<<mupEval(math_parser)<<std::endl;
    }
    else {
        mupSetExpr(math_parser, str.c_str());
        mupEval(math_parser);
    }
}
