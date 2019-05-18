// kalmanFilter.cpp : Este arquivo contém a função 'main'. A execução do programa começa e termina ali.
//

#include "pch.h"
#include <iostream>


typedef struct xpPrediction {
	int meanMatrix = [3]; // matriz de medias das posicoes e velocidades
	int covMatrix = [3]; // matriz de covariancias das posicoes e velocidades 
}xpPrediction;

/*### PREDICTION PART ###

input:
1.covMatrix: matriz de previsao de covariancias;
2.meanMatrix: matriz das medias de posicao e velocidade;
3.noisePar: parametros do filtro de ruido;
A cada iteracao, a prediction part vai usar como input, o output do update part

updatePV = funcao que atualiza posicao p e velocidade v;
transpose(updatePV) = funcao que faz a matriz transposta de updatePV
randomError = vai ser um erro aleatorio/admissivel para o ssl vision*/

xpPrediction predictionPart(xpPrediction oldpred) {
	xpPrediction newMatrix;
	newMatrix.covMatrix = updatePV * oldpred.covMatrix * transpose(updatePV) + noisePar;
	newMatrix.meanMatrix = updatePV * oldpred.meanMatrix + randomError;

	return newMatrix;
}

// ### end prediction part ###

/*### UPDATE PART ###
input:
1. pcovMatrix: matriz de previsao de covariancias;
2. pmeanMatrix: matriz de previsao das medias de posicao e velocidade;
3. noise: medida do ruido;

output:
1. meanMatrix: matriz das medias de posicao e velocidade;
2. covMatrix: matriz das covariancias ;
*/

xpPrediction updatePart(xpPrediction prediction) {
	xpPrediction final;

	difference = polarCoordinates - cartesianToPolarCoordinates(preditiction.meanMatrix); // difference: Difference between Measured Value and Actual Value
	totalError = jacobianMatrix() * prediction.covMatrix * jacobianMatrixtrans + noiseMeasure;
	kalmanGain = prediction.covMatrix * jacobianMatrixtrans * reverseMatrix(totalError);

	final.meanMatrix = prediction.meanMatrix + (KalmanGain * difference) ;
	final.covMatrix = (identityMatrix - (kalmanGain * jacobianMatrix))* prediction.covMatrix;

		return final;
}



