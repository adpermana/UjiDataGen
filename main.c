#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "aes/aes.h"

int main() {
    int length = 16, n, i, j, k;
    unsigned char key[length];
    unsigned char P[length];
    unsigned char C[length];
    unsigned s;
    time_t t;
    FILE *fout1, *fout2, *fout3, *fout4, *fin;

    fout1 = fopen("seed.txt", "w");
    s = (unsigned) time(&t);
    srand(s);
    fprintf(fout1, "%x\n", s);
    fclose(fout1);

    //printf("Sample length (bytes): ");
    //scanf("%d", &length);

    printf("Number of samples: ");
    scanf("%d", &n);

    fout1 = fopen("key.txt", "w");
    for(i=0; i<length; i++) {
        key[i] = rand()%256;
        fprintf(fout1, "%.2x", key[i]);
    }
    fprintf(fout1, "\n");
    fclose(fout1);

    fout1 = fopen("plaintext1.txt", "w");
    fout4 = fopen("plaintext2.txt", "w");
    fout2 = fopen("ciphertext1.txt", "w");
    fout3 = fopen("ciphertext2.txt", "w");

    for(i=0; i<n; i++) {
        // Generate plaintext
        for(j=0; j<length; j++) {
            P[j] = rand()%256;
            fprintf(fout1, "%.2x", P[j]);
        }
        fprintf(fout1, "\n");

        // Generate ciphertext
        AES128_ECB_encrypt(P, key, C);
        for(j=0; j<length; j++) {
            fprintf(fout2, "%.2x", C[j]);
        }
        fprintf(fout2, "\n");

        //Generate flipped bit in plaintext and its ciphertext
        for(j=0; j<length*8; j++) {
            unsigned char mask = 128 >> (j%8);
            P[j/8] ^= mask;
            for(k=0; k<length; k++) {
                fprintf(fout4, "%.2x", P[k]);
            }
            fprintf(fout4, "\n");
            //fflush(fout4);

            AES128_ECB_encrypt(P, key, C);
            for(k=0; k<length; k++) {
                fprintf(fout3, "%.2x", C[k]);
            }
            fprintf(fout3, "\n");
            P[j/8] ^= mask;
        }
    }

    fclose(fout1);
    fclose(fout2);
    fclose(fout3);
    fclose(fout4);

    // Melakukan pembacaan file dg data Hexadecimal, dan menampilkan ke Terminal
    fin = fopen("plaintext1.txt", "r");
    for(i=0;i<n;i++) {
        for(j=0;j<length;j++) {
            unsigned char x, y;
            fscanf(fin, " %c %c", &x, &y);

//            if (x >= '0' && x <= '9')
//                x -= 48;
//            else if (x >= 'a' && x <= 'f')
//                x -= 87;

//            if (y >= '0' && y <= '9')
//                y -= 48;
//            else if (y >= 'a' && y <= 'f')
//                y -= 87;

            P[j] = (x << 4) + y;
            printf("%.2x", P[j]);
        }
        printf("\n");
    }
    fclose(fin);

    return 0;
}
