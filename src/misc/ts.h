#ifndef TS_H
#define TS_H

static struct {
    void *memory;
    uint64_t size;
    volatile uint64_t used;
} g_temp_storage;

bool ts_init(uint64_t size)
{
    int page_size = getpagesize();

    uint64_t num_pages = size / page_size;
    uint64_t remainder = size % page_size;
    if (remainder) num_pages++;

    g_temp_storage.used = 0;
    g_temp_storage.size = num_pages * page_size;
    g_temp_storage.memory = mmap(0, g_temp_storage.size + page_size, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);

    bool result = g_temp_storage.memory != MAP_FAILED;
    if (result) mprotect(g_temp_storage.memory + g_temp_storage.size, page_size, PROT_NONE);

    return result;
}

static inline void ts_assert_within_bounds(uint64_t size)
{
    if (size > g_temp_storage.size) {
        fprintf(stderr, "fatal error: temporary_storage exceeded amount of allocated memory. requested %lld, but allocated size is %lld\n", size, g_temp_storage.size);
        exit(EXIT_FAILURE);
    }
}

static inline uint64_t ts_align(uint64_t used, uint64_t align)
{
    assert((align & (align-1)) == 0);

    uintptr_t ptr   = (uintptr_t) (g_temp_storage.memory + used);
    uintptr_t a_ptr = (uintptr_t) align;
    uintptr_t mod   = ptr & (a_ptr - 1);

    if (mod != 0) ptr += a_ptr - mod;

    return ptr - (uintptr_t) g_temp_storage.memory;
}

#define ts_alloc_list(elem_type, elem_count) \
    ts_alloc_aligned(__alignof__(elem_type), sizeof(elem_type) * elem_count)

static inline void *ts_alloc_aligned(uint64_t alignment, uint64_t size)
{
    for (;;) {
        uint64_t used = __atomic_load_n(&g_temp_storage.used, __ATOMIC_RELAXED);
        uint64_t aligned = ts_align(used, alignment);
        uint64_t new_used = aligned + size;

        if (__sync_bool_compare_and_swap(&g_temp_storage.used, used, new_used)) {
            ts_assert_within_bounds(new_used);
            return g_temp_storage.memory + aligned;
        }
    }
}

static inline void *ts_alloc_unaligned(uint64_t size)
{
    uint64_t used = __sync_fetch_and_add(&g_temp_storage.used, size);
    ts_assert_within_bounds(used+size);
    return g_temp_storage.memory + used;
}

static inline void *ts_expand(void *ptr, uint64_t old_size, uint64_t increment)
{
    if (ptr) {
        assert(ptr == g_temp_storage.memory + g_temp_storage.used - old_size);
        uint64_t used = __sync_fetch_and_add(&g_temp_storage.used, increment);
        ts_assert_within_bounds(used+increment);
    } else {
        ptr = ts_alloc_unaligned(increment);
    }

    return ptr;
}

static inline void *ts_resize(void *ptr, uint64_t old_size, uint64_t new_size)
{
    assert(ptr == g_temp_storage.memory + g_temp_storage.used - old_size);
    if (new_size > old_size) {
        __sync_fetch_and_add(&g_temp_storage.used, new_size - old_size);
    } else if (new_size < old_size) {
        __sync_fetch_and_sub(&g_temp_storage.used, old_size - new_size);
    }
    return ptr;
}

static inline void ts_reset(void)
{
    g_temp_storage.used = 0;
}

#endif
