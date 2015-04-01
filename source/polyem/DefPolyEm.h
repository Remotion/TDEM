#ifndef __DEFPOLYEM_H
#define __DEFPOLYEM_H

//-------------------------------------------
#define POLYEM_ID			1009325 //POLYEM
//-------------------------------------------

//------------------------------------
//Emitter
#define PLE_EMITTERNAME		1011//String
#define PLE_NUM_EMITTER		1012//LONG

//Quele
#define PLE_OBJECTNAME		1021//String
#define PLE_NUM_OBJECT		1022//LONG
#define PLE_SEL_OBJECT		1023//String
#define PLE_SEL_NUM_OBJECT	1024//LONG
//Q Vertex
#define PLE_VERTEX			1031//String
#define PLE_VERTEX_NUM		1032//LONG

//Ziel
/*
#define PLE_DESTLOCROT		1040
#define PLE_USEDEST			1041//Bool
#define PLE_LOCALDES		1042//Bool
#define PLE_DESTINATION		1043//String
#define PLE_NUM_DESTIN		1044//LONG
#define PLE_SEL_DESTIN		1045//String
#define PLE_SEL_NUM_DESTIN	1046//LONG
#define PLE_USESPCOR		1047
#define PLE_DESTALWAYS		1048
#define PLE_DEST_TYP		1049
#define PLE_DEST_SPEED		1050

//Pfad
#define PLE_USEPATH			1051//Bool
#define PLE_LOCALPATH		1052//Bool
#define PLE_PATH		    1053//String
#define PLE_PATH_NUM	    1054//LONG
#define PLE_PATH_SPEED	    1055
#define PLE_PATH_TYP	    1056
#define PLE_PATHLOCROT	    1057
*/

//rest
#define PLE_DISPERSING_X	1061//Real
#define PLE_DISPERSING_Y	1062//Real
#define PLE_DISPERSING_Z	1063//Real
#define PLE_DIRECTION_TYP	1064//LONG
#define PLE_RANDOM			1065
#define PLE_SPEED			1066
#define PLE_SEED			1067
#define PLE_VISIBLE			1068 //Bool



#define PLE_SPEEDCHAN		1071 //LONG IDC_PLE_CHA1
#define PLE_SPEEDFIL		1072 //LONG IDC_PLE_TEX_N1

#define PLE_VISICHAN		1081 //LONG IDC_PLE_CHA2
#define PLE_VISIFIL			1082 //LONG IDC_PLE_TEX_N2

#define PLE_ANGLECHAN		1091 //LONG IDC_PLE_CHA3
#define PLE_ANGLEFIL		1092 //LONG IDC_PLE_TEX_N3

#define PLE_OFFCHAN			1101 //LONG IDC_PLE_CHA4
#define PLE_OFFFIL			1102 //LONG IDC_PLE_TEX_N4
#define PLE_OFFMAX			1103 //Real



#define PLE_CHANNEL0		1200
#define PLE_CHANNEL1		1201
#define PLE_CHANNEL2		1202
#define PLE_CHANNEL3		1203
#define PLE_CHANNEL4		1204
#define PLE_CHANNEL5		1205
#define PLE_CHANNEL6		1206
#define PLE_CHANNEL7		1207
#define PLE_CHANNEL8		1208

#define PLE_TEXTURE0		1210
#define PLE_TEXTURE1		1211
#define PLE_TEXTURE2		1212
#define PLE_TEXTURE3		1213
#define PLE_TEXTURE4		1214
#define PLE_TEXTURE5		1215
#define PLE_TEXTURE6		1216
#define PLE_TEXTURE7		1217
#define PLE_TEXTURE8		1218

#endif//end