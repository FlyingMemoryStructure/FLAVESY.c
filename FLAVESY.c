#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXy 15
#define MAXx 19

struct Particle {
    float fMomentum;
    int y, x;
    int bUp, bDown, bIncrement;
};

void Delay(int nSpeed, int nMode);
void refreshFrame();
void centerFrame();
void updateParticles(struct Particle *P, char arrayArea[MAXy][MAXx], int nAmplitude);

int main() {
    char arrayArea[MAXy][MAXx] = {0};
    int nSpeed = 0, nMode = 0, nAmplitude = 0, nHeight = 0, nInitialAmplitude = 0, nIntensity = 0, forever = 1;
    int x = 0, y = 0, i = 0, bStable = 0;
    float fInitialMomentum = 0, fTemp = 0;

    struct Particle P[MAXx] = {0};

    while (forever) {
        printf("Initial momentum (1 calmest): ");
        if (scanf("%f", &fInitialMomentum) == 1 && fInitialMomentum >= 1) break;
        while (getchar() != '\n'); // clear input buffer
    }
    
    while (forever) {
        printf("Initial Amplitude (4 minimum, 14 maximum): ");
        if (scanf("%d", &nAmplitude) == 1 && nAmplitude >= 4 && nAmplitude < MAXy) break;
        while (getchar() != '\n'); // clear input buffer
    }

    while (forever) {
        printf("Initial height ((nAmplitude / 2) - 1 is lowest, 0 is highest): ");
        if (scanf("%d", &nHeight) == 1 && nHeight >= 0 && nHeight < nAmplitude / 2) break;
        while (getchar() != '\n'); // clear input buffer
    }

    while (forever) {
        printf("Stable oscillation\n(1 = Yes | 2 = No): ");
        if (scanf("%d", &bStable) == 1 && (bStable == 1 || bStable == 2)) break;
        while (getchar() != '\n'); // clear input buffer
    }

    if (bStable == 2) {
        while (forever) {
            printf("Intensity (1 is stablest, 14 is most unstable): ");
            if (scanf("%d", &nIntensity) == 1 && nIntensity >= 1 && nIntensity < MAXy) break;
            while (getchar() != '\n'); // clear input buffer
        }
    }

    while (forever) {
        printf("Mode\n(1 = Manual | 2 = Automatic): ");
        if (scanf("%d", &nMode) == 1 && (nMode == 1 || nMode == 2)) break;
        while (getchar() != '\n'); // clear input buffer
    }

    if (nMode == 2) {
        while (forever) {
            printf("Speed: ");
            if (scanf("%d", &nSpeed) == 1 && nSpeed >= 1) break;
            while (getchar() != '\n'); // clear input buffer
        }
    } 
	else
        printf("\nPress enter repeatedly to continue the motion of the wave");

    // Initialize particles
    for (i = 0; i < MAXx; i++) {
        P[i].x = i;
        P[i].y = 0;
        P[i].bUp = 0;
        P[i].bDown = 1;
        P[i].bIncrement = 0;
        P[i].fMomentum = fInitialMomentum;
    }

	nInitialAmplitude = nAmplitude;

    while (forever) {
        i = 0;

		if (bStable == 2) {
        	if (!P[0].bUp) {
                if (nAmplitude != nInitialAmplitude)
                	nAmplitude--;
            } 
			else {
            	if (nAmplitude != nInitialAmplitude + nIntensity)
                	nAmplitude++;
            }
        }

        while (i < MAXx) {
            if (P[i].y >= nAmplitude && !(P[i].bIncrement)) {
                P[i].fMomentum = 0;
                P[i].bUp = 1;
                P[i].bDown = 0;
            } 
			else if ((P[i].y == nHeight) && !(P[i].bIncrement)) {
                P[i].fMomentum = 0;
                P[i].bUp = 0;
                P[i].bDown = 1;
            }
            if (P[i].fMomentum <= 0)
                P[i].bIncrement = 1;
            if (P[i].fMomentum >= fInitialMomentum)
                P[i].bIncrement = 0;
            if (P[i].bIncrement)
                P[i].fMomentum += 0.25;
            else if (P[i].bUp) {
                if (P[i].y != 0)
                    P[i].y--;
                P[i].fMomentum -= 0.25;
            } 
			else if (P[i].bDown) {
                P[i].y++;
                P[i].fMomentum -= 0.25;
            }
            if (i + 1 < MAXx)
                P[i + 1].fMomentum = P[i].fMomentum * 1.246;

            i++;
        }
        
        updateParticles(P, arrayArea, nAmplitude);

        Delay(nSpeed, nMode);
        refreshFrame();
        for (y = 0; y < MAXy; y++) {
            printf("||");
            for (x = 0; x < MAXx; x++)
                printf("%c", arrayArea[y][x]);
            printf("||\n");
        }
        printf("=======================\n\n");
        i = 0;
        fTemp = 0;
        while (i < MAXx) {
            printf("%f ", P[i].fMomentum);
            if (i == 4 || i == 9 || i == 14)
                printf("\n");
            fTemp += P[i].fMomentum;
            i++;
        }
        printf("\nMean momentum: %f", fTemp / MAXx);
        printf("\nAmplitude: %d", nAmplitude);
        if (nMode == 1)
            printf("\n\nPress enter repeatedly to continue the motion of the wave");
        centerFrame();
    }

    return 0;
}

void Delay(int nSpeed, int nMode) {
    if (nMode == 1)
        // Delay is dependent on how fast the user presses enter if mode 1:
        while (getchar() != '\n');
    else {
        // Delay is dependent on speed input if mode 2:
        int i = 0;
        for (i = 0; i < nSpeed * 2000000; i++);
    }
}

void refreshFrame() {
    // Clear screen
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void centerFrame() {
	int i = 0;
    for (i = 0; i < 5; i++)
        printf("\n");
}

void updateParticles(struct Particle P[], char arrayArea[MAXy][MAXx], int nAmplitude) {
    memset(arrayArea, ' ', sizeof(char) * MAXy * MAXx); // Clear the array
    int i = 0, j = 0, x = 0, y = 0;
    for (i = 0; i < MAXx; i++) {
        if (P[i].y < MAXy && P[i].x < MAXx) {
            // Set the particle character
            if (P[i].bDown)
                arrayArea[P[i].y][P[i].x] = 'v';
            else if (P[i].bUp)
                arrayArea[P[i].y][P[i].x] = '^';
            // Fill all rows below the particle with '#'
            for (j = P[i].y + 1; j < MAXy; j++)
                arrayArea[j][P[i].x] = '#';
        }
    }
}






