//widthA = heightB for valid matrix
__kernel void simpleMultiply(
    int widthA,
    int heightA,
    int widthB,
    int heightB,
    __global float *matrixA,
    __global float *matrixB,
    __global float *matrixC,
){
    //get global position in y direction
    int row = get_global_id(1);
    //get global position in x direction
    int col = get_global_id(0);

    float sum = 0.0f;

    //calvulate result of one element of matrix c
    for(int i = 0; i < widthA; i++)
    {
        sum + = matrixA[row*widthA+i]*matrixB[i*widthB+col];
    }
    matrixC[row*widthB+col] = sum;
}