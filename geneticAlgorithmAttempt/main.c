//Program to test a genetic algorithm. Match a string.

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#define TRUE 1
#define FALSE 0

#define POPULATION_SIZE 6000
#define CHROMOSONE_LENGTH sizeof(goalWord) 
#define MUTATION_CHANCE_PERCENTAGE 10

static char goalWord[] = { 'e','m','i','g','r','a','t','e' };
static char population[POPULATION_SIZE][CHROMOSONE_LENGTH];
static char populationToBeCrossed[POPULATION_SIZE / 2][CHROMOSONE_LENGTH];
static char populationPostCoitus[POPULATION_SIZE][CHROMOSONE_LENGTH];
static double fitness[POPULATION_SIZE];

void genPop();
void evaluateFitness();
void selection();
void tornementSelection();
void singlePointCrossover();
void mutation();

int main() {
	/*
	####Psudo-psudo###
	1. Initialise: Create a population of N elements with randomly generated DNA
	2. Evaluation: Each member of the population is then evaluated and a fitness is calculated for that individual
	3. Selection: Discard bad designs and only keep the best individuals in the population.
	4. Crossover: Create new individuals by combining aspects of our selected individuals.
	5. Mutation: Randomise our populations' genetics. 
	6. Repeate: go back to step 2. 

	TERMINATE WHEN GOAL IS ACHIEVED.
	*/

	int geneIndex = 0, chromosoneIndex = 0, generation = 0;
	int taskComple = FALSE;
	

	///Initialise 
	genPop();

	//Start main loop
	while (taskComple != TRUE) {
		///Evaluate
		evaluateFitness();
		
		///Selection
		tornementSelection();

		///Crossover
		singlePointCrossover();

		///Mutation
		mutation();
		

		///Check population for matching chromosone and copy postCoitus population back into the original population. Record Generation.
		for (chromosoneIndex = 0; chromosoneIndex < POPULATION_SIZE; chromosoneIndex++) {
			for (geneIndex = 0; geneIndex < CHROMOSONE_LENGTH; geneIndex++) {
				if (fitness[chromosoneIndex] > 0.79) {///Utter wank. Needs to change
					taskComple = TRUE;
				}
				population[chromosoneIndex][geneIndex] = populationPostCoitus[chromosoneIndex][geneIndex];
			}
		}
		if (generation == 6) {
			printf("");
		}
		
		generation++;
	}//End main loop

	printf("Generation: %d", generation);
	getchar();
	return 0;
}

//Generate random DNA in population array
void genPop(){
	int chromosoneIndex = 0, geneIndex = 0;
	srand(time(NULL));

	for (chromosoneIndex = 0; chromosoneIndex < POPULATION_SIZE; chromosoneIndex++) {
		for (geneIndex = 0; geneIndex < CHROMOSONE_LENGTH; geneIndex++) {
			population[chromosoneIndex][geneIndex] = 'a' + (rand() % 26);
		}
	}
}

//Evaluates fitness. If chromosone has 1 correct gene, fitness += 0.1 (10%), for 2 correct fitness += 0.2(20%) etc.
void evaluateFitness()
{
	int chromosoneIndex = 0, geneIndex = 0;
	
	//intitialize fitness model to zero
	for (chromosoneIndex = 0; chromosoneIndex < POPULATION_SIZE; chromosoneIndex++) {
		fitness[chromosoneIndex] = 0.0;
	}
	
	//Collect fitnesses
	for (chromosoneIndex = 0; chromosoneIndex < POPULATION_SIZE; chromosoneIndex++) {
		for (geneIndex = 0; geneIndex < CHROMOSONE_LENGTH; geneIndex++) {
			if (population[chromosoneIndex][geneIndex] == goalWord[geneIndex]) {
				fitness[chromosoneIndex] += 0.1;
			}
		}
	}
}

