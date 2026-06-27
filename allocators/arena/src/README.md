# ARENA ALLOCTOR

Arena allocator is just a memory handler.
Gives back a pointer to a memory that te caller may use to store any data.
It does not have any control of that data, nor it stores data in that memory.

Arena allocator, it's the simplest allocator, meaning there is not much overhead, 
just a pointer advancing to the next available free memory address. 

Arena allocator 
* It does handle proper alignment, so it has generic approach to data storage.
* It does NOT "know" anything of it's previous "data stores", so practically it cannot read at any specific stored value, it rather reads the whole memory from it's starting address.
* It does check for memory overflow.
* It does reallocate on memory overflow, through it's "instances" implementation.
* It does have a table of "allocator instances", so that many instances of the allocator are possible.
* 