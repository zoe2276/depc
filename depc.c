#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct depsSt {
    char** data;
    size_t count;
};

FILE* readPkg() {
    FILE* fptr = fopen("./package.json", "r");
    if (fptr == NULL) {
        printf("No package.json found in the current directory. Make sure you are in the project root and try again.");
    }
    return fptr;
}

int getDepCount() {
    FILE* fptr = readPkg();

    char lineCursor[256];
    int deps = 0;
    int depth = 0; // track how many levels of nesting we are in relative to deps line
    while (fgets(lineCursor, sizeof(lineCursor), fptr)) {
        if (depth == 0) {
            if (strstr(lineCursor, "\"dependencies\":") != NULL) {
                // we have found the dependency line
                depth++;
            }
        } else {
            if (strstr(lineCursor, "{") != NULL) depth++; // start of new nesting level
            if (strstr(lineCursor, "}") != NULL) depth--; // end of current nesting level
            if (depth > 0) deps++;
        }
    }

    fclose(fptr);
    return deps;
}

struct depsSt getDeps() {
    int depCount = getDepCount();
    FILE* fptr = readPkg();

    char** deps = malloc(depCount + 1);

    char lineCursor[256];
    int depIdx = 0;
    int depth = 0; // track how many levels of nesting we are in relative to deps line
    while (fgets(lineCursor, sizeof(lineCursor), fptr)) {
        if (depth == 0) {
            if (strstr(lineCursor, "\"dependencies\":") != NULL) {
                // we have found the dependency line
                depth++;
            }
        } else {
            if (strstr(lineCursor, "{") != NULL) depth++; // start of new nesting level
            if (strstr(lineCursor, "}") != NULL) depth--; // end of current nesting level
            if (depth > 0) {
                // parse dep name
                char* depName = calloc(256, sizeof(char));
                char* cursor = strstr(lineCursor, "\"");
                if (cursor != NULL) {
                    cursor++;
                    int dci = 0;
                    while (*cursor != '"' && *cursor != '\0') {
                        depName[dci++] = *cursor++;
                    }
                    deps[depIdx] = malloc(strlen(depName) + 1);
                    strcpy(deps[depIdx], depName);
                    depIdx++;
                }
                free(depName);
            } 
        }
    }
    // fclose(fptr);
    struct depsSt data = { deps, depCount };
    return data;
}

char** scanFile(char* path, char** deps) {
    FILE* fptr = fopen(path, "r");

    char lineCursor[256];
    while (fgets(lineCursor, sizeof(lineCursor), fptr)) {
        if (strstr(lineCursor, "import") != NULL) {

        }
    }

    free(fptr);
}

int main() {
    struct depsSt deps = getDeps();

    // free deps
    for (int i = 0; i < deps.count; i++) {
        printf("dep %d: %s\n", i + 1, deps.data[i]);
        free(deps.data[i]);
    }
    free(deps.data);
    
    printf("Done.");
    return 0;
}
