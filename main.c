#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

/*
    I wrote Java's LCG for this even though this would take ~5 lines of Java, but I'm a masochist ig
    This picks away at generateRecipe in MKUCraftingHandler.java (HBM's Nuclear Tech) and takes the world seed as an argument
    https://github.com/HbmMods/Hbm-s-Nuclear-Tech-GIT/blob/master/src/main/java/com/hbm/crafting/handlers/MKUCraftingHandler.java#L47
*/

const uint64_t jump = 11;
const uint64_t multiplier = 25214903917ULL;

uint64_t internal_seed = 0;

void setSeed(int64_t world_seed) {
    internal_seed = ((uint64_t)world_seed ^ multiplier) & 0xFFFFFFFFFFFFULL;
}

int32_t next(int bits) {
    internal_seed = (internal_seed * multiplier + jump) & 0xFFFFFFFFFFFFULL;
    return (int32_t)(internal_seed >> (48 - bits));
}

int32_t nextInt(int32_t bound) {
    if ((bound & -bound) == bound) {
        return (int32_t)((bound * (int64_t)next(31)) >> 31);
    }

    int32_t bits, val;
    do {
        bits = next(31);
        val = bits % bound;
    } while (bits - val + (bound - 1) < 0);
    
    return val;
}

void shuffle(char *array[], int length) {
    for (int i = length; i > 1; i--) {
        int j = nextInt(i); 
        
        char* temp = array[i - 1];
        array[i - 1] = array[j];
        array[j] = temp;
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("usage: %s <world seed>\n", argv[0]);
        return 1;
    }

    int64_t world_seed = strtoll(argv[1], NULL, 10);

    setSeed(world_seed);

    char *recipe[9] = {
        "Iodine",
        "Phosphorus",
        "Dust",
        "Mercury",
        "Morning Glory",
        "Metal Syringe",
        "Empty",
        "Empty",
        "Empty"
    };

    shuffle(recipe, 9);

    printf("--------------------------------------------------------\n");

    for (int i = 0; i < 9; i += 3) {
        printf("%-15s | %-15s | %-15s\n", recipe[i], recipe[i+1], recipe[i+2]);
    }

    printf("--------------------------------------------------------\n");

    return 0;
}
