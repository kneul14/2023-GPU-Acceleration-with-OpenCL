__kernel void vector_multiply(__global const int* D, __global const int* E, __global int* F) {
    // Get index
    int i = get_global_id(0);

    F[i] = D[i] * E[i];
}