double eff(float p, float theta, float phi, int pid) {
  Acc *a = Fid::Instance()->acc;
  float err = 0;
  return a->GetEff(p,theta,phi,pid,err);
}

double efferr(float p, float theta, float phi, int pid) {
  float err = 0;
  Acc *a = Fid::Instance()->acc;
  a->GetEff(p,theta,phi,pid,err);
  return err;
}

double accw8(float w, float q2, float ct, float phi) {
  Acc *a = Fid::Instance()->acc;
  float err = 0;
  return a->GetAcc(w,q2,ct,phi,err);
}

double accw8err(float w, float q2, float ct, float phi) {
  Acc *a = Fid::Instance()->acc;
  float err = 0;
  a->GetAcc(w,q2,ct,phi,err);
  return err;
}
