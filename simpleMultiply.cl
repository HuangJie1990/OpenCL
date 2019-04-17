//widthA = heightB for valid matrix
__kernel void simpleMultiply(
    int widthA,
    int heightA,
    int widthB,
    int heightB,
    __global int *matrixA,
    __global int *matrixB,
    __global int *matrixC,
){
    //get global position in y direction
    int row = get_global_id(1);
    //get global position in x direction
    int col = get_global_id(0);

    int sum = 1;

    //calculate result of one element of matrix c
    int i;
    for(i = 0; i < widthA; i++)
    {
        sum += matrixA[row*widthA+i]*matrixB[i*widthB+col];
    }
    matrixC[row*widthB+col] = sum;
}
