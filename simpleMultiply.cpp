//
// Created by huangjie on 19-4-15.
//
#include <CL/cl.hpp>

const char *source;

int main(int argc, const char *argv[]) {
    cl_int ciErrNum;

    int wa = atoi(argv[1]);
    int ha = atoi(argv[2]);
    int wb = atoi(argv[3]);
    int hb = atoi(argv[4]);
    int wc = wb;
    int hc = ha;

    float *A = (float *) malloc(wa * ha * sizeof(float));
    float *B = (float *) malloc(wb * hb * sizeof(float));
    float *C = (float *) malloc(wc * hc * sizeof(float));

    //TODO: init A, B

    //use the first platform
    cl_platform_id platform;
    ciErrNum = clGetPlatformIDs(1, &platform, NULL);

    //use the first device
    cl_device_id device;
    ciErrNum = clGetDeviceIDs(platform, CL_DEVICE_TYPE_ALL, 1, &device, NULL);

    cl_context_properties cps[3] = {CL_CONTEXT_PLATFORM, (cl_context_properties) platform, 0};

    //create context
    cl_context context = clCreateContext(cps, 1, &device, NULL, NULL, &ciErrNum);

    //create command queue;
    cl_command_queue queue = clCreateCommandQueue(context, device, 0, &ciErrNum);

    /* 声明buffer，传输数据 */
    //allocate space for matrix a on the device
    cl_mem buffer_a = clCreateBuffer(context, CL_MEM_READ_ONLY, wa * ha * sizeof(float), NULL, &ciErrNum);

    //copy matrix a to the device
    ciErrNum = clEnqueueWriteBuffer(queue, buffer_a, CL_TRUE, 0, wa * ha * sizeof(float), (void *) A, 0, NULL, NULL);

    //allocate space for matrix b on the device
    cl_mem buffer_b = clCreateBuffer(context, CL_MEM_READ_ONLY, wb * hb * sizeof(float), NULL, &ciErrNum);

    //copy matrix b to the device
    ciErrNum = clEnqueueWriteBuffer(queue, buffer_b, CL_TRUE, 0, wb * hb * sizeof(float), (void *) B, 0, NULL, NULL);

    //allocate space for matrix c on the device
    cl_mem buffer_c = clCreateBuffer(context, CL_MEM_READ_ONLY, ha * wb * sizeof(float), NULL, &ciErrNum);

    /* 运行时kernel编译 */
    //TODO： read kernel file
    cl_program program = clCreateProgramWithSource(context, 1, (const char **) &source, NULL, &ciErrNum);

    //compile the program. Passing NULL for the 'device_list' argument targets all devices in the context
    ciErrNum = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);

    //create the kernel
    cl_kernel kernel = clCreateKernel(program, "simpleMutiply", &ciErrNum);

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
    ciErrNum = clEnqueueNDRangeKernel(queue, kernel, 2, NULL, globalws, localws, 0, NULL, NULL);

    free(A);
    free(B);

    ciErrNum = clEnqueueReadBuffer(queue, buffer_c, CL_TRUE, 0, wc * hc * sizeof(float), (void *) C, 0, NULL, NULL);

    //TODO: process C

    //release C
    free(C);/**/
}