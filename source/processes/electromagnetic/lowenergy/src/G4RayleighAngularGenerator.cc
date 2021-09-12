//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
// -------------------------------------------------------------------
//
// GEANT4 Class file
//
// File name:     G4RayleighAngularGenerator
//
// Author:        V. Ivanchenko using design of existing 
//                interface 
// 
// Creation date: 31 May 2012
//
// Modifications: 
//
// Class Description: Class are tabulated data according modified formula
//                    modified fit formulas from Dermott E. Cullen, 
//                    Nucl. Instrum. Meth. Phys. Res. B v.101, (4),499-510. 
//                    
//
// Class for Rayleigh scattering generation
//
// Class Description: End 
// -------------------------------------------------------------------
//
//    

#include "G4RayleighAngularGenerator.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"
#include "G4Log.hh"
#include "G4Exp.hh"

using namespace std;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4RayleighAngularGenerator::G4RayleighAngularGenerator()
  : G4VEmAngularDistribution("CullenGenerator")
{
  G4double x = cm/(h_Planck*c_light);
  fFactor = 0.5*x*x;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4RayleighAngularGenerator::~G4RayleighAngularGenerator() 
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... ....oooOO0OOooo....

G4ThreeVector& 
G4RayleighAngularGenerator::SampleDirection(const G4DynamicParticle* dp,
					    G4double, G4int Z,
					    const G4Material*)
{
  G4double ekin = dp->GetKineticEnergy();
  G4double xx = fFactor*ekin*ekin;

  G4double n0 = PP6[Z] - 1.0;
  G4double n1 = PP7[Z] - 1.0;
  G4double n2 = PP8[Z] - 1.0;
  G4double b0 = PP3[Z];
  G4double b1 = PP4[Z];
  G4double b2 = PP5[Z];

  static const G4double numlim = 0.02;
  G4double x  = 2.*xx*b0;
  G4double w0 = (x < numlim) ? n0*x*(1.0 - 0.5*(n0 - 1.0)*x*(1.0 - (n0 - 2.0)*x/3.))
    : 1.0 - G4Exp(-n0*G4Log(1.0 + x)); 

  x  = 2.*xx*b1;
  G4double w1 = (x < numlim) ? n1*x*(1.0 - 0.5*(n1 - 1.0)*x*(1.0 - (n1 - 2.0)*x/3.))
    : 1.0 - G4Exp(-n1*G4Log(1.0 + x));
 
  x  = 2.*xx*b2;
  G4double w2 = (x < numlim) ? n2*x*(1.0 - 0.5*(n2 - 1.0)*x*(1.0 - (n2 - 2.0)*x/3.))
    : 1.0 - G4Exp(-n2*G4Log(1.0 + x));

  G4double x0= w0*PP0[Z]/(b0*n0);
  G4double x1= w1*PP1[Z]/(b1*n1);
  G4double x2= w2*PP2[Z]/(b2*n2);

  G4double cost;
  do {
    G4double w = w0;
    G4double n = n0;
    G4double b = b0;
  
    x = G4UniformRand()*(x0 + x1 + x2);
    if(x > x0) {
      x -= x0;
      if(x <= x1 ) {
	w = w1;
	n = n1;
	b = b1;
      } else { 
	w = w2;
	n = n2;
	b = b2;
      }
    }
    n = 1.0/n;

    // sampling of angle
    G4double y = w*G4UniformRand();
    if(y < numlim) { x = y*n*( 1. + 0.5*(n + 1.)*y*(1. - (n + 2.)*y/3.)); }   
    else           { x = G4Exp(-n*G4Log(1. - y)) - 1.0; }
    cost = 1.0 - x/(b*xx);
  } while (2*G4UniformRand() > 1.0 + cost*cost || cost < -1.0);

  G4double phi  = twopi*G4UniformRand();
  G4double sint = sqrt((1. - cost)*(1.0 + cost));
  fLocalDirection.set(sint*cos(phi),sint*sin(phi),cost);
  fLocalDirection.rotateUz(dp->GetMomentumDirection());

  return fLocalDirection;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... ....oooOO0OOooo....

const G4double
G4RayleighAngularGenerator::PP0[101] = { 0, 
0, 2., 5.21459, 10.2817, 3.66207, 3.63903, 3.71155, 36.5165, 3.43548, 3.40045,     // 1-10 
2.87811, 3.35541, 3.21141, 2.95234, 3.02524, 126.146, 175.044, 162, 296.833, 300.994,     // 11-20 
373.186, 397.823, 430.071, 483.293, 2.14885, 335.553, 505.422, 644.739, 737.017, 707.575,     // 21-30 
3.8094, 505.957, 4.10347, 574.665, 15.5277, 10.0991, 4.95013, 16.3391, 6.20836, 3.52767,     // 31-40 
2.7763, 2.19565, 12.2802, 965.741, 1011.09, 2.85583, 3.65673, 225.777, 1.95284, 15.775,     // 41-50 
39.9006, 3.7927, 64.7339, 1323.91, 3.73723, 2404.54, 28.3408, 29.9869, 217.128, 71.7138,     // 51-60 
255.42, 134.495, 3364.59, 425.326, 449.405, 184.046, 3109.04, 193.133, 3608.48, 152.967,     // 61-70 
484.517, 422.591, 423.518, 393.404, 437.172, 432.356, 478.71, 455.097, 495.237, 417.8,     // 71-80 
3367.95, 3281.71, 3612.56, 3368.73, 3407.46, 40.2866, 641.24, 826.44, 3579.13, 4916.44,     // 81-90 
930.184, 887.945, 3490.96, 4058.6, 3068.1, 3898.32, 1398.34, 5285.18, 1, 872.368,     // 91-100 
};

const G4double
G4RayleighAngularGenerator::PP1[101] = { 0, 
1., 2., 3.7724, 2.17924, 11.9967, 17.7772, 23.5265, 23.797, 39.9937, 46.7748,     // 1-10 
60, 68.6446, 81.7887, 98, 112, 128, 96.7939, 162, 61.5575, 96.4218,     // 11-20 
65.4084, 83.3079, 96.2889, 90.123, 312, 338, 181.943, 94.3868, 54.5084, 132.819,     // 21-30 
480, 512, 544, 578, 597.472, 647.993, 682.009, 722, 754.885, 799.974,     // 31-40 
840, 882, 924, 968, 1012, 1058, 1104, 1151.95, 1199.05, 1250,     // 41-50 
1300, 1352, 1404, 1458, 1512, 729.852, 1596.66, 1682, 1740, 1800,     // 51-60 
1605.79, 1787.51, 603.151, 2048, 2112, 1993.95, 334.907, 2312, 885.149, 2337.19,     // 61-70 
2036.48, 2169.41, 2241.49, 2344.6, 2812, 2888, 2964, 2918.04, 2882.97, 2938.74,     // 71-80 
2716.13, 511.66, 581.475, 594.305, 672.232, 3657.71, 3143.76, 3045.56, 3666.7, 1597.84,     // 81-90 
3428.87, 3681.22, 1143.31, 1647.17, 1444.9, 1894.33, 3309.12, 2338.59, 4900, 4856.61,     // 91-100 
};

const G4double
G4RayleighAngularGenerator::PP2[101] = { 0, 
0, 0, 0.0130091, 3.53906, 9.34125, 14.5838, 21.7619, 3.68644, 37.5709, 49.8248,     // 1-10 
58.1219, 72, 83.9999, 95.0477, 109.975, 1.85351, 17.1623, 0, 2.60927, 2.58422,     // 11-20 
2.4053, 2.86948, 2.63999, 2.58417, 310.851, 2.44683, 41.6348, 44.8739, 49.4746, 59.6053,     // 21-30 
477.191, 6.04261, 540.897, 3.33531, 612, 637.908, 682.041, 705.661, 759.906, 796.498,     // 31-40 
838.224, 879.804, 912.72, 2.25892, 1.90993, 1055.14, 1101.34, 926.275, 1200, 1234.23,     // 41-50 
1261.1, 1348.21, 1340.27, 134.085, 1509.26, 1.60851, 1624, 1652.01, 1523.87, 1728.29,     // 51-60 
1859.79, 1922, 1.25916, 1622.67, 1663.6, 2178, 1045.05, 2118.87, 267.371, 2409.84,     // 61-70 
2520, 2592, 2664, 2738, 2375.83, 2455.64, 2486.29, 2710.86, 2862.79, 3043.46,     // 71-80 
476.925, 2930.63, 2694.96, 3092.96, 3145.31, 3698, 3784, 3872, 675.166, 1585.71,     // 81-90 
3921.95, 3894.83, 4014.73, 3130.23, 4512, 3423.35, 4701.53, 1980.23, 4900, 4271.02,     // 91-100 
};

const G4double
G4RayleighAngularGenerator::PP3[101] = { 0, 
1.53728e-16, 2.95909e-16, 1.95042e-15, 6.24521e-16, 4.69459e-17, 3.1394e-17, 2.38808e-17, 3.59428e-16, 1.2947e-17, 1.01182e-17,     // 1-10 
6.99543e-18, 6.5138e-18, 5.24063e-18, 4.12831e-18, 4.22067e-18, 2.12802e-16, 3.27035e-16, 2.27705e-16, 1.86943e-15, 8.10577e-16,     // 11-20 
1.80541e-15, 9.32266e-16, 5.93459e-16, 4.93049e-16, 5.03211e-19, 2.38223e-16, 4.5181e-16, 5.34468e-16, 5.16504e-16, 3.0641e-16,     // 21-30 
1.24646e-18, 2.13805e-16, 1.21448e-18, 2.02122e-16, 5.91556e-18, 3.4609e-18, 1.39331e-18, 5.47242e-18, 1.71017e-18, 7.92438e-19,     // 31-40 
4.72225e-19, 2.74825e-19, 4.02137e-18, 1.6662e-16, 1.68841e-16, 4.73202e-19, 7.28319e-19, 3.64382e-15, 1.53323e-19, 4.15409e-18,     // 41-50 
7.91645e-18, 6.54036e-19, 1.04123e-17, 9.116e-17, 5.97268e-19, 1.23272e-15, 5.83259e-18, 5.42458e-18, 2.20137e-17, 1.19654e-17,     // 51-60 
2.3481e-17, 1.53337e-17, 8.38225e-16, 3.40248e-17, 3.50901e-17, 1.95115e-17, 2.91803e-16, 1.98684e-17, 3.59425e-16, 1.54e-17,     // 61-70 
3.04174e-17, 2.71295e-17, 2.6803e-17, 2.36469e-17, 2.56818e-17, 2.50364e-17, 2.6818e-17, 2.56229e-17, 2.7419e-17, 2.27442e-17,     // 71-80 
1.38078e-15, 1.49595e-15, 1.20023e-16, 1.74446e-15, 1.82836e-15, 5.80108e-18, 3.02324e-17, 3.71029e-17, 1.01058e-16, 4.87707e-16,     // 81-90 
4.18953e-17, 4.03182e-17, 1.11553e-16, 9.51125e-16, 2.57569e-15, 1.14294e-15, 2.98597e-15, 5.88714e-16, 1.46196e-20, 1.53226e-15,     // 91-100 
};

const G4double
G4RayleighAngularGenerator::PP4[101] = { 0, 
1.10561e-15, 3.50254e-16, 1.56836e-16, 7.86286e-15, 2.2706e-16, 7.28454e-16, 4.54123e-16, 8.03792e-17, 4.91833e-16, 1.45891e-16,     // 1-10 
1.71829e-16, 3.90707e-15, 2.76487e-15, 4.345e-16, 6.80131e-16, 4.04186e-16, 8.95703e-17, 3.32136e-16, 1.3847e-17, 4.16869e-17,     // 11-20 
1.37963e-17, 1.96187e-17, 2.93852e-17, 2.46581e-17, 4.49944e-16, 3.80311e-16, 1.62925e-15, 7.52449e-16, 9.45445e-16, 5.47652e-16,     // 21-30 
6.89379e-16, 1.37078e-15, 1.22209e-15, 1.13856e-15, 9.06914e-16, 8.77868e-16, 9.70871e-16, 1.8532e-16, 1.69254e-16, 1.14059e-15,     // 31-40 
7.90712e-16, 5.36611e-16, 8.27932e-16, 2.4329e-16, 5.82899e-16, 1.97595e-16, 1.96263e-16, 1.73961e-16, 1.62174e-16, 5.31143e-16,     // 41-50 
5.29731e-16, 4.1976e-16, 4.91842e-16, 4.67937e-16, 4.32264e-16, 6.91046e-17, 1.62962e-16, 9.87241e-16, 1.04526e-15, 1.05819e-15,     // 51-60 
1.10579e-16, 1.49116e-16, 4.61021e-17, 1.5143e-16, 1.53667e-16, 1.67844e-15, 2.7494e-17, 2.31253e-16, 2.27211e-15, 1.33401e-15,     // 61-70 
9.02548e-16, 1.77743e-15, 1.76608e-15, 9.45054e-16, 1.06805e-16, 1.06085e-16, 1.01688e-16, 1.0226e-16, 7.7793e-16, 8.0166e-16,     // 71-80 
9.18595e-17, 2.73428e-17, 3.01222e-17, 3.09814e-17, 3.39028e-17, 1.49653e-15, 1.19511e-15, 1.40408e-15, 2.37226e-15, 8.35973e-17,     // 81-90 
1.4089e-15, 1.2819e-15, 4.96925e-17, 6.04886e-17, 7.39507e-17, 6.6832e-17, 1.09433e-16, 9.61804e-17, 1.38525e-16, 2.49104e-16,     // 91-100 
};

const G4double
G4RayleighAngularGenerator::PP5[101] = { 0, 
6.89413e-17, 2.11456e-17, 2.47782e-17, 7.01557e-17, 1.01544e-15, 1.76177e-16, 1.28191e-16, 1.80511e-17, 1.96803e-16, 3.16753e-16,     // 1-10 
1.21362e-15, 6.6366e-17, 8.42625e-17, 1.01935e-16, 1.34162e-16, 1.87076e-18, 2.76259e-17, 1.2217e-16, 1.66059e-18, 1.76249e-18,     // 11-20 
1.13734e-18, 1.58963e-18, 1.33987e-18, 1.18496e-18, 2.44536e-16, 6.69957e-19, 2.5667e-17, 2.62482e-17, 2.55816e-17, 2.6574e-17,     // 21-30 
2.26522e-16, 2.17703e-18, 2.07434e-16, 8.8717e-19, 1.75583e-16, 1.81312e-16, 1.83716e-16, 2.58371e-15, 1.74416e-15, 1.7473e-16,     // 31-40 
1.76817e-16, 1.74757e-16, 1.6739e-16, 2.68691e-19, 1.8138e-19, 1.60726e-16, 1.59441e-16, 1.36927e-16, 2.70127e-16, 1.63371e-16,     // 41-50 
1.29776e-16, 1.49012e-16, 1.17301e-16, 1.67919e-17, 1.47596e-16, 1.14246e-19, 1.10392e-15, 1.58755e-16, 1.11706e-16, 1.80135e-16,     // 51-60 
1.00213e-15, 9.44133e-16, 4.722e-20, 1.18997e-15, 1.16311e-15, 2.31716e-16, 1.86238e-15, 1.53632e-15, 2.45853e-17, 2.08069e-16,     // 61-70 
1.08659e-16, 1.29019e-16, 1.24987e-16, 1.07865e-16, 1.03501e-15, 1.05211e-15, 9.38473e-16, 8.66912e-16, 9.3778e-17, 9.91467e-17,     // 71-80 
2.58481e-17, 9.72329e-17, 9.77921e-16, 1.02928e-16, 1.01767e-16, 1.81276e-16, 1.07026e-16, 1.11273e-16, 3.25695e-17, 1.77629e-15,     // 81-90 
1.18382e-16, 1.111e-16, 1.56996e-15, 8.45221e-17, 3.6783e-16, 1.20652e-16, 3.91104e-16, 3.52282e-15, 4.29979e-16, 1.28308e-16,     // 91-100 
};

const G4double
G4RayleighAngularGenerator::PP6[101] = { 0, 
6.57834, 3.91446, 7.59547, 10.707, 3.97317, 4.00593, 3.93206, 8.10644, 3.97743, 4.04641,     // 1-10 
4.30202, 4.19399, 4.27399, 4.4169, 4.04829, 2.21745, 11.3523, 1.84976, 1.61905, 3.68297,     // 11-20 
1.5704, 2.58852, 3.59827, 3.61633, 9.07174, 1.76738, 1.97272, 1.91032, 1.9838, 2.64286,     // 21-30 
4.16296, 1.80149, 3.94257, 1.72731, 2.27523, 2.57383, 3.33453, 2.2361, 2.94376, 3.91332,     // 31-40 
5.01832, 6.8016, 2.19508, 1.65926, 1.63781, 4.23097, 3.4399, 2.55583, 7.96814, 2.06573,     // 41-50 
1.84175, 3.23516, 1.79129, 2.90259, 3.18266, 1.51305, 1.88361, 1.91925, 1.68033, 1.72078,     // 51-60 
1.66246, 1.66676, 1.49394, 1.58924, 1.57558, 1.63307, 1.84447, 1.60296, 1.56719, 1.62166,     // 61-70 
1.5753, 1.57329, 1.558, 1.57567, 1.55612, 1.54607, 1.53251, 1.51928, 1.50265, 1.52445,     // 71-80 
1.4929, 1.51098, 2.52959, 1.42334, 1.41292, 2.0125, 1.45015, 1.43067, 2.6026, 1.39261,     // 81-90 
1.38559, 1.37575, 2.53155, 2.51924, 1.32386, 2.31791, 2.47722, 1.33584, 9.60979, 6.84949,     // 91-100 
};

const G4double
G4RayleighAngularGenerator::PP7[101] = { 0, 
3.99983, 6.63093, 3.85593, 1.69342, 14.7911, 7.03995, 8.89527, 13.1929, 4.93354, 5.59461,     // 1-10 
3.98033, 1.74578, 2.67629, 14.184, 8.88775, 13.1809, 4.51627, 13.7677, 9.53727, 4.04257,     // 11-20 
7.88725, 5.78566, 4.08148, 4.18194, 7.96292, 8.38322, 3.31429, 13.106, 13.0857, 13.1053,     // 21-30 
3.54708, 2.08567, 2.38131, 2.58162, 3.199, 3.20493, 3.19799, 1.88697, 1.80323, 3.15596,     // 31-40 
4.10675, 5.68928, 3.93024, 11.2607, 4.86595, 12.1708, 12.2867, 9.29496, 1.61249, 5.0998,     // 41-50 
5.25068, 6.67673, 5.82498, 6.12968, 6.94532, 1.71622, 1.63028, 3.34945, 2.84671, 2.66325,     // 51-60 
2.73395, 1.93715, 1.72497, 2.74504, 2.71531, 1.52039, 1.58191, 1.61444, 2.67701, 1.51369,     // 61-70 
2.60766, 1.46608, 1.49792, 2.49166, 2.84906, 2.80604, 2.92788, 2.76411, 2.59305, 2.5855,     // 71-80 
2.80503, 1.4866, 1.46649, 1.45595, 1.44374, 1.54865, 2.45661, 2.43268, 1.35352, 1.35911,     // 81-90 
2.26339, 2.26838, 1.35877, 1.37826, 1.3499, 1.36574, 1.33654, 1.33001, 1.37648, 4.28173,     // 91-100 
};

const G4double
G4RayleighAngularGenerator::PP8[101] = { 0, 
4, 4, 5.94686, 4.10265, 7.87177, 12.0509, 12.0472, 3.90597, 5.34338, 6.33072,     // 1-10 
2.76777, 7.90099, 5.58323, 4.26372, 3.3005, 5.69179, 2.3698, 3.68167, 5.2807, 4.61212,     // 11-20 
5.87809, 4.46207, 4.59278, 4.67584, 1.75212, 7.00575, 2.05428, 2.00415, 2.02048, 1.98413,     // 21-30 
1.71725, 3.18743, 1.74231, 4.40997, 2.01626, 1.8622, 1.7544, 1.60332, 2.23338, 1.70932,     // 31-40 
1.67223, 1.64655, 1.76198, 6.33416, 7.92665, 1.67835, 1.67408, 1.55895, 9.3642, 1.68776,     // 41-50 
2.02167, 1.65401, 2.20616, 1.76498, 1.63064, 7.13771, 3.17033, 1.65236, 2.66943, 1.62703,     // 51-60 
2.72469, 2.73686, 10.86, 2.76759, 2.69728, 1.62436, 2.76662, 1.48514, 1.57342, 1.61518,     // 61-70 
3.18455, 2.73467, 2.72521, 2.786, 2.35611, 2.31574, 2.5787, 2.46877, 2.89052, 2.6478,     // 71-80 
1.50419, 2.73998, 2.79809, 2.66207, 2.73089, 1.34835, 2.59656, 2.7006, 1.41867, 4.26255,     // 81-90 
2.47985, 2.47126, 1.72573, 3.44856, 1.36451, 2.8715, 2.35731, 1.28196, 4.1224, 1.32633,     // 91-100 
};
 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... ....oooOO0OOooo....
