//this is a partial representation to show how hoard memory allocator helps in the memory allocation in Haiku OS
#include <stdio.h>
#include <stdlib.h> 

// to show the memorychunk implementation
typedef struct MemoryChunk {
    void* base;             // Pointer to the allocated memory
    size_t size;            // Size of the allocated memory block
    struct MemoryChunk* next;  // Pointer to the next memory chunk in the linked list
} MemoryChunk;

// different pool sizes which will be later used in the size allocation in the linked list type implementation
#define SMALL_POOL 0
#define MEDIUM_POOL 1
#define LARGE_POOL 2
#define XLARGE_POOL 3
#define NUM_POOLS 4

// Simulated memory pools for different size categories
MemoryChunk* free_pools[NUM_POOLS] = {NULL}; // 4 pools for the different sizes which can be allocated

// Allocate memory from the pool (simulated using malloc)
void* hoard_malloc(size_t size) {
    const char* pool_names[NUM_POOLS] = {"SMALL_POOL", "MEDIUM_POOL", "LARGE_POOL", "XLARGE_POOL"};
    
    // Choose pool based on size
    int pool_index = 0; //this is the smallest pool
    
    if (size <= 16)//this is for the small pool size
    {
        pool_index = SMALL_POOL;
    } else if (size <= 64) //this is the range for medium limit size
    {
        pool_index = MEDIUM_POOL;
    } else if (size <= 256) //this is the range for large limit size
    {
        pool_index = LARGE_POOL;
    } else //the remaining greater sizes than 256
    {
        pool_index = XLARGE_POOL;
    }

    // outputting from where the size of the data is allocated 
    printf("Allocating memory of size %zu from pool %s\n", size, pool_names[pool_index]);

    // Search for a suitable chunk in the chosen pool
    MemoryChunk* current = free_pools[pool_index];
    while (current != NULL) {
        if (current->size >= size) {
            // Found a suitable chunk, remove from free pool
            free_pools[pool_index] = current->next;
            // Remove freed chunk from the free pool list,here current->next refers to the next one after being freed
            //removes the current chunk from the linked list by pointing the head to the next chunk
            return current->base;
        }
        current = current->next;
    }

    // No fitting chunk found, simulate requesting more memory from VM
    printf("Hoard requesting more memory from VM for size %zu\n", size);

    // To show the VM interaction (replace with actual VM calls)
    void* vm_allocated_memory = malloc(size); // Simulate VM allocating memory
    if (vm_allocated_memory == NULL) {
        printf("VM allocation failed!\n");
        return NULL; // Indicate allocation failure
    }
    else
    printf("VM allocation sucess\n");

    // Simulate VM returning virtual address
    void* virtual_address = vm_allocated_memory; // Replace with VM-provided virtual address

    // Add the newly allocated memory (from VM) to the free pool
    MemoryChunk* chunk = (MemoryChunk*)malloc(sizeof(MemoryChunk));
    if (chunk == NULL) {
        // Handle allocation failure (e.g., print error message)
        free(vm_allocated_memory); // Free VM-allocated memory if chunk allocation fails
        return NULL;
    }
    chunk->base = virtual_address;//assigning the virtual address to the base(The starting value) of the chunk.
    chunk->size = size;
    chunk->next = NULL; 
    free_pools[pool_index] = chunk;//returning the remaining chunk to the free pools list

    return virtual_address;
}

// Free memory back to the pool (simulated using free)
void hoard_free(void* ptr) {
    //find the size of the freed chunk 
    //used the free keyword in the above , symbolic representation of the above 
    //it is usually used for freeing the dynamically left out space that is not used
    //free( *ptr);
    //the above is used for showing that the data can be freed 
}

int main() {
    void* ptr1 = hoard_malloc(10);
    void* ptr2 = hoard_malloc(20);
    void* ptr3 = hoard_malloc(100);
    void* ptr4 = hoard_malloc(300);

    // Use the allocated memory (ptr1 and ptr2)

    hoard_free(ptr1);
    hoard_free(ptr2);
    hoard_free(ptr3);
    hoard_free(ptr4);

    return 0;
}
