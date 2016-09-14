import root_numpy
import numpy as np
import math

bnames = ['evntid', 'gpart', 'id', 'q', 'p', 'cx', 'cy', 'cz',
          'm', 'b', 'vx', 'vy', 'vz', 'tr_time', 'q_l', 't_l',
          'sc', 'sc_sect', 'sc_pd', 'sc_t', 'sc_r',
          'cc', 'cc_sect', 'cc_hit', 'cc_segm', 'nphe']

def inv_prod(lv1, lv2):
    '''Minkowski (t, x, y, z) inner product of four-vectors lv1 and lv2.
    '''
    return lv1[0]*lv2[0] - lv1[1]*lv2[1] - lv1[2]*lv2[2] - lv1[3]*lv2[3]

def inv_mass2(lv):
    '''Minkowski (t, x, y, z) quadratic form of four-vector lv.
    Mass squared if lv is four-momentum.
    '''
    return inv_prod(lv, lv)

def inv_mass(lv):
    '''Invariant mass of system with four-momentum lv.
    Or, more generally, signed norm of four-vector lv.
    Four-vectors assumed to be in form (t, x, y, z).
    '''
    m2 = inv_mass2(lv)
    return math.sqrt(m2) if m2 > 0 else -math.sqrt(-m2)

def dot(v1, v2):
    '''Dot product of three-vectors v1 and v2.
    '''
    return v1[0]*v2[0] + v1[1]*v2[1] + v1[2]*v2[2]

def norm(v):
    '''Norm of three-vector v.
    '''
    return math.sqrt(dot(v,v))

def boost(lv, bz):
    '''Lorentz boost of four-vector lv along z-axis only,
    according to beta = bz. Four-vectors assumed to be in
    form (t, x, y, z).
    '''
    g = 1/math.sqrt(1-bz**2)
    L = [ [g,     0, 0, -bz*g],
          [0,     1, 0,     0],
          [0,     0, 1,     0],
          [-bz*g, 0, 0,     g] ]
    return np.dot(L, lv)

def rot3(v, z, y):
    '''Rotation of three-vector v according to new basis
    with z-axis pointing in direction of three-vector z
    and y-zxis in direction of three-vector y.  All input
    three-vectors expressed in original basis (e.g., lab
    frame).
    '''
    z = np.array(z)/norm(z)
    y = np.array(y)/norm(y)
    x = np.cross(y,z)
    x /= norm(x)
    rot = [ x, y, z ]
    return np.dot(rot, v)

def rot4(lv, cm4, V):
    '''Rotation plus Lorentz boost taking four-vector lv from lab
    frame to "helicity" frame in single meson electroproduction.
    Four-momentum cm4 defines the CMS; V is the four-momentum of
    meson.
    '''
    y = np.cross(cm4[1:], V[1:])
    lv[1:] = rot3(lv[1:], cm4[1:], y)
    beta = cm4[3]/cm4[0]
    return boost(lv, beta)

def p_cosTheta_phi(lv, cm4 = None, V = None):
    '''Return energy, momentum, cos(theta), and phi corresponding
    to four-momentum lv.  If cm4 and V are provided, then rotate
    and boost into the helicity frame first.  BOTH or NEITHER of
    cm4 and V are assumed to be provided.  If either is None, then
    no transformation occurs.  Four-momentum cm4 defines the CMS;
    V is the four-momentum of meson.
    '''
    lv = lv if cm4 is None or V is None else rot4(lv, cm4, V)
    p = norm(lv[1:])
    cosTheta = lv[3]/p
    phi = math.atan2(lv[2], lv[1])
    return [lv[0], p, cosTheta, phi]

