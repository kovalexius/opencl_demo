#include <iostream>
#include <CL/opencl.h>

#define MAX_SOURCE_SIZE (0x100000)

int main(int argc, char *argv[])
{
    cl_int err;
    
    cl_uint ret_num_platforms;
    cl_platform_id platform_id;
    cl_device_id device_id;
    cl_uint num_devices;
    cl_uint max_compute_units;
    cl_uint ret_num_devices;
    
    // получить доступные платформы 
    err = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
    if( CL_SUCCESS == err )
        std::cout << "platform_id=" << platform_id << " num_platforms=" << ret_num_platforms << std::endl;
    else if( CL_INVALID_VALUE == err )
    {
        std::cout << "Platforms not found" << std::endl;
    }
    else
    {
        std::cout << "Failed while clGetPlatformIDs() err = " << err << std::endl;
    }
    
    
    // получить доступные устройства 
    err = clGetDeviceIDs( platform_id, CL_DEVICE_TYPE_DEFAULT, 1, &device_id, &num_devices );
    if( CL_SUCCESS == err )
    {
        std::cout << "device_id = " << device_id << " num_devices = " << num_devices << std::endl;
    }
    else if( CL_INVALID_VALUE == err )
    {
        std::cout << "Devices not found" << std::endl;
    }
    else
    {
        std::cout << "Failed while clGetDeviceIDs() err = " << err << std::endl;
    }
    
    err = clGetDeviceInfo( device_id, CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(max_compute_units), &max_compute_units, NULL);
    if(err == CL_SUCCESS)
    {
        std::cout << "Num of units:" << max_compute_units << std::endl;
    }
    else if( CL_INVALID_VALUE == err )
    {
        std::cout << "Units not found" << std::endl;
    }
    else
    {
        std::cout << "Failed while clGetDeviceInfo() err = " << err << std::endl;
    }

    cl_uint max_dimension_size;
    err = clGetDeviceInfo(device_id, CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS, sizeof(max_dimension_size), &max_dimension_size, NULL);
    if (err == CL_SUCCESS)
    {
        std::cout << "Num of dimension:" << max_dimension_size << std::endl;
    }
    else
        std::cout << "Error occured while clGetDeviceInfo() err = " << err << std::endl;

    /* создать контекст */
    cl_context context;
    context = clCreateContext( NULL, 1, &device_id, NULL, NULL, &err );
    if (err != CL_SUCCESS)
        std::cout << "Error occured while clCreateContext() err = " << err << std::endl;
    
    /* создаем команду */
    cl_command_queue command_queue;
    command_queue = clCreateCommandQueue(context, device_id, 0, &err);
    if (err != CL_SUCCESS)
        std::cout << "Error occured while clCreateCommandQueue() err = " << err << std::endl;
    
    cl_program program = NULL;
    cl_kernel kernel = NULL;

    const char fileName[] = "./forTest.cl";
    size_t source_size;
    char *source_str;
    int i;

    try 
    {
        FILE *fp;
        fp = fopen(fileName, "r");
        if (!fp) 
        {
            fprintf(stderr, "Failed to load kernel.\n");
            exit(1);
        }
        source_str = (char *)::malloc(MAX_SOURCE_SIZE);
        source_size = fread(source_str, 1, MAX_SOURCE_SIZE, fp);
        fclose(fp);
    } 
    catch (int a) 
    {
        printf("Exception code = %f", a);
    }

    /* создать бинарник из кода программы */
    program = clCreateProgramWithSource(context, 1, (const char **)&source_str, (const size_t *)&source_size, &err);
    if (err != CL_SUCCESS)
        std::cout << "Error occured while clCreateProgramWithSource()" << std::endl;

    /* скомпилировать программу */
    err = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);
    if (err != CL_SUCCESS)
        std::cout << "Error occured while clCreateBuildProgram()" << std::endl;

    /* создать кернел */
    kernel = clCreateKernel(program, "test", &err);
    if (err != CL_SUCCESS)
        std::cout << "Error occured while clCreateKernel()" << std::endl;
    
    
    return 0;
}