#ifndef PROCESS_MANAGER_H
#define PROCESS_MANAGER_H

#define PROCESS_EVENT_HANDLER(name) OSStatus name(EventHandlerCallRef ref, EventRef event, void *context)
typedef PROCESS_EVENT_HANDLER(process_event_handler);

struct process
{
    ProcessSerialNumber psn;
    pid_t pid;
    char *name;
    void *ns_application;
    int policy;
    bool volatile terminated;
};

struct process_manager
{
    struct table process;
    EventTargetRef target;
    EventHandlerUPP handler;
    EventTypeSpec type[3];
    EventHandlerRef ref;
    pid_t front_pid;
    pid_t last_front_pid;
    EventTime switch_event_time;
    ProcessSerialNumber finder_psn;
};

void process_destroy(struct process *process);
struct process *process_manager_find_process(struct process_manager *pm, ProcessSerialNumber *psn);
bool process_manager_begin(struct process_manager *pm);

#endif
