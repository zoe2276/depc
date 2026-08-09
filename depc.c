#include <stdio.h>
#include <string.h>

int main() {
    FILE* fptr;

    fptr = fopen("./package.json", "r");
    if (fptr == NULL) {
        printf("No package.json found in the current directory. Make sure you are in the project root and try again.");
        return 1;
    }

    char sToCheck[256];

    while (fgets(sToCheck, sizeof(sToCheck), fptr)) {
        if (strstr(sToCheck, "\"dependencies\":") != NULL) {
            // we have found the dependency line
            printf("Found dependencies.\n");

            char dep[256];
            while (fgets(dep, sizeof(dep), fptr)) {
                if (strstr(dep, "}") != NULL) {
                    // end of deps block
                    break;
                }

                // parse dep name
                char depName[256];
                const char* cursor = strstr(dep, "\"") + 1;
                while (*cursor != "\"" && *cursor != '\0') {
                    depName[strlen(depName)] = *cursor;
                    cursor++;
                }
                printf("dependency name: %s\n", depName);
            }
            break;
        }
    }

    printf("Done.");
    fclose(fptr);

    return 0;
}
