#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE* readPkg() {
    FILE* fptr;
    fptr = fopen("./package.json", "r");
    if (fptr == NULL) {
        printf("No package.json found in the current directory. Make sure you are in the project root and try again.");
    }
    return fptr;
}

int getDepCount() {
    FILE* fptr = readPkg();

    char sToCheck[256];
    int deps = 0;
    while (fgets(sToCheck, sizeof(sToCheck), fptr)) {
        if (strstr(sToCheck, "\"dependencies\":") != NULL) {
            // we have found the dependency line
            char dep[256];
            while (fgets(dep, sizeof(dep), fptr)) {
                if (strstr(dep, "}") != NULL) {
                    // end of deps block
                    break;
                }
                deps++;
            }
            break;
        }
    }
    fclose(fptr);
    return deps;
}

char** getDeps() {
    FILE* fptr = readPkg();

    int depCount = getDepCount();
    char** deps = malloc(depCount);

    char sToCheck[256];
    while (fgets(sToCheck, sizeof(sToCheck), fptr)) {
        if (strstr(sToCheck, "\"dependencies\":") != NULL) {
            // we have found the dependency line
            char dep[256];
            int i = 0;
            while (fgets(dep, sizeof(dep), fptr)) {
                if (strstr(dep, "}") != NULL) {
                    // end of deps block
                    break;
                }

                // parse dep name
                char* depName = calloc(256, sizeof(char));
                char* cursor = strstr(dep, "\"") + 1;
                if (*cursor != NULL) {
                    while (*cursor != '"' && *cursor != '\0') {
                        printf("cursor: %c\n", *cursor);
                        depName[strlen(depName)] = *cursor;
                        cursor++;
                    }
                    printf("deps[%d] ", i);
                    deps[i] = malloc(strlen(depName) + 1);
                    printf("strcpy ");
                    strcpy(deps[i], depName);
                    // depName should realloc and clean up
                    i++;
                    if (i > 5) {
                    //    break;
                    }
                }
            }
            printf("before outer break");
            break;
        }
    }
    fclose(fptr);
    return deps;
}

int main() {
    char** deps = getDeps();

    printf("dep count: %d", getDepCount());

    for (int i = 0; i < getDepCount(); i++) {
        printf("dep %d: %s\n", i + 1, deps[i]);
        free(deps[i]);
    }

    printf("Done.");

    free(deps);
    return 0;
}