//Roulette wheel selection
void selection(){
/*[Sum] Calculate sum of all chromosome fitnesses in population - sum S.
  [Select] Generate random number from interval(0, S) - r.
  [Loop] Go through the population and sum fitnesses from 0 - sum s.
	     When the sum s is greater then r, stop and return the chromosome where you are.
*/
	int chromosoneIndex = 0, geneIndex = 0, popToCross = 0;
	double sum = 0.0; 
	double randInterval = 0.0;
	
	///Sum of all fitnesses
	for (chromosoneIndex = 0; chromosoneIndex < POPULATION_SIZE; chromosoneIndex++) {
		sum += fitness[chromosoneIndex];
	}
	//Select half of the chromosones to be kept for crossover
	while (popToCross != (POPULATION_SIZE / 2)) {
		///Select random number between 0 and sum
		randInterval = (rand() % (int)sum);
		sum = 0;
		int popCheck = 0;
		///Loop through fitnesses, if sum > random Interval return that chromosone
		for (chromosoneIndex = 0; chromosoneIndex < POPULATION_SIZE; chromosoneIndex++) {
			for (geneIndex = 0; geneIndex < CHROMOSONE_LENGTH; geneIndex++) {
				sum += fitness[chromosoneIndex];
				if (sum > randInterval) {
					populationToBeCrossed[popCheck][geneIndex] = population[chromosoneIndex - 1][geneIndex];
					if (geneIndex == CHROMOSONE_LENGTH - 1) {
						popCheck++;
						break;
					}
				}
			//	popToCross++;
			}
			popToCross++;
		}
		//popToCross++;
	}
}

