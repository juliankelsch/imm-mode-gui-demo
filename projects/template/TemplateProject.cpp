#include "TemplateProject.h"
#include "platform/Platform.h"
#include "DebugLog.h"

struct name_state
{

};

static name_state* State = nullptr;

void Name::UpdateAndRender(input *Input) {
    if(!State)
    {
        State = (name_state*)PlatformAllocateMemoryZeroInitialized(sizeof(name_state));
        if(!State)
        {
            LogError("Could not allocate memory for noise project\n");
            return;
        }
    }

}

void Name::Shutdown()
{
    PlatformFreeMemory(State);
    State = nullptr;
}
