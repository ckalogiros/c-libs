# https://www.kernel.org/doc/gorman/html/understand/understand011.html

# 8.1.1��Cache Descriptor

All information describing a cache is stored in a struct kmem_cache_s declared in mm/slab.c. 
This is an extremely large struct and so will be described in parts.

<code>

     struct kmem_cache_s {
         struct list_head        slabs_full;
         struct list_head        slabs_partial;
         struct list_head        slabs_free;
         unsigned int            objsize;
         unsigned int            flags;
         unsigned int            num;
         spinlock_t              spinlock;
     #ifdef CONFIG_SMP
         unsigned int            batchcount;
     #endif

</code>
Most of these fields are of interest when allocating or freeing objects.
slabs_*     These are the three lists where the slabs are stored as described in the previous section;
objsize     This is the size of each object packed into the slab;
flags       These flags determine how parts of the allocator will behave when dealing with the cache. See Section 8.1.2;
num         This is the number of objects contained in each slab;
spinlock    A spinlock protecting the structure from concurrent accessses;
batchcount  This is the number of objects that will be allocated in batch for the per-cpu caches 
            as described in the previous section.




     unsigned int            gfporder;
     unsigned int            gfpflags;
 
     size_t                  colour;
     unsigned int            colour_off;
     unsigned int            colour_next;
     kmem_cache_t            *slabp_cache;
     unsigned int            growing;
     unsigned int            dflags;
 
     void (*ctor)(void *, kmem_cache_t *, unsigned long);
     void (*dtor)(void *, kmem_cache_t *, unsigned long);
 
     unsigned long           failures;
 

This block deals with fields of interest when allocating or freeing slabs from the cache.

gfporder    This indicates the size of the slab in pages. Each slab consumes 2gfporder pages 
            as these are the allocation sizes the buddy allocator provides;
gfpflags    The GFP flags used when calling the buddy allocator to allocate pages are stored here. 
            See Section 6.4 for a full list; 
colour      Each slab stores objects in different cache lines if possible. Cache colouring will be further discussed in Section 8.1.5;
colour_off  This is the byte alignment to keep slabs at. For example, slabs for the size-X caches are aligned on the L1 cache;
colour_next This is the next colour line to use. This value wraps back to 0 when it reaches colour;
growing     This flag is set to indicate if the cache is growing or not. If it is, it is much less likely 
            this cache will be selected to reap free slabs under memory pressure;
dflags      These are the dynamic flags which change during the cache lifetime. See Section 8.1.3; 
ctor        A complex object has the option of providing a constructor function to be called to initialise each new object. 
            This is a pointer to that function and may be NULL;
dtor        This is the complementing object destructor and may be NULL;
failures    This field is not used anywhere in the code other than being initialised to 0.

     char                    name[CACHE_NAMELEN];
     struct list_head        next;

These are set during cache creation

name This is the human readable name of the cache; 
next This is the next cache on the cache chain. 

     #ifdef CONFIG_SMP
         cpucache_t              *cpudata[NR_CPUS];
     #endif

cpudata This is the per-cpu data and is discussed further in Section 8.5. 

     #if STATS
         unsigned long           num_active;
         unsigned long           num_allocations;
         unsigned long           high_mark;
         unsigned long           grown;
         unsigned long           reaped;
         unsigned long           errors;
     #ifdef CONFIG_SMP
         atomic_t                allochit;
         atomic_t                allocmiss;
         atomic_t                freehit;
         atomic_t                freemiss;
     #endif
     #endif
     };

These figures are only available if the CONFIG_SLAB_DEBUG option is set during compile time. 
They are all beancounters and not of general interest. 
The statistics for /proc/slabinfo are calculated when the proc entry is read by another process 
by examining every slab used by each cache rather than relying on these fields to be available.

num_active      The current number of active objects in the cache is stored here;
num_allocations A running total of the number of objects that have been allocated on this cache is stored in this field;
high_mark       This is the highest value num_active has had to date;
grown           This is the number of times kmem_cache_grow() has been called;
reaped          The number of times this cache has been reaped is kept here;
errors          This field is never used;
allochit        This is the total number of times an allocation has used the per-cpu cache;
allocmiss       To complement allochit, this is the number of times an allocation has missed the per-cpu cache;
freehit         This is the number of times a free was placed on a per-cpu cache;
freemiss        This is the number of times an object was freed and placed on the global pool.



########################################################################################################################
# 8.2��Slabs

This section will describe how a slab is structured and managed. The struct which describes it is much simpler 
than the cache descriptor, but how the slab is arranged is considerably more complex. It is declared as follows:

    typedef struct slab_s {
        struct list_head        list;
        unsigned long           colouroff;
        void                    *s_mem;
        unsigned int            inuse;
        kmem_bufctl_t           free;
    } slab_t;

The fields in this simple struct are as follows:
list        This is the linked list the slab belongs to. This will be one of slab_full, slab_partial or slab_free from the cache manager;
colouroff   This is the colour offset from the base address of the first object within the slab. 
            The address of the first object is s_mem + colouroff;
s_mem       This gives the starting address of the first object within the slab;
inuse       This gives the number of active objects in the slab;
free        This is an array of bufctls used for storing locations of free objects. See Section 8.2.3 for further details.

8.6��Slab Allocator Initialisation

Here we will describe how the slab allocator initialises itself. When the slab allocator creates a new cache, 
it allocates the kmem_cache_t from the cache_cache or kmem_cache cache. 
This is an obvious chicken and egg problem so the cache_cache has to be statically initialised as

    static kmem_cache_t cache_cache = {
        slabs_full:     LIST_HEAD_INIT(cache_cache.slabs_full),
        slabs_partial:  LIST_HEAD_INIT(cache_cache.slabs_partial),
        slabs_free:     LIST_HEAD_INIT(cache_cache.slabs_free),
        objsize:        sizeof(kmem_cache_t),
        flags:          SLAB_NO_REAP,
        spinlock:       SPIN_LOCK_UNLOCKED,
        colour_off:     L1_CACHE_BYTES,
        name:           "kmem_cache",
    };

This code statically initialised the kmem_cache_t struct as follows:

    Initialise the three lists as empty lists;
    The size of each object is the size of a cache descriptor;
    The creation and deleting of caches is extremely rare so do not consider it for reaping ever;
    Initialise the spinlock unlocked;
    Align the objects to the L1 cache;
    Record the human readable name.

That statically defines all the fields that can be calculated at compile time. 
To initialise the rest of the struct, kmem_cache_init() is called from start_kernel().