//Tornement Selection
void tornementSelection() {
	/*
	MY TORNEMENT:
	devide population up into N pools.
	if Chromosone1 is has greater value than chromosone2, move chromosone to higher position. Choose highest half for popToCross.
	FOUR SUBGROUPS. FROM 0 TO 1/4 ADD TO GROUP ONE. 1/4 TO 1/2 ADD TO GROUP 2. FROM 1/2 TO 3/4 ADD TO GROUP 3. ELSE ADD TO GROUP 4.
	IN EACH SUBGROUP: SORT DESCENDING, CHOOSE FIRST HALF, AND ADD TO popToCross.
	*/
	int chromosoneIndex = 0, geneIndex = 0, divisionIndex = 0;

	char TempPoolOne[POPULATION_SIZE / 2][CHROMOSONE_LENGTH];
	char TempPoolTwo[POPULATION_SIZE / 2][CHROMOSONE_LENGTH];
	double dbTempPoolOne[POPULATION_SIZE / 2];
	double dbTempPoolTwo[POPULATION_SIZE / 2];

	char poolOne[POPULATION_SIZE / 4][CHROMOSONE_LENGTH];
	char poolTwo[POPULATION_SIZE / 4][CHROMOSONE_LENGTH];
	char poolThree[POPULATION_SIZE / 4][CHROMOSONE_LENGTH];
	char poolFour[POPULATION_SIZE / 4][CHROMOSONE_LENGTH];

	double poolOneFitness[POPULATION_SIZE / 4];
	double poolTwoFitness[POPULATION_SIZE / 4];
	double poolThreeFitness[POPULATION_SIZE / 4];
	double poolFourFitness[POPULATION_SIZE / 4];

	int i, j, k;
	double temp = 0;
	int shellLimit = POPULATION_SIZE / 4;
	char tempChrom[1][CHROMOSONE_LENGTH];

	//split array into two temp arrays
	for (i = 1, j = 0; i < POPULATION_SIZE / 2; i++, j++) {
		for (geneIndex = 0; geneIndex < CHROMOSONE_LENGTH; geneIndex++) {
			TempPoolOne[j][geneIndex] = population[i][geneIndex];
			dbTempPoolOne[j] = fitness[i];
		}
	}
	for (i = POPULATION_SIZE / 2, k = 0; i < POPULATION_SIZE; i++, k++) {
		for (geneIndex = 0; geneIndex < CHROMOSONE_LENGTH; geneIndex++) {
			TempPoolTwo[k][geneIndex] = population[i][geneIndex];
			dbTempPoolTwo[k] = fitness[i];
		}
	  }
	//End split

	//split tempArray one into two pools (1 and 2)
	for (i = 1, j = 0; i < (POPULATION_SIZE / 4) + 1; i++, j++) {
		for (geneIndex = 0; geneIndex < CHROMOSONE_LENGTH; geneIndex++) {
			poolOne[j][geneIndex] = TempPoolOne[i][geneIndex];
			poolOneFitness[j] = dbTempPoolOne[i];
		}
	}
	for (i = POPULATION_SIZE / 4, k = 0; i < (POPULATION_SIZE / 2); i++, k++) {
		for (geneIndex = 0; geneIndex < CHROMOSONE_LENGTH; geneIndex++) {
			poolTwo[k][geneIndex] = TempPoolOne[i][geneIndex];
			poolTwoFitness[k] = dbTempPoolOne[i];
		}
	}
	//end split

	//split tempArray two into two pools (3 and 4)
	for (i = 1, j = 0; i < (POPULATION_SIZE / 4) + 1; i++, j++) {
		for (geneIndex = 0; geneIndex < CHROMOSONE_LENGTH; geneIndex++) {
			poolThree[j][geneIndex] = TempPoolTwo[i][geneIndex];
			poolThreeFitness[j] = dbTempPoolTwo[i];
		}
	}
	for (i = POPULATION_SIZE / 4, k = 0; i < (POPULATION_SIZE / 2); i++, k++) {
		for (geneIndex = 0; geneIndex < CHROMOSONE_LENGTH; geneIndex++) {
			poolFour[k][geneIndex] = TempPoolTwo[i][geneIndex];
			poolFourFitness[k] = dbTempPoolTwo[i];
		}
	}
	//end split

	//Sort all pools into descending order. 
	//pool one
	for (i = 0; i < POPULATION_SIZE / 4; i++) {
		for (j = 0; j < (POPULATION_SIZE / 4) - 1; j++) {
			
			if (poolOneFitness[j] < poolOneFitness[j + 1]) {

				temp = poolOneFitness[j];
				for (geneIndex = 0; geneIndex < CHROMOSONE_LENGTH; geneIndex++) {
					tempChrom[0][geneIndex] = poolOne[j][geneIndex];
				}

				poolOneFitness[j] = poolOneFitness[j + 1];
				for (geneIndex = 0; geneIndex < CHROMOSONE_LENGTH; geneIndex++) {
					poolOne[j][geneIndex] = poolOne[j + 1][geneIndex];
				}
				poolOneFitness[j + 1] = temp;
				for (geneIndex = 0; geneIndex < CHROMOSONE_LENGTH; geneIndex++) {
					poolOne[j + 1][geneIndex] = tempChrom[0][geneIndex];
				}
			}
		}
	}
	//pool two
	for (i = 0; i < POPULATION_SIZE / 4; i++) {
		for (j = 0; j < (POPULATION_SIZE / 4) - 1; j++) {

			if (poolTwoFitness[j] < poolTwoFitness[j + 1]) {

				temp = poolTwoFitness[j];
				for (geneIndex = 0; geneIndex < CHROMOSONE_LENGTH; geneIndex++) {
					tempChrom[0][geneIndex] = poolTwo[j][geneIndex];
				}

				poolTwoFitness[j] = poolTwoFitness[j + 1];
				for (geneIndex = 0; geneIndex < CHROMOSONE_LENGTH; geneIndex++) {
					poolTwo[j][geneIndex] = poolTwo[j + 1][geneIndex];
				}
				poolTwoFitness[j + 1] = temp;
				for (geneIndex = 0; geneIndex < CHROMOSONE_LENGTH; geneIndex++) {
					poolTwo[j + 1][geneIndex] = tempChrom[0][geneIndex];
				}
			}
		}
	}
	//pool three
	for (i = 0; i < POPULATION_SIZE / 4; i++) {
		for (j = 0; j < (POPULATION_SIZE / 4) - 1; j++) {

			if (poolThreeFitness[j] < poolThreeFitness[j + 1]) {

				temp = poolThreeFitness[j];
				for (geneIndex = 0; geneIndex < CHROMOSONE_LENGTH; geneIndex++) {
					tempChrom[0][geneIndex] = poolThree[j][geneIndex];
				}

				poolThreeFitness[j] = poolThreeFitness[j + 1];
				for (geneIndex = 0; geneIndex < CHROMOSONE_LENGTH; geneIndex++) {
					poolThree[j][geneIndex] = poolThree[j + 1][geneIndex];
				}
				poolThreeFitness[j + 1] = temp;
				for (geneIndex = 0; geneIndex < CHROMOSONE_LENGTH; geneIndex++) {
					poolThree[j + 1][geneIndex] = tempChrom[0][geneIndex];
				}
			}
		}
	}
	//pool four
	for (i = 0; i < POPULATION_SIZE / 4; i++) {
		for (j = 0; j < (POPULATION_SIZE / 4) - 1; j++) {

			if (poolFourFitness[j] < poolFourFitness[j + 1]) {

				temp = poolFourFitness[j];
				for (geneIndex = 0; geneIndex < CHROMOSONE_LENGTH; geneIndex++) {
					tempChrom[0][geneIndex] = poolFour[j][geneIndex];
				}

				poolFourFitness[j] = poolFourFitness[j + 1];
				for (geneIndex = 0; geneIndex < CHROMOSONE_LENGTH; geneIndex++) {
					poolFour[j][geneIndex] = poolFour[j + 1][geneIndex];
				}
				poolFourFitness[j + 1] = temp;
				for (geneIndex = 0; geneIndex < CHROMOSONE_LENGTH; geneIndex++) {
					poolFour[j + 1][geneIndex] = tempChrom[0][geneIndex];
				}
			}
		}
	}

	//Add half of each pool to popToCross
	for (chromosoneIndex = 0; chromosoneIndex < POPULATION_SIZE / 2; chromosoneIndex++) {
		for (geneIndex = 0; geneIndex < CHROMOSONE_LENGTH; geneIndex++) {
			if (chromosoneIndex < POPULATION_SIZE / 8) {
				populationToBeCrossed[chromosoneIndex][geneIndex] = poolOne[chromosoneIndex][geneIndex];
			}
			else if (chromosoneIndex < POPULATION_SIZE / 4) {
				populationToBeCrossed[chromosoneIndex][geneIndex] = poolTwo[chromosoneIndex - POPULATION_SIZE / 8][geneIndex];
			}
			else if (chromosoneIndex < (POPULATION_SIZE / 4) + (POPULATION_SIZE / 8) ) {
				populationToBeCrossed[chromosoneIndex][geneIndex] = poolThree[chromosoneIndex - POPULATION_SIZE / 4][geneIndex];
			}
			else if (chromosoneIndex < (POPULATION_SIZE / 2)) {
				populationToBeCrossed[chromosoneIndex][geneIndex] = poolFour[chromosoneIndex - (( POPULATION_SIZE / 4) + (POPULATION_SIZE / 8))][geneIndex];
			}

		}
	}

}

