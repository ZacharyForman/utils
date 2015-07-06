#include "../arguments.h"

#include <string>

int main(int argc, const char **argv)
{
  // path to images.
  std::string images_path = "images/";
  // base dictionary path.
  std::string dict_base_path = "dict/";
  // Controls number of patches in dictionary.
  int dict_size = 1024;
  // Controls sparsity of coding.
  float lambda = 0.15;
  // Size of an image patch.
  int patch_size = 3;
  // Number of patches to sample.
  int samples = 100000;
  // Upscaling factor.
  int upscale_factor = 3;
  // If we train this run.
  bool train = false;
  // If we test this run.
  bool test= false;
  // Iterations to run the dict through
  int iterations = 1000;
  // Size of each training batch
  int batch_size = 50;
  // Extension of images to use
  std::string extension = ".jpg";
  // Variance threshold
  float threshold = 0.1;
  // Overlap in resolved images;
  int overlap = 2;

  utils::ArgList args;

  args.add_arg(images_path, "--image-path",
               "Path to the folder the images are in",
               utils::ArgList::required);
  args.add_arg(dict_base_path, "--dict-path",
               "Path to the folder the dictionaries are in");
  args.add_arg(dict_size, "--dict-size",
               "The size of the dictionary used");
  args.add_arg(lambda, "--lambda",
               "Controls the sparsity of coding");
  args.add_arg(patch_size, "--patch-size",
               "Size of an image patch");
  args.add_arg(samples, "--samples",
               "Number of samples to train on");
  args.add_arg(upscale_factor, "--upscale-factor",
               "Upscaling factor");
  args.add_arg(train, "--train",
               "Train a coupled dictionary");
  args.add_arg(test, "--test",
               "Test the dictionary on the images given");
  args.add_arg(iterations, "--iterations",
               "Number of batches to run the dictionary through");
  args.add_arg(batch_size, "--batch_size",
               "Size of each training batch.");
  args.add_arg(extension, "--ext",
               "Extension of images to use");
  args.add_arg(threshold, "--threshold",
               "Threshold of variance to keep patches");
  args.add_arg(overlap, "--overlap",
               "Overlap of adjacent image patches");

  args.parse_args(argc, argv);
}
