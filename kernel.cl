__kernel void vector_add(__global const int* A, __global const int* B, __global int* C) {
    
    // Get index
    int i = get_global_id(0);

    // Perform addition
    C[i] = A[i] + B[i];
}