void singlePointCrossover(){
	/*
	Select two parents at random.
	Cross over at random point between 1 and Chromosone Length, to produce two children
	repeat until population is full. My implementation actually only keeps one child per generation.
	*/
	int geneIndex = 0, chromosoneIndex = 0, k = 0;
	
	char childOne[1][CHROMOSONE_LENGTH];
	char childTwo[1][CHROMOSONE_LENGTH];
	char parentOne[1][CHROMOSONE_LENGTH];
	char parentDos[1][CHROMOSONE_LENGTH];
	


   //main loop to fill popPostCoitus
	while (k != POPULATION_SIZE ) {
		
		int parentSelectionOne = 1 + rand() % 1000;
		int parentSelectionTwo = 1 + rand() % 1000;
		int crossoverSelection = 1 + rand() % CHROMOSONE_LENGTH;

		//select Parents (one is a Spanish speaker)
		for (geneIndex = 0; geneIndex < CHROMOSONE_LENGTH; geneIndex++) {
			parentOne[0][geneIndex] = populationToBeCrossed[parentSelectionOne][geneIndex];
			parentDos[0][geneIndex] = populationToBeCrossed[parentSelectionTwo][geneIndex];
		}

		//crossover (sex)
		for (geneIndex = 0; geneIndex < CHROMOSONE_LENGTH; geneIndex++) {
			if (geneIndex < crossoverSelection) {
				childOne[0][geneIndex] = parentOne[0][geneIndex];
				childTwo[0][geneIndex] = parentDos[0][geneIndex];
			}
			else {
				childOne[0][geneIndex] = parentDos[0][geneIndex];
				childTwo[0][geneIndex] = parentOne[0][geneIndex];
			}
		}

		//fill popPostCoitus
		for (geneIndex = 0; geneIndex < CHROMOSONE_LENGTH; geneIndex++) {
			if (k % 2 == 0) {
				populationPostCoitus[k][geneIndex] = childOne[0][geneIndex];
			}
			else {
				populationPostCoitus[k][geneIndex] = childTwo[0][geneIndex];
			}
		}
    k++;
	}
}

void mutation(){
	//May not be right, but single gene has allele changed
	int rateOfMutaion = (POPULATION_SIZE / 100) * MUTATION_CHANCE_PERCENTAGE;
	int realRate = POPULATION_SIZE / rateOfMutaion;
	int mutationIndex = 0, count = 0, geneIndex = 0, chromosoneIndex = 0;
	
	for (mutationIndex = 1; mutationIndex <= POPULATION_SIZE; mutationIndex++) {
		int randomGene = 1 + rand() % CHROMOSONE_LENGTH;
		char randomAllel = 'a' + (rand() % 26);
		
		count++;
		if (count == realRate) {
			populationPostCoitus[mutationIndex][randomGene] = populationPostCoitus[mutationIndex][randomAllel];

			count = 0;
		}

	}
}
