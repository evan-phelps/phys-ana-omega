namespace MomCorr_E16 {
  static const float tormax = 3375.f;
  static const float tas1 = -5.1299142e-03f;
  static const float tas2 = -2.0352014e-03f;
  static const float tas3 = -2.826383e-04f;
  static const float tas4 = -1.58515365e-03f;
  static const float tas5 = -9.6490117e-04f;
  static const float tas6 = -8.60422808e-04f;
  static const float has1 = 6.37836137e-04f;
  static const float has2 = -3.910457e-04f;
  static const float has3 = 1.17432835e-03f;
  static const float has4 = 7.9181789e-04f;
  static const float has5 = 2.44439224e-03f;
  static const float has6 = 3.71918054e-05f;
  static const float qas1 = 2.72359806e-03f;
  static const float qas2 = -1.8383547e-03f;
  static const float qas3 = 1.71147716e-03f;
  static const float qas4 = 8.92225567e-05f;
  static const float qas5 = -4.205488e-03f;
  static const float qas6 = 3.39126187e-03f;
  static const float las1 = -1.13440559e-03f;
  static const float las2 = 4.10083333e-03f;
  static const float las3 = 1.1502628e-02f;
  static const float las4 = 8.71534207e-03f;
  static const float las5 = 6.3294693e-04f;
  static const float las6 = 1.38427978e-02f;
  static const float as1 = -5.16479755e-03f;
  static const float as2 = 8.16171478e-04f;
  static const float as3 = -3.45627949e-03f;
  static const float as4 = -2.6545698e-03f;
  static const float as5 = 1.33297453e-03f;
  static const float as6 = -3.70724347e-03f;
  static const float tbs1 = 2.24826958e-01f;
  static const float tbs2 = 9.0687908e-02f;
  static const float tbs3 = 1.7845019e-02f;
  static const float tbs4 = 8.09892278e-02f;
  static const float tbs5 = 4.9708738e-02f;
  static const float tbs6 = 4.63797833e-02f;
  static const float hbs1 = -3.31684806e-02f;
  static const float hbs2 = 1.2504244e-02f;
  static const float hbs3 = -5.12792186e-02f;
  static const float hbs4 = -3.6355055e-02f;
  static const float hbs5 = -1.08187405e-01f;
  static const float hbs6 = -7.9916894e-03f;
  static const float qbs1 = -1.221424e-01f;
  static const float qbs2 = 5.6957746e-02f;
  static const float qbs3 = -6.00117927e-02f;
  static const float qbs4 = -1.39340483e-02f;
  static const float qbs5 = 1.72421567e-01f;
  static const float qbs6 = -1.28512835e-01f;
  static const float lbs1 = 4.20764036e-02f;
  static const float lbs2 = -1.61183679e-01f;
  static const float lbs3 = -4.27954855e-01f;
  static const float lbs4 = -3.10226155e-01f;
  static const float lbs5 = -1.8280028e-02f;
  static const float lbs6 = -5.06583454e-01f;
  static const float bs1 = 2.07143724e-01f;
  static const float bs2 = -4.2716178e-02f;
  static const float bs3 = 1.09249069e-01f;
  static const float bs4 = 1.0517602e-01f;
  static const float bs5 = -2.003366e-02f;
  static const float bs6 = 1.59486435e-01f;
  static const float tcs1 = -2.44489241f;
  static const float tcs2 = -1.0028315f;
  static const float tcs3 = -2.7030428e-01f;
  static const float tcs4 = -9.95714015e-01f;
  static const float tcs5 = -6.3760666e-01f;
  static const float tcs6 = -6.0376152e-01f;
  static const float hcs1 = 4.07094475e-01f;
  static const float hcs2 = -6.89544888e-02f;
  static const float hcs3 = 5.47101163e-01f;
  static const float hcs4 = 4.23272698e-01f;
  static const float hcs5 = 1.20123351f;
  static const float hcs6 = 1.41910734e-01f;
  static const float qcs1 = 1.31174283f;
  static const float qcs2 = -5.1219438e-01f;
  static const float qcs3 = 3.5334639e-01f;
  static const float qcs4 = 1.62542337e-01f;
  static const float qcs5 = -1.77085757f;
  static const float qcs6 = 9.3981116e-01f;
  static const float lcs1 = -2.8053815e-01f;
  static const float lcs2 = 1.66281611f;
  static const float lcs3 = 3.4657869f;
  static const float lcs4 = 2.3246248f;
  static const float lcs5 = 4.5728914e-02f;
  static const float lcs6 = 4.00965538f;
  static const float cs1 = -2.09355677f;
  static const float cs2 = 5.64114112e-01f;
  static const float cs3 = -7.70204323e-01f;
  static const float cs4 = -1.14963156f;
  static const float cs5 = -5.01741685e-01f;
  static const float cs6 = -1.83039564f;
  static const float ps1 = 9.5112707858653e-08;
  static const float ps2 = 1.0033722952831e-07;
  static const float ps3 = 1.0797150380251e-07;
  static const float ps4 = 1.7100120165989e-07;
  static const float ps5 = 1.3082672597941e-08;
  static const float ps6 = 8.7085945937647e-08;
  static const float ks1 = -7.9599239534114e-06;
  static const float ks2 = -5.6599480422299e-06;
  static const float ks3 = -5.0800069598611e-06;
  static const float ks4 = -1.1728924887767e-05;
  static const float ks5 = -2.4757597510374e-06;
  static const float ks6 = -5.3018979653485e-06;
  static const float js1 = 1.0176579118581e-02;
  static const float js2 = 1.0087801836821e-02;
  static const float js3 = 1.0073933449548e-02;
  static const float js4 = 1.0221497899831e-02;
  static const float js5 = 1.0110107335248e-02;
  static const float js6 = 1.0112945449548e-02;
  static const float lps1 = -6.9400370675523e-05;
  static const float lps2 = -1.5960488468465e-04;
  static const float lps3 = -2.1899832926782e-04;
  static const float lps4 = -6.3507481408119e-05;
  static const float lps5 = -9.3393940238598e-05;
  static const float lps6 = -1.0356199140906e-04;
  static const float lks1 = 7.9655917387783e-03;
  static const float lks2 = 1.1661447536158e-02;
  static const float lks3 = 1.1211638296420e-02;
  static const float lks4 = 2.4612021589962e-03;
  static const float lks5 = 6.1878054901081e-03;
  static const float lks6 = 4.6198689235790e-03;
  static const float ljs1 = -0.20495101625537f;
  static const float ljs2 = -0.22412829804132f;
  static const float ljs3 = -1.0019870422334e-01;
  static const float ljs4 = -2.0318433751035e-02;
  static const float ljs5 = -0.11955549846234f;
  static const float ljs6 = -1.8772394756644e-02;
  static const float qps1 = -1.8086561701112e-04;
  static const float qps2 = -1.8929500388527e-04;
  static const float qps3 = -1.1416945384378e-04;
  static const float qps4 = -9.1297828333589e-05;
  static const float qps5 = -1.1784209351117e-04;
  static const float qps6 = 5.3194937440755e-05;
  static const float qks1 = 1.0743244205188e-02;
  static const float qks2 = 1.0942470368942e-02;
  static const float qks3 = 6.8099881808583e-03;
  static const float qks4 = 5.2998598339562e-03;
  static const float qks5 = 6.8477708809656e-03;
  static const float qks6 = -3.2324591761590e-03;
  static const float qjs1 = -0.15348562918845f;
  static const float qjs2 = -0.15076163008159f;
  static const float qjs3 = -9.9580311244930e-02;
  static const float qjs4 = -7.6365686745569e-02;
  static const float qjs5 = -9.6360858729669e-02;
  static const float qjs6 = 3.1792598257448e-02;
  static const float dps1 = 1.3019443527636e-04;
  static const float dps2 = 1.2506972055652e-04;
  static const float dps3 = -7.3395343512598e-06;
  static const float dps4 = -1.4020818540861e-05;
  static const float dps5 = 4.4662480048095e-05;
  static const float dps6 = 1.8214642092420e-05;
  static const float dks1 = -7.9967328974761e-03;
  static const float dks2 = -7.6499984847730e-03;
  static const float dks3 = 2.5650746001025e-04;
  static const float dks4 = 4.7628893452639e-04;
  static const float dks5 = -2.7046265634257e-03;
  static const float dks6 = -1.3819042499386e-03;
  static const float djs1 = 0.11761993283213f;
  static const float djs2 = 0.11317471787380f;
  static const float djs3 = -1.2820491669835e-04;
  static const float djs4 = 4.7326314122685e-04;
  static const float djs5 = 3.9383613408158e-02;
  static const float djs6 = 2.6160738941275e-02;
 
  static const float parms_pip_tas1 = -5.1299142e-03f;
  static const float parms_pip_tas2 = -2.0352014e-03f;
  static const float parms_pip_tas3 = -2.826383e-04f;
  static const float parms_pip_tas4 = -1.58515365e-03f;
  static const float parms_pip_tas5 = -9.6490117e-04f;
  static const float parms_pip_tas6 = -8.60422808e-04f;
  static const float parms_pip_has1 = 6.37836137e-04f;
  static const float parms_pip_has2 = -3.910457e-04f;
  static const float parms_pip_has3 = 1.17432835e-03f;
  static const float parms_pip_has4 = 7.9181789e-04f;
  static const float parms_pip_has5 = 2.44439224e-03f;
  static const float parms_pip_has6 = 3.71918054e-05f;
  static const float parms_pip_qas1 = 2.72359806e-03f;
  static const float parms_pip_qas2 = -1.8383547e-03f;
  static const float parms_pip_qas3 = 1.71147716e-03f;
  static const float parms_pip_qas4 = 8.92225567e-05f;
  static const float parms_pip_qas5 = -4.205488e-03f;
  static const float parms_pip_qas6 = 3.39126187e-03f;
  static const float parms_pip_las1 = -1.13440559e-03f;
  static const float parms_pip_las2 = 4.10083333e-03f;
  static const float parms_pip_las3 = 1.1502628e-02f;
  static const float parms_pip_las4 = 8.71534207e-03f;
  static const float parms_pip_las5 = 6.3294693e-04f;
  static const float parms_pip_las6 = 1.38427978e-02f;
  static const float parms_pip_as1 = -5.16479755e-03f;
  static const float parms_pip_as2 = 8.16171478e-04f;
  static const float parms_pip_as3 = -3.45627949e-03f;
  static const float parms_pip_as4 = -2.6545698e-03f;
  static const float parms_pip_as5 = 1.33297453e-03f;
  static const float parms_pip_as6 = -3.70724347e-03f;
  static const float parms_pip_tbs1 = 2.24826958e-01f;
  static const float parms_pip_tbs2 = 9.0687908e-02f;
  static const float parms_pip_tbs3 = 1.7845019e-02f;
  static const float parms_pip_tbs4 = 8.09892278e-02f;
  static const float parms_pip_tbs5 = 4.9708738e-02f;
  static const float parms_pip_tbs6 = 4.63797833e-02f;
  static const float parms_pip_hbs1 = -3.31684806e-02f;
  static const float parms_pip_hbs2 = 1.2504244e-02f;
  static const float parms_pip_hbs3 = -5.12792186e-02f;
  static const float parms_pip_hbs4 = -3.6355055e-02f;
  static const float parms_pip_hbs5 = -1.08187405e-01f;
  static const float parms_pip_hbs6 = -7.9916894e-03f;
  static const float parms_pip_qbs1 = -1.221424e-01f;
  static const float parms_pip_qbs2 = 5.6957746e-02f;
  static const float parms_pip_qbs3 = -6.00117927e-02f;
  static const float parms_pip_qbs4 = -1.39340483e-02f;
  static const float parms_pip_qbs5 = 1.72421567e-01f;
  static const float parms_pip_qbs6 = -1.28512835e-01f;
  static const float parms_pip_lbs1 = 4.20764036e-02f;
  static const float parms_pip_lbs2 = -1.61183679e-01f;
  static const float parms_pip_lbs3 = -4.27954855e-01f;
  static const float parms_pip_lbs4 = -3.10226155e-01f;
  static const float parms_pip_lbs5 = -1.8280028e-02f;
  static const float parms_pip_lbs6 = -5.06583454e-01f;
  static const float parms_pip_bs1 = 2.07143724e-01f;
  static const float parms_pip_bs2 = -4.2716178e-02f;
  static const float parms_pip_bs3 = 1.09249069e-01f;
  static const float parms_pip_bs4 = 1.0517602e-01f;
  static const float parms_pip_bs5 = -2.003366e-02f;
  static const float parms_pip_bs6 = 1.59486435e-01f;
  static const float parms_pip_tcs1 = -2.44489241f;
  static const float parms_pip_tcs2 = -1.0028315f;
  static const float parms_pip_tcs3 = -2.7030428e-01f;
  static const float parms_pip_tcs4 = -9.95714015e-01f;
  static const float parms_pip_tcs5 = -6.3760666e-01f;
  static const float parms_pip_tcs6 = -6.0376152e-01f;
  static const float parms_pip_hcs1 = 4.07094475e-01f;
  static const float parms_pip_hcs2 = -6.89544888e-02f;
  static const float parms_pip_hcs3 = 5.47101163e-01f;
  static const float parms_pip_hcs4 = 4.23272698e-01f;
  static const float parms_pip_hcs5 = 1.20123351f;
  static const float parms_pip_hcs6 = 1.41910734e-01f;
  static const float parms_pip_qcs1 = 1.31174283f;
  static const float parms_pip_qcs2 = -5.1219438e-01f;
  static const float parms_pip_qcs3 = 3.5334639e-01f;
  static const float parms_pip_qcs4 = 1.62542337e-01f;
  static const float parms_pip_qcs5 = -1.77085757f;
  static const float parms_pip_qcs6 = 9.3981116e-01f;
  static const float parms_pip_lcs1 = -2.8053815e-01f;
  static const float parms_pip_lcs2 = 1.66281611f;
  static const float parms_pip_lcs3 = 3.4657869f;
  static const float parms_pip_lcs4 = 2.3246248f;
  static const float parms_pip_lcs5 = 4.5728914e-02f;
  static const float parms_pip_lcs6 = 4.00965538f;
  static const float parms_pip_cs1 = -2.09355677f;
  static const float parms_pip_cs2 = 5.64114112e-01f;
  static const float parms_pip_cs3 = -7.70204323e-01f;
  static const float parms_pip_cs4 = -1.14963156f;
  static const float parms_pip_cs5 = -5.01741685e-01f;
  static const float parms_pip_cs6 = -1.83039564f;
  static const float parms_pip_pphas1 = -7.1844219079881e-05;
  static const float parms_pip_pphas2 = -4.8440373290278e-05;
  static const float parms_pip_pphas3 = -7.5425590169320e-05;
  static const float parms_pip_pphas4 = -4.4554868249908e-05;
  static const float parms_pip_pphas5 = -1.2137659841733e-04;
  static const float parms_pip_pphas6 = -7.0998407879210e-05;
  static const float parms_pip_pptas1 = 6.3269024406430e-06;
  static const float parms_pip_pptas2 = -2.4069056941098e-05;
  static const float parms_pip_pptas3 = -7.0215912241867e-05;
  static const float parms_pip_pptas4 = -2.0278991993274e-05;
  static const float parms_pip_pptas5 = 1.3774882073097e-04;
  static const float parms_pip_pptas6 = 1.7314651554110e-05;
  static const float parms_pip_ppqas1 = 6.9390123917417e-05;
  static const float parms_pip_ppqas2 = -2.7535249014628e-06;
  static const float parms_pip_ppqas3 = 1.1662582466434e-04;
  static const float parms_pip_ppqas4 = 4.3454299924587e-05;
  static const float parms_pip_ppqas5 = 8.7070678161577e-05;
  static const float parms_pip_ppqas6 = 5.6084084931323e-05;
  static const float parms_pip_pplas1 = -1.0869417734953e-04;
  static const float parms_pip_pplas2 = -5.8143222676711e-05;
  static const float parms_pip_pplas3 = 1.9792243033956e-04;
  static const float parms_pip_pplas4 = 1.1593141811510e-04;
  static const float parms_pip_pplas5 = -3.7830937738190e-05;
  static const float parms_pip_pplas6 = 4.9455233402118e-05;
  static const float parms_pip_ppas1 = -4.3424651403068e-07;
  static const float parms_pip_ppas2 = 1.0630214597195e-06;
  static const float parms_pip_ppas3 = -3.5053116445780e-07;
  static const float parms_pip_ppas4 = -5.8560815086425e-07;
  static const float parms_pip_ppas5 = -2.5695606257633e-07;
  static const float parms_pip_ppas6 = -4.0057393727035e-07;
  static const float parms_pip_pphbs1 = 4.6949962334791e-03;
  static const float parms_pip_pphbs2 = 3.2695665809276e-03;
  static const float parms_pip_pphbs3 = 4.9441619886524e-03;
  static const float parms_pip_pphbs4 = 2.8710131678481e-03;
  static const float parms_pip_pphbs5 = 7.8106249434667e-03;
  static const float parms_pip_pphbs6 = 4.4864346138715e-03;
  static const float parms_pip_pptbs1 = -1.3143424370788e-04;
  static const float parms_pip_pptbs2 = 1.7806239924986e-03;
  static const float parms_pip_pptbs3 = 4.6940828938751e-03;
  static const float parms_pip_pptbs4 = 1.4721139409307e-03;
  static const float parms_pip_pptbs5 = -8.6812638324776e-03;
  static const float parms_pip_pptbs6 = -8.1883768503485e-04;
  static const float parms_pip_ppqbs1 = -4.9511941857830e-03;
  static const float parms_pip_ppqbs2 = -2.6993010700726e-04;
  static const float parms_pip_ppqbs3 = -7.6944128537099e-03;
  static const float parms_pip_ppqbs4 = -2.9053080429949e-03;
  static const float parms_pip_ppqbs5 = -6.3117358280142e-03;
  static const float parms_pip_ppqbs6 = -4.0186323969971e-03;
  static const float parms_pip_pplbs1 = 8.8234617050698e-03;
  static const float parms_pip_pplbs2 = 4.5095253789595e-03;
  static const float parms_pip_pplbs3 = -1.3832456668735e-02;
  static const float parms_pip_pplbs4 = -7.9456250292464e-03;
  static const float parms_pip_pplbs5 = 3.1575061611213e-03;
  static const float parms_pip_pplbs6 = -4.7771875186452e-03;
  static const float parms_pip_ppbs1 = 2.5117588263152e-05;
  static const float parms_pip_ppbs2 = -8.8638293953487e-05;
  static const float parms_pip_ppbs3 = 7.3721662420257e-06;
  static const float parms_pip_ppbs4 = 4.4341862119425e-05;
  static const float parms_pip_ppbs5 = 1.2818991016199e-05;
  static const float parms_pip_ppbs6 = 3.5424024673774e-05;
  static const float parms_pip_pphcs1 = -7.9400009868257e-02;
  static const float parms_pip_pphcs2 = -5.9553204368528e-02;
  static const float parms_pip_pphcs3 = -8.4112643988478e-02;
  static const float parms_pip_pphcs4 = -4.8536135903168e-02;
  static const float parms_pip_pphcs5 = -0.12494311516901f;
  static const float parms_pip_pphcs6 = -7.4126030849555e-02;
  static const float parms_pip_pptcs1 = -8.0304529031240e-03;
  static const float parms_pip_pptcs2 = -3.8842746925294e-02;
  static const float parms_pip_pptcs3 = -8.1306337783908e-02;
  static const float parms_pip_pptcs4 = -3.0499202793116e-02;
  static const float parms_pip_pptcs5 = 0.13249260356287f;
  static const float parms_pip_pptcs6 = 2.1991659224153e-03;
  static const float parms_pip_ppqcs1 = 0.10637200070218f;
  static const float parms_pip_ppqcs2 = 3.4246805206286e-02;
  static const float parms_pip_ppqcs3 = 0.14194708949095f;
  static const float parms_pip_ppqcs4 = 5.8621184194944e-02;
  static const float parms_pip_ppqcs5 = 0.11934450352563f;
  static const float parms_pip_ppqcs6 = 9.7327218289753e-02;
  static const float parms_pip_pplcs1 = -0.14511389911125f;
  static const float parms_pip_pplcs2 = -5.2945117280107e-02;
  static const float parms_pip_pplcs3 = 0.27043480204968f;
  static const float parms_pip_pplcs4 = 0.15979137314589f;
  static const float parms_pip_pplcs5 = -4.7681356529085e-02;
  static const float parms_pip_pplcs6 = 0.15144397990750f;
  static const float parms_pip_ppcs1 = 9.9380861161087e-02;
  static const float parms_pip_ppcs2 = 1.0165916607859e-01;
  static const float parms_pip_ppcs3 = 1.0024929124968e-01;
  static const float parms_pip_ppcs4 = 9.8929456850176e-02;
  static const float parms_pip_ppcs5 = 9.9827501650252e-02;
  static const float parms_pip_ppcs6 = 9.9153515589292e-02;

float pow2(float x) {
  return x*x;
}

float pow3(float x) {
  return x*x*x;
}

float pow4(float x) {
  return x*x*x*x;
}

void e_corr_sub(
  float const& thetaeld,
  float const& phield,
  float const& pel,
  float const& torcur,
  int const& secte,
  float& thetaeldnew,
  float& newpel)
{
  //C***************************************************************
  //C           secte : detector sector                            *
  //C           torcur : torus current [A]
  //C           pel  : uncorrected electron momentum               *
  //C  INPUT    phield, thetaeld : dimension : [degree]            *
  //C                           : uncorrected electron angles      *
  //C--------------------------------------------------------------*
  //C  OUTPUT   thetae_corr : corrected electron angle [degree]    *
  //C           pe_corr     : corrected electron momenutm          *
  //C***************************************************************
  //C
  //C     ########### theta & phi correction constants ##########
  //C
  //C     23456789012345678901234567890123456789012345678901234567890123456789
  //C
  //C     ########### momentum correction constants ##########
  //C
  //C     ###########  ELECTRON PHI&THETA ANGLE CORRECTION  #########BEGIN
  float phield2;
  float identifier_const;
  float linear;
  float quadratic;
  float thirdorder;
  float fourthorder;
  float identifier_new;
  if (thetaeld > 12 && thetaeld < 25.f) {
    //C
    if (secte == 1) {
      phield2 = phield;
      //C          if(phield2.gt.-20.0.and.phield2.lt.20.0)then ! constraint phi angle
      identifier_const = (cs1 + bs1 * thetaeld + as1 * pow2(
        thetaeld)) / 10.f;
      linear = (lcs1 + lbs1 * thetaeld + las1 * pow2(thetaeld)) / 100.f;
      quadratic = (qcs1 + qbs1 * thetaeld + qas1 * pow2(
        thetaeld)) / 1000.f;
      thirdorder = (tcs1 + tbs1 * thetaeld + tas1 * pow2(
        thetaeld)) / 10000.f;
      fourthorder = (hcs1 + hbs1 * thetaeld + has1 * pow2(
        thetaeld)) / 100000.f;
      //C
      identifier_new = (identifier_const + linear * phield2 +
        quadratic * pow2(phield2) + thirdorder * pow3(
        phield2) + fourthorder * pow4(phield2));
      thetaeldnew = thetaeld + identifier_new;
      //C           endif        ! constraint phi angle
      //C
    }
    else if (secte == 2) {
      phield2 = phield - 60;
      //C          if(phield2.gt.-20.0.and.phield2.lt.20.0)then ! constraint phi angle
      identifier_const = (cs2 + bs2 * thetaeld + as2 * pow2(
        thetaeld)) / 10.f;
      linear = (lcs2 + lbs2 * thetaeld + las2 * pow2(thetaeld)) / 100.f;
      quadratic = (qcs2 + qbs2 * thetaeld + qas2 * pow2(
        thetaeld)) / 1000.f;
      thirdorder = (tcs2 + tbs2 * thetaeld + tas2 * pow2(
        thetaeld)) / 10000.f;
      fourthorder = (hcs2 + hbs2 * thetaeld + has2 * pow2(
        thetaeld)) / 100000.f;
      //C
      identifier_new = (identifier_const + linear * phield2 +
        quadratic * pow2(phield2) + thirdorder * pow3(
        phield2) + fourthorder * pow4(phield2));
      thetaeldnew = thetaeld + identifier_new;
      //C           endif        ! constraint phi angle
      //C
    }
    else if (secte == 3) {
      phield2 = phield - 120;
      //C           if(phield2.gt.-20.0.and.phield2.lt.20.0)then ! constraint phi angle
      identifier_const = (cs3 + bs3 * thetaeld + as3 * pow2(
        thetaeld)) / 10.f;
      linear = (lcs3 + lbs3 * thetaeld + las3 * pow2(thetaeld)) / 100.f;
      quadratic = (qcs3 + qbs3 * thetaeld + qas3 * pow2(
        thetaeld)) / 1000.f;
      thirdorder = (tcs3 + tbs3 * thetaeld + tas3 * pow2(
        thetaeld)) / 10000.f;
      fourthorder = (hcs3 + hbs3 * thetaeld + has3 * pow2(
        thetaeld)) / 100000.f;
      //C
      identifier_new = (identifier_const + linear * phield2 +
        quadratic * pow2(phield2) + thirdorder * pow3(
        phield2) + fourthorder * pow4(phield2));
      thetaeldnew = thetaeld + identifier_new;
      //C           endif        ! constraint phi angle
      //C
    }
    else if (secte == 4) {
      if (phield >= 150 && phield <= 180) {
        phield2 = phield - 180;
      }
      else if (phield >=  - 180 && phield <=  - 150) {
        phield2 = phield + 180;
      }
      //C          if(phield2.gt.-20.0.and.phield2.lt.20.0)then ! constraint phi angle
      identifier_const = (cs4 + bs4 * thetaeld + as4 * pow2(
        thetaeld)) / 10.f;
      linear = (lcs4 + lbs4 * thetaeld + las4 * pow2(thetaeld)) / 100.f;
      quadratic = (qcs4 + qbs4 * thetaeld + qas4 * pow2(
        thetaeld)) / 1000.f;
      thirdorder = (tcs4 + tbs4 * thetaeld + tas4 * pow2(
        thetaeld)) / 10000.f;
      fourthorder = (hcs4 + hbs4 * thetaeld + has4 * pow2(
        thetaeld)) / 100000.f;
      //C
      identifier_new = (identifier_const + linear * phield2 +
        quadratic * pow2(phield2) + thirdorder * pow3(
        phield2) + fourthorder * pow4(phield2));
      thetaeldnew = thetaeld + identifier_new;
      //C           endif        ! constraint phi angle
      //C
    }
    else if (secte == 5) {
      phield2 = phield + 120;
      //C          if(phield2.gt.-20.0.and.phield2.lt.20.0)then ! constraint phi angle
      identifier_const = (cs5 + bs5 * thetaeld + as5 * pow2(
        thetaeld)) / 10.f;
      linear = (lcs5 + lbs5 * thetaeld + las5 * pow2(thetaeld)) / 100.f;
      quadratic = (qcs5 + qbs5 * thetaeld + qas5 * pow2(
        thetaeld)) / 1000.f;
      thirdorder = (tcs5 + tbs5 * thetaeld + tas5 * pow2(
        thetaeld)) / 10000.f;
      fourthorder = (hcs5 + hbs5 * thetaeld + has5 * pow2(
        thetaeld)) / 100000.f;
      //C
      identifier_new = (identifier_const + linear * phield2 +
        quadratic * pow2(phield2) + thirdorder * pow3(
        phield2) + fourthorder * pow4(phield2));
      thetaeldnew = thetaeld + identifier_new;
      //C           endif        ! constraint phi angle
      //C
    }
    else if (secte == 6) {
      phield2 = phield + 60;
      //C          if(phield2.gt.-20.0.and.phield2.lt.20.0)then ! constraint phi angle
      identifier_const = (cs6 + bs6 * thetaeld + as6 * pow2(
        thetaeld)) / 10.f;
      linear = (lcs6 + lbs6 * thetaeld + las6 * pow2(thetaeld)) / 100.f;
      quadratic = (qcs6 + qbs6 * thetaeld + qas6 * pow2(
        thetaeld)) / 1000.f;
      thirdorder = (tcs6 + tbs6 * thetaeld + tas6 * pow2(
        thetaeld)) / 10000.f;
      fourthorder = (hcs6 + hbs6 * thetaeld + has6 * pow2(
        thetaeld)) / 100000.f;
      //C
      identifier_new = (identifier_const + linear * phield2 +
        quadratic * pow2(phield2) + thirdorder * pow3(
        phield2) + fourthorder * pow4(phield2));
      thetaeldnew = thetaeld + identifier_new;
      //C           endif        ! constraint phi angle
      //C
    }
  }
  else if (thetaeld <= 12.f || thetaeld >= 25.f) {
    if (secte == 1) {
      phield2 = phield;
      thetaeldnew = thetaeld;
    }
    else if (secte == 2) {
      phield2 = phield - 60;
      thetaeldnew = thetaeld;
    }
    else if (secte == 3) {
      phield2 = phield - 120;
      thetaeldnew = thetaeld;
    }
    else if (secte == 4) {
      if (phield >= 150 && phield <= 180) {
        phield2 = phield - 180;
      }
      else if (phield >=  - 180 && phield <=  - 150) {
        phield2 = phield + 180;
      }
      thetaeldnew = thetaeld;
    }
    else if (secte == 5) {
      phield2 = phield + 120;
      thetaeldnew = thetaeld;
    }
    else if (secte == 6) {
      phield2 = phield + 60;
      thetaeldnew = thetaeld;
    }
    //C proton angle limitation
  }
  //C
  thetaeldnew = thetaeldnew;
  //C
  //C     ######### ELECTRON PHI&THETA ANGLE CORRECTION  ##########END
  //C
  //C     ************** magnetic field consideration **************
  //C
  float bratio = torcur / tormax;
  //C
  //C     ************ Electron Momentum Correction Parts ************BEGIN*
  newpel = pel;
  //C
  float sangsu;
  float eilcha;
  float jeggop;
  float sejeggop;
  float corr_fact;
  if (secte == 1) {
    //C constraint phi ang
    if (phield2 >  - 20.0f && phield2 < 20.0f) {
      sangsu = (js1 + ks1 * thetaeldnew + ps1 * pow2(thetaeldnew)) * 100.f;
      eilcha = (ljs1 + lks1 * thetaeldnew + lps1 * pow2(
        thetaeldnew)) / 100.f;
      jeggop = (qjs1 + qks1 * thetaeldnew + qps1 * pow2(
        thetaeldnew)) / 1000.f;
      sejeggop = (djs1 + dks1 * thetaeldnew + dps1 * pow2(
        thetaeldnew)) / 10000.f;
      //C
      corr_fact = (sangsu + eilcha * phield2 + jeggop * pow2(
        phield2) + sejeggop * pow3(phield2));
      newpel = pel * corr_fact * bratio;
      //C constraint phi angle
    }
    //C
  }
  else if (secte == 2) {
    //C constraint phi ang
    if (phield2 >  - 20.0f && phield2 < 20.0f) {
      sangsu = (js2 + ks2 * thetaeldnew + ps2 * pow2(thetaeldnew)) * 100.f;
      eilcha = (ljs2 + lks2 * thetaeldnew + lps2 * pow2(
        thetaeldnew)) / 100.f;
      jeggop = (qjs2 + qks2 * thetaeldnew + qps2 * pow2(
        thetaeldnew)) / 1000.f;
      sejeggop = (djs2 + dks2 * thetaeldnew + dps2 * pow2(
        thetaeldnew)) / 10000.f;
      //C
      corr_fact = (sangsu + eilcha * phield2 + jeggop * pow2(
        phield2) + sejeggop * pow3(phield2));
      newpel = pel * corr_fact * bratio;
      //C constraint phi angle
    }
    //C
  }
  else if (secte == 3) {
    //C constraint phi ang
    if (phield2 >  - 20.0f && phield2 < 20.0f) {
      sangsu = (js3 + ks3 * thetaeldnew + ps3 * pow2(thetaeldnew)) * 100.f;
      eilcha = (ljs3 + lks3 * thetaeldnew + lps3 * pow2(
        thetaeldnew)) / 100.f;
      jeggop = (qjs3 + qks3 * thetaeldnew + qps3 * pow2(
        thetaeldnew)) / 1000.f;
      sejeggop = (djs3 + dks3 * thetaeldnew + dps3 * pow2(
        thetaeldnew)) / 10000.f;
      //C
      corr_fact = (sangsu + eilcha * phield2 + jeggop * pow2(
        phield2) + sejeggop * pow3(phield2));
      newpel = pel * corr_fact * bratio;
      //C constraint phi angle
    }
    //C
  }
  else if (secte == 4) {
    //C constraint phi ang
    if (phield2 >  - 20.0f && phield2 < 20.0f) {
      sangsu = (js4 + ks4 * thetaeldnew + ps4 * pow2(thetaeldnew)) * 100.f;
      eilcha = (ljs4 + lks4 * thetaeldnew + lps4 * pow2(
        thetaeldnew)) / 100.f;
      jeggop = (qjs4 + qks4 * thetaeldnew + qps4 * pow2(
        thetaeldnew)) / 1000.f;
      sejeggop = (djs4 + dks4 * thetaeldnew + dps4 * pow2(
        thetaeldnew)) / 10000.f;
      //C
      corr_fact = (sangsu + eilcha * phield2 + jeggop * pow2(
        phield2) + sejeggop * pow3(phield2));
      newpel = pel * corr_fact * bratio;
      //C constraint phi angle
    }
    //C
  }
  else if (secte == 5) {
    //C constraint phi ang
    if (phield2 >  - 20.0f && phield2 < 20.0f) {
      sangsu = (js5 + ks5 * thetaeldnew + ps5 * pow2(thetaeldnew)) * 100.f;
      eilcha = (ljs5 + lks5 * thetaeldnew + lps5 * pow2(
        thetaeldnew)) / 100.f;
      jeggop = (qjs5 + qks5 * thetaeldnew + qps5 * pow2(
        thetaeldnew)) / 1000.f;
      sejeggop = (djs5 + dks5 * thetaeldnew + dps5 * pow2(
        thetaeldnew)) / 10000.f;
      //C
      corr_fact = (sangsu + eilcha * phield2 + jeggop * pow2(
        phield2) + sejeggop * pow3(phield2));
      newpel = pel * corr_fact * bratio;
      //C constraint phi angle
    }
    //C
  }
  else if (secte == 6) {
    //C constraint phi ang
    if (phield2 >  - 20.0f && phield2 < 20.0f) {
      sangsu = (js6 + ks6 * thetaeldnew + ps6 * pow2(thetaeldnew)) * 100.f;
      eilcha = (ljs6 + lks6 * thetaeldnew + lps6 * pow2(
        thetaeldnew)) / 100.f;
      jeggop = (qjs6 + qks6 * thetaeldnew + qps6 * pow2(
        thetaeldnew)) / 1000.f;
      sejeggop = (djs6 + dks6 * thetaeldnew + dps6 * pow2(
        thetaeldnew)) / 10000.f;
      //C
      corr_fact = (sangsu + eilcha * phield2 + jeggop * pow2(
        phield2) + sejeggop * pow3(phield2));
      newpel = pel * corr_fact * bratio;
      //C constraint phi angle
    }
  }
  newpel = newpel;
  //C
}

void pi_corr_sub(
  float const& thetapid,
  float const& phipid,
  float const& mom_pip,
  float const& torcur,
  int const& secth,
  float& nthetapid,
  float& newpip)
{
  //C***************************************************************
  //C           secte : detector sector                            *
  //C           torcur : torus current [A]
  //C           p_pip  : uncorrected (+) pion momentum             *
  //C  INPUT    phipid, thetapid : dimension : [degree]            *
  //C                           : uncorrected pion(+)  angles      *
  //C--------------------------------------------------------------*
  //C  OUTPUT   thetapi_corr : corrected pion(+) angle [degree]    *
  //C           ppi_corr     : corrected pion(+) momenutm          *
  //C***************************************************************
  //C
  //C     ########### pion(+) angles correction constants ##########
  //C
  //C23456789012345678901234567890123456789012345678901234567890123456789
  //C
  //C     ########### pi^+ momentum correction constants ##########
  //C
  //C23456789012345678901234567890123456789012345678901234567890123456789
  //C
  //C#########################################################################
  //C
  //C     ##################  PHI&THETA PION ANGLE CORRECTION  #############BEGIN
  float phipid2;
  if (secth == 1) {
    phipid2 = phipid;
  }
  else if (secth == 2) {
    phipid2 = phipid - 60.f;
  }
  else if (secth == 3) {
    phipid2 = phipid - 120.f;
  }
  else if (secth == 4) {
    if (phipid >= 150 && phipid <= 180) {
      phipid2 = phipid - 180;
    }
    else if (phipid >=  - 180 && phipid <=  - 150) {
      phipid2 = phipid + 180;
    }
  }
  else if (secth == 5) {
    phipid2 = phipid + 120.f;
  }
  else if (secth == 6) {
    phipid2 = phipid + 60.f;
  }
  //C
  float identifier_const;
  int linear;
  float quadratic;
  float thirdorder;
  float fourthorder;
  float pinew;
  if (thetapid > 12 && thetapid < 25.f) {
    //C
    if (secth == 1) {
      //C constraint phi an
      if (phipid2 >  - 25.0f && phipid2 < 25.0f) {
        identifier_const = (parms_pip_cs1 + parms_pip_bs1 * thetapid + parms_pip_as1 * pow2(
          thetapid)) / 10.f;
        linear = (parms_pip_lcs1 + parms_pip_lbs1 * thetapid + parms_pip_las1 * pow2(thetapid)) / 100.f;
        quadratic = (parms_pip_qcs1 + parms_pip_qbs1 * thetapid + parms_pip_qas1 * pow2(
          thetapid)) / 1000.f;
        thirdorder = (parms_pip_tcs1 + parms_pip_tbs1 * thetapid + parms_pip_tas1 * pow2(
          thetapid)) / 10000.f;
        fourthorder = (parms_pip_hcs1 + parms_pip_hbs1 * thetapid + parms_pip_has1 * pow2(
          thetapid)) / 100000.f;
        //C
        pinew = (identifier_const + linear * phipid2 + quadratic *
          pow2(phipid2) + thirdorder * pow3(phipid2) +
          fourthorder * pow4(phipid2));
        nthetapid = thetapid + pinew;
        //C constraint phi angle
      }
      //C
    }
    else if (secth == 2) {
      //C constraint phi an
      if (phipid2 >  - 25.0f && phipid2 < 25.0f) {
        identifier_const = (parms_pip_cs2 + parms_pip_bs2 * thetapid + parms_pip_as2 * pow2(
          thetapid)) / 10.f;
        linear = (parms_pip_lcs2 + parms_pip_lbs2 * thetapid + parms_pip_las2 * pow2(thetapid)) / 100.f;
        quadratic = (parms_pip_qcs2 + parms_pip_qbs2 * thetapid + parms_pip_qas2 * pow2(
          thetapid)) / 1000.f;
        thirdorder = (parms_pip_tcs2 + parms_pip_tbs2 * thetapid + parms_pip_tas2 * pow2(
          thetapid)) / 10000.f;
        fourthorder = (parms_pip_hcs2 + parms_pip_hbs2 * thetapid + parms_pip_has2 * pow2(
          thetapid)) / 100000.f;
        //C
        pinew = (identifier_const + linear * phipid2 + quadratic *
          pow2(phipid2) + thirdorder * pow3(phipid2) +
          fourthorder * pow4(phipid2));
        nthetapid = thetapid + pinew;
        //C constraint phi angle
      }
      //C
    }
    else if (secth == 3) {
      //C constraint phi an
      if (phipid2 >  - 25.0f && phipid2 < 25.0f) {
        identifier_const = (parms_pip_cs3 + parms_pip_bs3 * thetapid + parms_pip_as3 * pow2(
          thetapid)) / 10.f;
        linear = (parms_pip_lcs3 + parms_pip_lbs3 * thetapid + parms_pip_las3 * pow2(thetapid)) / 100.f;
        quadratic = (parms_pip_qcs3 + parms_pip_qbs3 * thetapid + parms_pip_qas3 * pow2(
          thetapid)) / 1000.f;
        thirdorder = (parms_pip_tcs3 + parms_pip_tbs3 * thetapid + parms_pip_tas3 * pow2(
          thetapid)) / 10000.f;
        fourthorder = (parms_pip_hcs3 + parms_pip_hbs3 * thetapid + parms_pip_has3 * pow2(
          thetapid)) / 100000.f;
        //C
        pinew = (identifier_const + linear * phipid2 + quadratic *
          pow2(phipid2) + thirdorder * pow3(phipid2) +
          fourthorder * pow4(phipid2));
        nthetapid = thetapid + pinew;
        //C constraint phi angle
      }
      //C
    }
    else if (secth == 4) {
      //C constraint phi an
      if (phipid2 >  - 25.0f && phipid2 < 25.0f) {
        identifier_const = (parms_pip_cs4 + parms_pip_bs4 * thetapid + parms_pip_as4 * pow2(
          thetapid)) / 10.f;
        linear = (parms_pip_lcs4 + parms_pip_lbs4 * thetapid + parms_pip_las4 * pow2(thetapid)) / 100.f;
        quadratic = (parms_pip_qcs4 + parms_pip_qbs4 * thetapid + parms_pip_qas4 * pow2(
          thetapid)) / 1000.f;
        thirdorder = (parms_pip_tcs4 + parms_pip_tbs4 * thetapid + parms_pip_tas4 * pow2(
          thetapid)) / 10000.f;
        fourthorder = (parms_pip_hcs4 + parms_pip_hbs4 * thetapid + parms_pip_has4 * pow2(
          thetapid)) / 100000.f;
        //C
        pinew = (identifier_const + linear * phipid2 + quadratic *
          pow2(phipid2) + thirdorder * pow3(phipid2) +
          fourthorder * pow4(phipid2));
        nthetapid = thetapid + pinew;
        //C constraint phi angle
      }
      //C
    }
    else if (secth == 5) {
      //C constraint phi an
      if (phipid2 >  - 25.0f && phipid2 < 25.0f) {
        identifier_const = (parms_pip_cs5 + parms_pip_bs5 * thetapid + parms_pip_as5 * pow2(
          thetapid)) / 10.f;
        linear = (parms_pip_lcs5 + parms_pip_lbs5 * thetapid + parms_pip_las5 * pow2(thetapid)) / 100.f;
        quadratic = (parms_pip_qcs5 + parms_pip_qbs5 * thetapid + parms_pip_qas5 * pow2(
          thetapid)) / 1000.f;
        thirdorder = (parms_pip_tcs5 + parms_pip_tbs5 * thetapid + parms_pip_tas5 * pow2(
          thetapid)) / 10000.f;
        fourthorder = (parms_pip_hcs5 + parms_pip_hbs5 * thetapid + parms_pip_has5 * pow2(
          thetapid)) / 100000.f;
        //C
        pinew = (identifier_const + linear * phipid2 + quadratic *
          pow2(phipid2) + thirdorder * pow3(phipid2) +
          fourthorder * pow4(phipid2));
        nthetapid = thetapid + pinew;
        //C constraint phi angle
      }
      //C
    }
    else if (secth == 6) {
      //C constraint phi an
      if (phipid2 >  - 25.0f && phipid2 < 25.0f) {
        identifier_const = (parms_pip_cs6 + parms_pip_bs6 * thetapid + parms_pip_as6 * pow2(
          thetapid)) / 10.f;
        linear = (parms_pip_lcs6 + parms_pip_lbs6 * thetapid + parms_pip_las6 * pow2(thetapid)) / 100.f;
        quadratic = (parms_pip_qcs6 + parms_pip_qbs6 * thetapid + parms_pip_qas6 * pow2(
          thetapid)) / 1000.f;
        thirdorder = (parms_pip_tcs6 + parms_pip_tbs6 * thetapid + parms_pip_tas6 * pow2(
          thetapid)) / 10000.f;
        fourthorder = (parms_pip_hcs6 + parms_pip_hbs6 * thetapid + parms_pip_has6 * pow2(
          thetapid)) / 100000.f;
        //C
        pinew = (identifier_const + linear * phipid2 + quadratic *
          pow2(phipid2) + thirdorder * pow3(phipid2) +
          fourthorder * pow4(phipid2));
        nthetapid = thetapid + pinew;
        //C constraint phi angle
      }
      //C
    }
    //C
  }
  else if (thetapid <= 12.f || thetapid >= 25.f) {
    //C
    if (secth == 1) {
      nthetapid = thetapid;
    }
    else if (secth == 2) {
      nthetapid = thetapid;
    }
    else if (secth == 3) {
      nthetapid = thetapid;
    }
    else if (secth == 4) {
      nthetapid = thetapid;
    }
    else if (secth == 5) {
      nthetapid = thetapid;
    }
    else if (secth == 6) {
      nthetapid = thetapid;
    }
    //C
    //C electronn theta angle limitation
  }
  //C
  nthetapid = nthetapid;
  //C
  //C     ##################  PHI&THETA PION ANGLE CORRECTION  #############END
  //C
  //C     ************** magnetic field consideration **************
  //C
  float bratio = torcur / tormax;
  //C
  //C     ##########  PHI&THETA PIONS MOMENTUM CORRECTION  #############BEGIN
  newpip = mom_pip;
  //C
  float hconst;
  float hlinear;
  float hquadratic;
  float hthirdorder;
  float hfourthorder;
  float hnew;
  if (nthetapid >= 12.f && nthetapid <= 30.f) {
    //C
    if (secth == 1) {
      if (phipid2 >  - 25.f && phipid2 < 25.f) {
        hconst = (parms_pip_ppcs1 + parms_pip_ppbs1 * nthetapid + parms_pip_ppas1 * pow2(
          nthetapid)) * 10.f;
        hlinear = (parms_pip_pplcs1 + parms_pip_pplbs1 * nthetapid + parms_pip_pplas1 * pow2(
          nthetapid)) / 100.f;
        hquadratic = (parms_pip_ppqcs1 + parms_pip_ppqbs1 * nthetapid + parms_pip_ppqas1 *
          pow2(nthetapid)) / 1000.f;
        hthirdorder = (parms_pip_pptcs1 + parms_pip_pptbs1 * nthetapid + parms_pip_pptas1 *
          pow2(nthetapid)) / 10000.f;
        hfourthorder = (parms_pip_pphcs1 + parms_pip_pphbs1 * nthetapid + parms_pip_pphas1 *
          pow2(nthetapid)) / 100000.f;
        //C
        hnew = (hconst + hlinear * phipid2 + hquadratic * pow2(
          phipid2) + hthirdorder * pow3(phipid2) +
          hfourthorder * pow4(phipid2));
        newpip = mom_pip * hnew * bratio;
      }
      else {
        newpip = mom_pip;
      }
      //C
    }
    else if (secth == 2) {
      if (phipid2 >  - 25.f && phipid2 < 25.f) {
        hconst = (parms_pip_ppcs2 + parms_pip_ppbs2 * nthetapid + parms_pip_ppas2 * pow2(
          nthetapid)) * 10.f;
        hlinear = (parms_pip_pplcs2 + parms_pip_pplbs2 * nthetapid + parms_pip_pplas2 * pow2(
          nthetapid)) / 100.f;
        hquadratic = (parms_pip_ppqcs2 + parms_pip_ppqbs2 * nthetapid + parms_pip_ppqas2 *
          pow2(nthetapid)) / 1000.f;
        hthirdorder = (parms_pip_pptcs2 + parms_pip_pptbs2 * nthetapid + parms_pip_pptas2 *
          pow2(nthetapid)) / 10000.f;
        hfourthorder = (parms_pip_pphcs2 + parms_pip_pphbs2 * nthetapid + parms_pip_pphas2 *
          pow2(nthetapid)) / 100000.f;
        //C
        hnew = (hconst + hlinear * phipid2 + hquadratic * pow2(
          phipid2) + hthirdorder * pow3(phipid2) +
          hfourthorder * pow4(phipid2));
        newpip = mom_pip * hnew * bratio;
      }
      else {
        newpip = mom_pip;
      }
      //C
    }
    else if (secth == 3) {
      if (phipid2 >  - 25.f && phipid2 < 25.f) {
        hconst = (parms_pip_ppcs3 + parms_pip_ppbs3 * nthetapid + parms_pip_ppas3 * pow2(
          nthetapid)) * 10.f;
        hlinear = (parms_pip_pplcs3 + parms_pip_pplbs3 * nthetapid + parms_pip_pplas3 * pow2(
          nthetapid)) / 100.f;
        hquadratic = (parms_pip_ppqcs3 + parms_pip_ppqbs3 * nthetapid + parms_pip_ppqas3 *
          pow2(nthetapid)) / 1000.f;
        hthirdorder = (parms_pip_pptcs3 + parms_pip_pptbs3 * nthetapid + parms_pip_pptas3 *
          pow2(nthetapid)) / 10000.f;
        hfourthorder = (parms_pip_pphcs3 + parms_pip_pphbs3 * nthetapid + parms_pip_pphas3 *
          pow2(nthetapid)) / 100000.f;
        //C
        hnew = (hconst + hlinear * phipid2 + hquadratic * pow2(
          phipid2) + hthirdorder * pow3(phipid2) +
          hfourthorder * pow4(phipid2));
        newpip = mom_pip * hnew * bratio;
      }
      else {
        newpip = mom_pip;
      }
      //C
    }
    else if (secth == 4) {
      if (phipid2 >  - 25.f && phipid2 < 25.f) {
        hconst = (parms_pip_ppcs4 + parms_pip_ppbs4 * nthetapid + parms_pip_ppas4 * pow2(
          nthetapid)) * 10.f;
        hlinear = (parms_pip_pplcs4 + parms_pip_pplbs4 * nthetapid + parms_pip_pplas4 * pow2(
          nthetapid)) / 100.f;
        hquadratic = (parms_pip_ppqcs4 + parms_pip_ppqbs4 * nthetapid + parms_pip_ppqas4 *
          pow2(nthetapid)) / 1000.f;
        hthirdorder = (parms_pip_pptcs4 + parms_pip_pptbs4 * nthetapid + parms_pip_pptas4 *
          pow2(nthetapid)) / 10000.f;
        hfourthorder = (parms_pip_pphcs4 + parms_pip_pphbs4 * nthetapid + parms_pip_pphas4 *
          pow2(nthetapid)) / 100000.f;
        //C
        hnew = (hconst + hlinear * phipid2 + hquadratic * pow2(
          phipid2) + hthirdorder * pow3(phipid2) +
          hfourthorder * pow4(phipid2));
        newpip = mom_pip * hnew * bratio;
      }
      else {
        newpip = mom_pip;
      }
      //C
    }
    else if (secth == 5) {
      if (phipid2 >  - 25.f && phipid2 < 25.f) {
        hconst = (parms_pip_ppcs5 + parms_pip_ppbs5 * nthetapid + parms_pip_ppas5 * pow2(
          nthetapid)) * 10.f;
        hlinear = (parms_pip_pplcs5 + parms_pip_pplbs5 * nthetapid + parms_pip_pplas5 * pow2(
          nthetapid)) / 100.f;
        hquadratic = (parms_pip_ppqcs5 + parms_pip_ppqbs5 * nthetapid + parms_pip_ppqas5 *
          pow2(nthetapid)) / 1000.f;
        hthirdorder = (parms_pip_pptcs5 + parms_pip_pptbs5 * nthetapid + parms_pip_pptas5 *
          pow2(nthetapid)) / 10000.f;
        hfourthorder = (parms_pip_pphcs5 + parms_pip_pphbs5 * nthetapid + parms_pip_pphas5 *
          pow2(nthetapid)) / 100000.f;
        //C
        hnew = (hconst + hlinear * phipid2 + hquadratic * pow2(
          phipid2) + hthirdorder * pow3(phipid2) +
          hfourthorder * pow4(phipid2));
        newpip = mom_pip * hnew * bratio;
      }
      else {
        newpip = mom_pip;
      }
      //C
    }
    else if (secth == 6) {
      if (phipid2 >  - 25.f && phipid2 < 25.f) {
        hconst = (parms_pip_ppcs6 + parms_pip_ppbs6 * nthetapid + parms_pip_ppas6 * pow2(
          nthetapid)) * 10.f;
        hlinear = (parms_pip_pplcs6 + parms_pip_pplbs6 * nthetapid + parms_pip_pplas6 * pow2(
          nthetapid)) / 100.f;
        hquadratic = (parms_pip_ppqcs6 + parms_pip_ppqbs6 * nthetapid + parms_pip_ppqas6 *
          pow2(nthetapid)) / 1000.f;
        hthirdorder = (parms_pip_pptcs6 + parms_pip_pptbs6 * nthetapid + parms_pip_pptas6 *
          pow2(nthetapid)) / 10000.f;
        hfourthorder = (parms_pip_pphcs6 + parms_pip_pphbs6 * nthetapid + parms_pip_pphas6 *
          pow2(nthetapid)) / 100000.f;
        //C
        hnew = (hconst + hlinear * phipid2 + hquadratic * pow2(
          phipid2) + hthirdorder * pow3(phipid2) +
          hfourthorder * pow4(phipid2));
        newpip = mom_pip * hnew * bratio;
      }
      else {
        newpip = mom_pip;
      }
      //C
    }
  }
  else {
    //C
    if (secth == 1) {
      newpip = mom_pip;
    }
    else if (secth == 2) {
      newpip = mom_pip;
    }
    else if (secth == 3) {
      newpip = mom_pip;
    }
    else if (secth == 4) {
      newpip = mom_pip;
    }
    else if (secth == 5) {
      newpip = mom_pip;
    }
    else if (secth == 6) {
      newpip = mom_pip;
    }
    //C
  }
  //C
  //C     ##########  PHI&THETA PIONS MOMENTUM CORRECTION  #############END
  newpip = newpip;
}

} // namespace MomCorr_E16
