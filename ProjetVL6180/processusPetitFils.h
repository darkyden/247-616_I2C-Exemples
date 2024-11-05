#ifndef PROCESSUSPETITFILS_H
#define PROCESSUSPETITFILS_H

//Description: processus qui lit périodiquement la distance mesurée par un VL6180x
//Le temps approximatifs entre les lectures doit être supérieur au temps requis
//pour compléter les échanges i2c avec le VL6180x

//Connectez le VL6180x au BbB de la façon suivante:
//broches: 1 = sda, 2 = scl, 3 = 3.3V, 4 = gnd

//         VL6180x                  connecteur I2C du bbb
//      vue de dessus               
//       _____________               ________________    
//      |             |             |                |
//      |             |             |sda scl 3.3v gnd|
//      |   _  _  _   |             |________________|
//      |  |_||_||_|  |
//      |             |
//      |scl sda  gnd |
//      |int gpio vdd |
//      |_____________|
//
// breadboard: 
// VL6180x    scl    sda    gnd    vdd    gpio
//             |      |      |      |        |
//             |      |      |      |_/\/\/\_|
//             |      |      |      |  10Kohms  
//             |      |      |      |    
// i2c du BBB scl    sda    gnd    3.3v

//dépendances logicielles
//copiez et adaptez la ligne suivante dans main.h
//#define PROCESSUSPETITFILS_TEMPS_ENTRE_LES_LECTURES_DE_DISTANCE_EN_MS 1


// function declarations
int processusPetitFils_initialise(void);
int processusPetitFils_gere(void);

#endif