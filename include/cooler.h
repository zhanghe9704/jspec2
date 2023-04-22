#ifndef COOLER_H
#define COOLER_H

class Cooler{
    double length_;      // in meter
    double section_number_;
    double magnetic_field_;           // in Tesla
    double beta_h_;      // in meter
    double beta_v_;      // in meter
    double disp_h_;
    double disp_v_;
    double alpha_h_;
    double alpha_v_;
    double der_disp_h_;
    double der_disp_v_;
    double pipe_radius_ = 0;
    double beta_mh_ = 0;    //beta function value (assuming alpha = 0) at the middle of the cooler
    double beta_mv_ = 0;
    int piece_number_ = 1;
    double beta_he_;      // in meter
    double beta_ve_;      // in meter
    double alpha_he_;
    double alpha_ve_;
    double beta_mhe_ = 0;    //beta function value (assuming alpha = 0) at the middle of the cooler
    double beta_mve_ = 0;
 public:
    double length(){return length_;}
    double section_number(){return section_number_;}
    double magnetic_field(){return magnetic_field_;}
    int piece_number() {return piece_number_;}
    void set_piece_number(int n) { piece_number_ = n>0?n:1;}
    double beta_h(){return beta_h_;}
    double beta_v(){return beta_v_;}
    double alpha_h(){return alpha_h_;}
    double alpha_v(){return alpha_v_;}
    double disp_h(){return disp_h_;}
    double disp_v(){return disp_v_;}
    double der_disp_h(){return der_disp_h_;}
    double der_disp_v(){return der_disp_v_;}
    double pipe_radius(){return pipe_radius_;}
    void set_pipe_radius(double x){pipe_radius_ = x;}
    void set_middle_beta(double beta_h, double beta_v) {
        if (piece_number_==1) return;
        double l = length_/2;
        beta_mh_ = beta_h;
        beta_mv_ = beta_v;
        beta_h_ = beta_h + l*l/beta_h;
        alpha_h_ = l*l/beta_h;
        beta_v_ = beta_v + l*l/beta_v;
        alpha_v_ = l*l/beta_v;
    }
    void set_middle_beta_e(double beta_h, double beta_v) {
        if (piece_number_==1) return;
        double l = length_/2;
        beta_mhe_ = beta_h;
        beta_mve_ = beta_v;
        beta_he_ = beta_h + l*l/beta_h;
        alpha_he_ = l*l/beta_h;
        beta_ve_ = beta_v + l*l/beta_v;
        alpha_ve_ = l*l/beta_v;
    }
    double beta_he(){return beta_he_;}
    double beta_ve(){return beta_ve_;}
    double alpha_he(){return alpha_he_;}
    double alpha_ve(){return alpha_ve_;}

    Cooler(double length, double section_number, double magnetic_field, double beta_h, double beta_v, double disp_h=0,
           double disp_v=0, double alpha_h=0, double alpha_v=0, double der_disp_h=0, double der_disp_v=0):length_(length),
           section_number_(section_number), magnetic_field_(magnetic_field), beta_h_(beta_h), beta_v_(beta_v),
           disp_h_(disp_h), disp_v_(disp_v), alpha_h_(alpha_h), alpha_v_(alpha_v), der_disp_h_(der_disp_h),
           der_disp_v_(der_disp_v){};
};

#endif // COOLER_H
