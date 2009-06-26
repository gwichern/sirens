# Sirens
Sirens is a library for segmentation, indexing, and retrieval of environmental and natural sounds. Sirens is currently under development and at the moment only supports very basic feature extraction. Check back later for updates.

Sirens is being developed simultaneously with [sirens-ruby](http://github.com/plant/sirens-ruby), a Ruby gem that implements the features within Sirens.

# Requirements
Any application using Sirens also needs to link against [STK](http://ccrma.stanford.edu/software/stk/) and [FFTW](http://www.fftw.org).

# Installation
To install Sirens, make sure you have [Python](http://www.python.org) and the [SCons](http://www.scons.org) build system installed. (Python is only used for building Sirens.) Then, perform the following commands in the project's directory:

	scons
	scons install
	
To get rid of intermediate build files, type:
	
	scons -c

# Example
Coming soon.

# Acknowledgements
Sirens is based off research within the Arts, Media and Engineering program at Arizona State University. For more information, the following papers may be of interest:

1. Gordon Wichern, H. Thornburg, B. Mechtley, A. Fink, A Spanias, and K. Tu, “Robust multi-feature segmentation and indexing for natural sound environments,” in _Proc. of IEEE/EURASIP International Workshop on Content Based Multimedia Indexing (CBMI)_, Bordeaux France, July 2007.
2. Gordon Wichern, J. Xue, H. Thornburg, and A. Spanias, “Distortion-aware query-by-example of environmental sounds,” in _Proc. of IEEE Workshop on Applications of Signal Processing to Audio and Acoustics (WASPAA)_, New Paltz, NY, October 2007.
3. J. Xue, Gordon Wichern, H. Thornburg, and A. Spanias, “Fast query-by-example of environmental sounds via robust and efficient cluster-based indexing,” in _Proc. of IEEE International Conference on Acoustics Speech and Signal Processing (ICASSP)_, Las Vegas, NV, April 2008.
