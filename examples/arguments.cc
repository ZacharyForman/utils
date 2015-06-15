int main(int argc, char **argv)
{
  // path to images.
  std::string images_path;
  // base dictionary path.
  std::string dict_base_path;
  // Controls number of patches in dictionary.
  int dict_size;
  // Controls sparsity of coding.
  float lambda;
  // Size of an image patch.
  int patch_size;
  // Number of patches to sample.
  int samples;
  // Upscaling factor.
  int upscale_factor;
  // If we train this run.
  bool train;
  // If we test this run.
  bool test;
  // Iterations to run the dict through
  int iterations;
  // Size of each training batch
  int batch_size;
  // Extension of images to use
  std::string extension;
  // Variance threshold
  float threshold;
  // Overlap in resolved images;
  int overlap;

  ArgList args;

  args.add_arg(&images_path, "--image-path", std::string("data/train/"),
               "path to the folder the images are in",
               "path must be a string", false);
  args.add_arg(&dict_base_path, "--dict-path", std::string("./"),
               "path to the folder the dictionaries are in",
               "path must be a string", false);
  args.add_arg(&dict_size, "--dict-size", 512,
               "the size of the dictionary used",
               "must be an integer", false);
  args.add_arg(&lambda, "--lambda", 0.15f,
               "controls the sparsity of coding",
               "must be a float", false);
  args.add_arg(&patch_size, "--patch-size", 5,
               "size of an image patch",
               "must be an integer", false);
  args.add_arg(&samples, "--samples", 100000,
               "number of samples to train on",
               "must be an integer", false);
  args.add_arg(&upscale_factor, "--upscale-factor", 2,
               "upscaling factor",
               "must be an integer", false);
  args.add_arg(&train, "--train", false,
               "train a coupled dictionary",
               "", false);
  args.add_arg(&test, "--test", false,
               "test the dictionary on the images given",
               "", false);
  args.add_arg(&iterations, "--iterations", 40,
               "number of batches to run the dictionary through",
               "must be an integer", false);
  args.add_arg(&batch_size, "--batch_size", -1,
               "size of each training batch. Number of images by default",
               "must be an integer", false);
  args.add_arg(&extension, "--ext", std::string("bmp"),
               "extension of images to use",
               "must be a string", false);
  args.add_arg(&threshold, "--threshold", 0.05f/256.0f,
               "threshold of variance to keep patches",
               "must be a float", false);
  args.add_arg(&overlap, "--overlap", 4,
               "overlap of adjacent image patches",
               "must be an integer", false);

  args.parse_args(argc, argv);
}
