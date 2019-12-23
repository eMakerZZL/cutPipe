#include "Parse.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define UNUSE(arg) (arg = arg)

#define PROTOCOL_FUNC(name_) \
    static bool protocol_##name_(void* parg)

#define PROTOCOL_ITEM(name_, id_)     \
    {                                 \
        .name = #name_,               \
        .id = id_,                    \
        .protocol = protocol_##name_, \
    }

PROTOCOL_FUNC(system);

struct _Protocol_fmt_t {
    uint8_t* name;
    uint8_t id;
    bool (*protocol)(void* parg);
};

static const struct _Protocol_fmt_t Protocol_fmt[] = {
    PROTOCOL_ITEM(system, 1),
};

#define MAX_CMD_HEADER_LENGTH 20
#define MAX_CMD_CONTEXT_LENGTH 30

static char cmd_context[MAX_CMD_CONTEXT_LENGTH] = { 0 };

PROTOCOL_FUNC(system)
{
    UNUSE(parg);
}

bool Parse_Cmd(const char* cmd)
{
    uint32_t i;
    uint32_t itmes = sizeof(Protocol_fmt) / sizeof(Protocol_fmt[0]);

    static char cmd_header[MAX_CMD_HEADER_LENGTH] = { 0 };
    char* pstr = NULL;

    if (cmd != NULL && strlen(cmd) > 0) {
        if ((pstr = strchr(':', cmd)) != NULL) {
            uint32_t pdiff = pstr - cmd;
            strcmp(cmd_header, cmd, pdiff);
        }
    }

    for (i = 0; i < itmes; i++) {
        if (strcmp(Protocol_fmt[i].name, cmd) == 0)
            return true;
    }

    return false;
}
