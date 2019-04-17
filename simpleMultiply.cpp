//
// Created by huangjie on 19-4-15.
//
#include <CL/cl.hpp>
#include <assert.h>

const char *source;

int main(int argc, const char *argv[]) {
    cl_int ciErrNum;
    int i, j;
    int ret;

    FILE *program_handle;
    size_t program_size[1];
    char *program_buffer;

    int wa = atoi(argv[1]);
    int ha = atoi(argv[2]);
    int wb = atoi(argv[3]);
    int hb = atoi(argv[4]);
    size_t wc = wb;
    size_t hc = ha;

    auto A = (int *) malloc(wa * ha * sizeof(int));
    auto B = (int *) malloc(wb * hb * sizeof(int));
    auto C = (int *) malloc(wc * hc * sizeof(int));

    for (i = 0; i < wa * ha; i++) {
        A[i] = 1;
    }

    printf("\n--------------------\n");
    for (i = 0; i < ha; i++) {
        for (j = 0; j < wa; j++) {
            printf("%d ", A[i * wa + j]);
        }
        printf("\n");
    }
    for (i = 0; i < wb * hb; i++) {
        B[i] = 1;
    }
    printf("\n--------------------\n");
    for (i = 0; i < hb; i++) {
        for (j = 0; j < wb; j++) {
            printf("%d ", B[i * wb + j]);
        }
        printf("\n");
    }
    printf("\n--------------------\n");

    //use the first platform
    cl_platform_id platform;
    ciErrNum = clGetPlatformIDs(1, &platform, nullptr);

    //use the first device
    cl_device_id device;
    ciErrNum = clGetDeviceIDs(platform, CL_DEVICE_TYPE_ALL, 1, &device, nullptr);

    cl_context_properties cps[3] = {CL_CONTEXT_PLATFORM, (cl_context_properties) platform, 0};

    //create context
    cl_context context = clCreateContext(cps, 1, &device, nullptr, nullptr, &ciErrNum);

    //create command queue;
    cl_command_queue queue = clCreateCommandQueue(context, device, 0, &ciErrNum);

    /* 声明buffer，传输数据 */
    //allocate space for matrix a on the device
    cl_mem buffer_a = clCreateBuffer(context, CL_MEM_READ_ONLY, wa * ha * sizeof(int), nullptr, &ciErrNum);

    //copy matrix a to the device
    ciErrNum = clEnqueueWriteBuffer(queue, buffer_a, CL_TRUE, 0, wa * ha * sizeof(int), (void *) A, 0, nullptr, nullptr);

    //allocate space for matrix b on the device
    cl_mem buffer_b = clCreateBuffer(context, CL_MEM_READ_ONLY, wb * hb * sizeof(int), nullptr, &ciErrNum);

    //copy matrix b to the device
    ciErrNum = clEnqueueWriteBuffer(queue, buffer_b, CL_TRUE, 0, wb * hb * sizeof(int), (void *) B, 0, nullptr, nullptr);

    //allocate space for matrix c on the device
    cl_mem buffer_c = clCreateBuffer(context, CL_MEM_READ_WRITE, ha * wb * sizeof(int), nullptr, &ciErrNum);

    /* 运行时kernel编译 */
    program_handle = fopen("../simpleMultiply.cl", "r");

    ret = fseek(program_handle, 0, SEEK_END);
    if (ret == -1) {
        perror("fseek");
    }

    program_size[0] = static_cast<size_t>(ftell(program_handle));

    rewind(program_handle);

    program_buffer = (char *) malloc((size_t)program_size + 1);

    program_buffer[program_size[0]] = '\0';

    fread(program_buffer, sizeof(char), program_size[0], program_handle);

    printf(program_buffer);

    fclose(program_handle);

    cl_program program = clCreateProgramWithSource(context, 1, (const char **) &program_buffer, program_size, &ciErrNum);

    //compile the program. Passing nullptr for the 'device_list' argument targets all devices in the context
    ciErrNum = clBuildProgram(program, 1, &device, nullptr, nullptr, nullptr);

    //create the kernel
    cl_kernel kernel = clCreateKernel(program, "simpleMultiply", &ciErrNum);
    assert(kernel!= nullptr);

    clSetKernelArg(kernel, 0, sizeof(cl_int), &wa);
    clSetKernelArg(kernel, 1, sizeof(cl_int), &ha);
    clSetKernelArg(kernel, 2, sizeof(cl_int), &wb);
    clSetKernelArg(kernel, 3, sizeof(cl_int), &hb);
    clSetKernelArg(kernel, 4, sizeof(cl_mem), &buffer_a);
    clSetKernelArg(kernel, 5, sizeof(cl_mem), &buffer_b);
    clSetKernelArg(kernel, 6, sizeof(cl_mem), &buffer_c);

    //set local and global workgroup sizes
    //we assume the matrix dimensions are divisible by 16
    size_t localws[2] = {16, 16};
    size_t globalws[2] = {wc, hc};

    //execute the kernel
    ciErrNum = clEnqueueNDRangeKernel(queue, kernel, 2, nullptr, globalws, localws, 0, nullptr, nullptr);

    free(A);
    free(B);

    ciErrNum = clEnqueueReadBuffer(queue, buffer_c, CL_TRUE, 0, wc * hc * sizeof(int), (void *) C, 0, nullptr, nullptr);

    printf("\n--------------------\n");
    for (i = 0; i < hc; i++) {
        for (j = 0; j < wc; j++) {
            printf("%d ", C[i * wc + j]);
        }
        printf("\n");
    }

    //release C
    free(C);/**/
}