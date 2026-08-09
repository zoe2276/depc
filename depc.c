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
    char** deps = malloc(depCount + 1);

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
                char* cursor = strstr(dep, "\"");
                if (cursor != NULL) {
                    cursor++;
                    while (*cursor != '"' && *cursor != '\0') {
                        depName[strlen(depName)] = *cursor++;
                        // printf("depName: %s\n", depName);
                    }
                    deps[i] = malloc(strlen(depName) + 1);
                    strcpy(deps[i], depName);
                    printf("deps[%d]: \t%s\n", i, deps[i]);
                    // depName should be clean up in next iter by calloc
                    i++;
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
