//
// Created by huangjie on 19-3-31.
//
#include <CL/cl.h>
#include <stdio.h>
#include <assert.h>

int main(int argc, const char *argv[])
{
    cl_int err;
    cl_platform_id clPlatformId[1] = {NULL};
    cl_device_id clDeviceId[1] = {NULL};

    clGetPlatformIDs(1, clPlatformId, NULL);
    assert(&clPlatformId[0]!=NULL);
    clGetDeviceIDs(clPlatformId[0], CL_DEVICE_TYPE_DEFAULT, 1, clDeviceId, NULL);
    assert(&clDeviceId[0]!=NULL);
    cl_context context = clCreateContext(NULL, 1, &clDeviceId[0], NULL, NULL, &err);
    if (err < 0) {
        perror("Couldn't create a context\n");
        exit(EXIT_FAILURE);
    }

    float main_data[100];
    cl_mem main_buffer, sub_buffer;
    void *main_buffer_mem = NULL, *sub_buffer_mem = NULL;
    size_t main_buffer_size, sub_buffer_size;
    cl_buffer_region region;

    main_buffer = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
                                 sizeof(main_data), main_data, &err);
    if (err < 0) {
        perror("Couldn't create a buffer\n");
        exit(EXIT_FAILURE);
    }

    region.origin = 30 * sizeof(float);
    region.size = 20 * sizeof(float);

//    sub_buffer = clCreateSubBuffer(main_buffer, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
//                                   CL_BUFFER_CREATE_TYPE_REGION, &region, &err);
//    if (err < 0) {
//        perror("Couldn't create a sub buffer\n");
//        exit(EXIT_FAILURE);
//    }

    clGetMemObjectInfo(main_buffer, CL_MEM_SIZE, sizeof(main_buffer_size), &main_buffer_size, NULL);
//    clGetMemObjectInfo(sub_buffer, CL_MEM_SIZE, sizeof(sub_buffer_size), &sub_buffer_size, NULL);
    printf("Main buffer size: %lu\n", main_buffer_size);
//    printf("Sub buffer size: %lu\n", sub_buffer_size);

    clGetMemObjectInfo(main_buffer, CL_MEM_HOST_PTR, sizeof(main_buffer_mem), &main_buffer_mem, NULL);
//    clGetMemObjectInfo(sub_buffer, CL_MEM_HOST_PTR, sizeof(sub_buffer_mem), &sub_buffer_mem, NULL);
    printf("Main data memory address: %p\n", main_data);
    printf("Main buffer memory address: %p\n", main_buffer_mem);
//    printf("Sub buffer memory address: %p\n", sub_buffer_mem);

    clReleaseMemObject(main_buffer);
//    clReleaseMemObject(sub_buffer);
    clReleaseContext(context);
}