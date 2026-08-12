#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct dependencies {
    char** data;
    size_t count;
} deps_t;

int count_dependencies(FILE* fptr) {
    char line_cursor[256];
    int deps = 0;
    bool in_dependency_block = false;
    while (fgets(line_cursor, sizeof(line_cursor), fptr)) {
        if (!in_dependency_block) {
            if (strstr(line_cursor, "\"dependencies\":") != NULL) {
                in_dependency_block = true;
            }
        } else {
            if (strstr(line_cursor, "}") != NULL) break;
            deps++;
        }
    }

    rewind(fptr);
    return deps;
}

deps_t get_deps(FILE* fptr) {
    int dep_count = count_dependencies(fptr);

    char** deps = malloc((sizeof(char*) * dep_count));

    char line_cursor[256];
    int dep_idx = 0;
    bool in_dependency_block = false;
    while (fgets(line_cursor, sizeof(line_cursor), fptr)) {
        if (!in_dependency_block) {
            if (strstr(line_cursor, "\"dependencies\":") != NULL) {
                in_dependency_block = true;
            }
        } else {
            if (strchr(line_cursor, '}') == NULL) {
                // parse dep name
                char* cursor = strchr(line_cursor, '"');
                if (cursor != NULL) {
                    char* end_idx = strchr(++cursor, '"');

                    deps[dep_idx] = malloc((end_idx - cursor) + 1);
                    memcpy(deps[dep_idx], cursor, (end_idx - cursor));
                    dep_idx++;
                }
            } else {
                break;
            }
        }
    }
    deps_t data = { deps, dep_count };
    return data;
}

char** scan_file(char* path, deps_t deps) {
    FILE* fptr = fopen(path, "r");

    char line_cursor[256];
    while (fgets(line_cursor, sizeof(line_cursor), fptr)) {
        if (strstr(line_cursor, "import") != NULL) {

        }
    }

    free(fptr);
}

int main() {
    FILE* fptr = fopen("./package.json", "r");
    if (fptr == NULL) {
        printf("No package.json found in the current directory. Make sure you are in the project root and try again.");
        return 1;
    }

    deps_t deps = get_deps(fptr);
    free(fptr);

    printf("deps: %i\n", deps.count);
    // free deps
    for (int i = 0; i < deps.count; i++) {
        printf("dep %d: %s\n", i + 1, deps.data[i]);
        free(deps.data[i]);
    }
    free(deps.data);
    printf("\e[0;34mDone.\e[0m");
    return 0;
}