golden_runs = [
37658, 37659, 37661, 37662, 37664, 37665, 37666, 37667,
37670, 37672, 37673, 37674, 37675, 37677, 37678, 37679,
37680, 37681, 37683, 37684, 37685, 37686, 37687, 37688,
37689, 37690, 37691, 37692, 37693, 37694, 37698, 37699,
37700, 37701, 37702, 37703, 37704, 37705, 37706, 37707,
37708, 37709, 37710, 37711, 37712, 37713, 37714, 37715,
37716, 37717, 37719, 37721, 37722, 37723, 37724, 37725,
37726, 37740, 37744, 37745, 37746, 37747, 37748, 37750,
37753, 37762, 37763, 37766, 37767, 37769, 37770, 37772,
37773, 37775, 37776, 37778, 37780, 37781, 37782, 37783,
37784, 37785, 37788, 37789, 37790, 37801, 37802, 37803,
37804, 37805, 37806, 37807, 37808, 37809, 37810, 37811,
37812, 37813, 37814, 37815, 37816, 37817, 37818, 37819,
37820, 37822, 37823, 37824, 37825, 37828, 37831, 37832,
37833, 37844, 37845, 37846, 37847, 37848, 37849, 37850,
37851, 37852, 38046, 38047, 38048, 38049, 38050, 38051,
38052, 38053, 38070, 38071, 38072, 38074, 38075, 38076,
38077, 38078, 38079, 38080, 38081, 38082, 38083, 38084,
38085, 38086, 38089, 38090, 38091, 38092, 38093, 38094,
38095, 38096, 38097, 38098, 38099, 38100, 38114, 38117,
38118, 38119, 38120, 38121, 38122, 38131, 38132, 38133,
38134, 38135, 38136, 38137, 38138, 38139, 38140, 38141,
38142, 38143, 38144, 38146, 38172, 38173, 38174, 38175,
38176, 38177, 38182, 38183, 38184, 38185, 38186, 38187,
38188, 38189, 38190, 38191, 38192, 38194, 38195, 38196,
38197, 38198, 38199, 38200, 38201, 38203, 38204, 38205,
38206, 38207, 38208, 38209, 38210, 38211, 38212, 38213,
38214, 38215, 38216, 38217, 38218, 38219, 38220, 38221,
38222, 38223, 38225, 38226, 38265, 38266, 38268, 38271,
38272, 38273, 38274, 38275, 38276, 38277, 38278, 38283,
38284, 38285, 38286, 38288, 38289, 38290, 38300, 38301,
38302, 38304, 38305, 38306, 38307, 38309, 38310, 38312,
38313, 38314, 38315, 38317, 38318, 38320, 38322, 38328,
38331, 38337, 38338, 38341, 38342, 38344, 38346, 38347,
38350, 38351, 38353, 38354, 38355, 38356, 38359, 38360,
38364, 38365, 38378, 38379, 38380, 38381, 38382, 38383,
38384, 38385, 38387, 38388, 38389, 38390, 38391, 38392,
38393, 38394, 38395, 38396, 38397, 38398, 38399, 38400,
38401, 38402, 38403, 38404, 38405, 38408, 38409, 38410,
38411, 38412, 38415, 38417, 38418, 38419, 38420, 38421,
38422, 38423, 38430, 38431, 38432, 38433, 38434, 38435,
38436, 38437, 38438, 38440, 38441, 38443, 38446, 38447,
38449, 38450, 38451, 38452, 38453, 38454, 38455, 38456,
38457, 38458, 38459, 38460, 38461, 38462, 38463, 38464,
38465, 38466, 38467, 38468, 38469, 38470, 38471, 38472,
38473, 38474, 38475, 38476, 38477, 38479, 38480, 38483,
38484, 38485, 38486, 38487, 38488, 38489, 38490, 38491,
38492, 38493, 38494, 38495, 38497, 38498, 38499, 38500,
38501, 38507, 38508, 38509, 38510, 38511, 38512, 38513,
38514, 38515, 38516, 38517, 38518, 38519, 38520, 38521,
38522, 38523, 38524, 38525, 38526, 38527, 38528, 38529,
38530, 38531, 38534, 38536, 38537, 38538, 38539, 38540,
38541, 38542, 38543, 38544, 38545, 38548, 38549, 38550,
38551, 38552, 38553, 38554, 38558, 38559, 38560, 38561,
38562, 38563, 38568, 38571, 38572, 38573, 38574, 38575,
38576, 38577, 38578, 38579, 38580, 38581, 38582, 38583,
38584, 38585, 38596, 38597, 38598, 38600, 38601, 38602,
38603, 38604, 38606, 38607, 38608, 38609, 38610, 38611,
38612, 38613, 38614, 38616, 38617, 38618, 38619, 38620,
38621, 38622, 38623, 38624, 38625, 38626, 38627, 38628,
38629, 38630, 38631, 38632, 38633, 38634, 38635, 38636,
38637, 38638, 38639, 38640, 38641, 38642, 38645, 38646,
38647, 38648, 38649, 38650, 38651, 38652, 38653, 38654,
38655, 38656, 38670, 38671, 38672, 38673, 38674, 38675,
38676, 38677, 38681, 38682, 38684, 38685, 38686, 38687,
38689, 38690, 38691, 38692, 38693, 38694, 38696, 38697,
38698, 38699, 38700, 38701, 38702, 38703, 38704, 38705,
38706, 38707, 38708, 38709, 38710, 38711, 38712, 38713,
38714, 38715, 38716, 38717, 38719, 38720, 38721, 38722,
38723, 38724, 38725, 38727, 38728, 38729, 38730, 38731,
38732, 38733, 38734, 38735, 38737, 38738, 38739, 38740,
38743, 38744, 38745, 38746, 38748, 38749, 38750, 38751
]
