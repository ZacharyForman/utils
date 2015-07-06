#include "../arguments.h"

#include <string>

int main(int argc, const char **argv)
{
  utils::ArgList args;

  bool ARG(train, false, args, "--train",
           "Train a coupled dictionary");
  bool ARG(test, false, args, "--test",
           "Test the dictionary on the images given");

  std::string ARG(images_path, "images/", args, "--image-path",
                  "Path to the folder the images are in.",
                  utils::ArgList::required);
  std::string ARG(dict_base_path, "dict/", args, "--dict-path",
                  "Path to the folder the dictionaries are in.");
  std::string ARG(extension, ".jpg", args, "--ext",
                  "Extension of images to use");

  int ARG(dict_size, 1024, args, "--dict-size",
          "The number of patches in the dictionary.");
  int ARG(patch_size, 3, args, "--patch-size",
          "Size of an image patch.");
  int ARG(samples, 100000, args, "--samples",
          "Number of samples to train on");
  int ARG(upscale_factor, 3, args, "--upscale-factor",
          "Upscaling factor");
  int ARG(iterations, 1000, args, "--iterations",
          "Number of batches to run the dictionary through");
  int ARG(batch_size, 50, args, "--batch_size",
          "Size of each training batch.");
  int ARG(overlap, 2, args, "--overlap",
          "Overlap of adjacent image patches");

  float ARG(lambda, 0.15, args, "--lambda",
            "Controls the sparsity of coding.");
  float ARG(threshold, 0.1, args, "--threshold",
            "Threshold of variance to keep patches");

  args.parse_args(argc, argv);
}
