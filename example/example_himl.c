/* test for primitive operator */
/*
 * define tensors: input, weight, bias and output
 * op name : conv
 * input size:     ni x ci x hi x wi
 * weight size:    co x ci x kh x kw
 * bias size:      1 x co x 1 x 1
 * output size:    no x co x ho x wo
*/
#include <iostream>
#include "tests.h"

int conv_test() {
  unsigned int seed = time(0);
  // prepare data for conv
  const int ni = 1, ci = 64, hi = 32, wi = 32;
  const int no = 1, co = 64, ho = 32, wo = 32;
  const int kh = 3, kw = 3, stride_h = 1, stride_w = 1;
  const int dilation_h = 1, dilation_w = 1;
  const int pad_h = 2, pad_w = 2;

  float *input_cpu_data = (float*)malloc(ni * ci * hi * wi * sizeof(float));
  for (int index = 0; index < ni * ci * hi * wi; ++index) 
  {
      input_cpu_data[index] = ((rand_r(&seed) % 100 / 100.0) - 0.5) / 8;
  }

  float *filter_cpu_data = (float*)malloc(co * ci * kh * kw * sizeof(float));
  for (int index = 0; index < co * ci * kh * kw; ++index) 
  {
      filter_cpu_data[index] = ((rand_r(&seed) % 100 / 100.0) - 0.5) / 8;
  }
  float *output_cpu_data = (float*)malloc(no * co * ho * wo * sizeof(float));
  float *cpu_result = (float*)malloc(no * co * ho * wo * sizeof(float));
  for (int index = 0; index < no * co * ho * wo; ++index) 
  {
      output_cpu_data[index] = 0.0;
  }

  float *bias_cpu_data = (float*)malloc(co * sizeof(float));
  for (int index = 0; index < co; ++index) 
  {
      bias_cpu_data[index] = (rand_r(&seed) % 100 / 100.0) * 128;
  }

  // prepare input cpu tensor
  himlCpuTensor_t input_cpu;
  himlCreateCpuTensor(&input_cpu,
          himl_TENSOR,
          himl_DATA_FLOAT32,
          himl_NCHW,
          ni,
          ci,
          hi,
          wi);

  // prepare weight cpu tensor
  himlCpuTensor_t filter_cpu;
  himlCreateCpuTensor(&filter_cpu,
          himl_FILTER,
          himl_DATA_FLOAT32,
          himl_NCHW,
          co,
          ci,
          kh,
          kw);

  // prepare bias cpu tensor
  himlCpuTensor_t bias_cpu;
  himlCreateCpuTensor(&bias_cpu,
          himl_CONST,
          himl_DATA_FLOAT32,
          himl_NCHW,
          1,
          co,
          1,
          1);

  // prepare output cpu tensor
  himlCpuTensor_t output_cpu;
  himlCreateCpuTensor(&output_cpu,
          himl_TENSOR,
          himl_DATA_FLOAT32,
          himl_NCHW,
          no,
          co,
          ho,
          wo);

  // no data order needed in creating himl tensor
  // prepare input himl tensor
  himlTensor_t input_himl;
  himlCreateTensor(&input_himl,
          himl_TENSOR,
          himl_DATA_FLOAT16,
          ni,
          ci,
          hi,
          wi);

  // prepare filter himl tensor
  himlTensor_t filter_himl;
  himlCreateTensor(&filter_himl,
          himl_FILTER,
          himl_DATA_FLOAT16,
          co,
          ci,
          kh,
          kw);

  // prepare bias himl tensor
  himlTensor_t bias_himl;
  himlCreateTensor(&bias_himl,
          himl_CONST,
          himl_DATA_FLOAT16,
          1,
          co,
          1,
          1);

  // prepare output himl tensor
  himlTensor_t output_himl;
  himlCreateTensor(&output_himl,
          himl_TENSOR,
          himl_DATA_FLOAT16,
          no,
          co,
          ho,
          wo);

  // bind cpu filter, bias to himltensor
  himlBindConstData(filter_himl,
                    filter_cpu,
                    filter_cpu_data);
  himlBindConstData(bias_himl,
                    bias_cpu,
                    bias_cpu_data);
  //  prepare conv param
  himlConvOpParam_t conv_param;
  himlCreateConvOpParam(&conv_param,
          stride_h,
          stride_w,
          dilation_h,
          dilation_w,
          pad_h,
          pad_w);

  // setup conv operator
  himlBaseOp_t conv_op;
  himlCreateConvOp(&conv_op,
          conv_param,
          input_himl,
          output_himl,
          filter_himl,
          bias_himl);
  // setup and check op runnable
  himlCheckBaseOpRunnable(conv_op, himl_C10);

  // compile op
  himlCompileBaseOp(conv_op, himl_C10, 1);

  // malloc himl tensor
  void *input_himl_data = himlMallocBuffer(
          input_himl);
  void *output_himl_data = himlMallocBuffer(
          output_himl);

  // copy input to himl tensor
  himlMemcpyTensorToDevice(
          input_cpu,
          input_cpu_data,
          input_himl,
          input_himl_data);

  /* create stream and func_type and llc */
  cnrtFunctionType_t func_type = CNRT_FUNC_TYPE_BLOCK;
  cnrtStream_t stream;
  cnrtCreateStream(&stream);

  // compute conv op
  himlComputeConvOpForward(conv_op,
          input_himl_data,
          output_himl_data,
          func_type,
          stream
          );

  // copy output to cpu tensor
  cnrtSyncStream(stream);
  cnrtDestroyStream(stream);

  himlMemcpyTensorToHost(
          output_himl,
          output_himl_data,
          output_cpu,
          output_cpu_data);
  himlDumpTensor2File("mlu_output", output_cpu, himl_TENSOR, output_cpu_data, false);

  himlCpuComputeConvOpForward(conv_param,
          input_cpu,
          input_cpu_data,
          filter_cpu,
          filter_cpu_data,
          output_cpu,
          cpu_result,
          bias_cpu,
          bias_cpu_data);
  himlDumpTensor2File("cpu_output", output_cpu, himl_TENSOR, cpu_result, false);

  // delete conv param
  himlDestroyConvOpParam(&conv_param);

  // delete himl tensors
  himlDestroyTensor(&input_himl);
  himlDestroyTensor(&filter_himl);
  himlDestroyTensor(&bias_himl);
  himlDestroyTensor(&output_himl);

  // delete cpu tensors
  himlDestroyCpuTensor(&input_cpu);
  himlDestroyCpuTensor(&filter_cpu);
  himlDestroyCpuTensor(&bias_cpu);
  himlDestroyCpuTensor(&output_cpu);
  himlDestroyBaseOp(&conv_op);

  //  delete pointers (including data pointers)
  free(input_cpu_data);
  free(filter_cpu_data);
  free(bias_cpu_data);
  free(output_cpu_data);
  free(cpu_result);

  return 0;
}
