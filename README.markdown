# Sirens
Sirens is a library for segmentation, indexing, and retrieval of environmental and natural sounds. Sirens is currently under development and at the moment is only ready to use for feature extraction, segmentation, and comparison as per [1] and [2] below. Check back later for updates.

Sirens is being developed simultaneously with [sirens-ruby](http://github.com/plant/sirens-ruby), a Ruby gem that implements the features within Sirens.

# Requirements
Any application using Sirens also needs to link against [FFTW](http://www.fftw.org), [libsndfile](http://www.mega-nerd.com/libsndfile/), and pthread.  It is also necessary to have the [uBLAS](http://www.boost.org/doc/libs/1_40_0/libs/numeric/ublas/doc/index.htm) headers installed. In the future, a sirens-config binary will assist with this process, but for now, something like the following should work:

	g++ myproject.cpp -lsirens -lfftw3 -lsndfile -lpthread

# Installation
To install Sirens, make sure you have [Python](http://www.python.org) and the [SCons](http://www.scons.org) build system installed. (Python is only used for building Sirens.) Additionally, make sure you have the [FFTW](http://www.fftw.org), [libsndfile](http://www.mega-nerd.com/libsndfile/), and [uBLAS](http://www.boost.org/doc/libs/1_40_0/libs/numeric/ublas/doc/index.htm) headers installed. Then, perform the following command in the project's directory:

	sudo scons install

To build the applications in the examples/ directory, do
	
	scons
	
To get rid of intermediate build files, type:
	
	scons -c

## Parameters
If you do not have root access, you may have troubles installing Sirens. In this case, you can put the library in a custom directory. For example, to put the library in ~/lib and the includes in ~/include/sirens:

	scons install --prefix=$HOME
	
Additionally, if you need to point the Sirens install to a specific directory to find the FFTW or libsndfile headers, you can specify any additional parameters to the compiler using $CFLAGS and $LDFLAGS. For example, to use ~/include/fftw3.h, ~/lib/fftw3.a, and so on:
	
	export CFLAGS=-I$HOME/include
	export LDFLAGS=-L$HOME/lib
	scons install --prefix=$HOME

# Tutorial
Coming soon. See the examples/ directory for some ideas.

# Upcoming changes / Helping out
There are a number of tasks we are planning for improving Sirens. If you are interested in working on or making any fixes to Sirens, thanks! Just fork this project. If you're looking for some inspiration, check out TODO.markdown.
 
# Acknowledgements
Sirens is based off research within the [Arts, Media and Engineering](http://ame.asu.edu/) program at [Arizona State University](http://asu.edu/). For more information, the following papers may be of interest:

1. Gordon Wichern, H. Thornburg, B. Mechtley, A. Fink, A Spanias, and K. Tu, “Robust multi-feature segmentation and indexing for natural sound environments,” in _Proc. of IEEE/EURASIP International Workshop on Content Based Multimedia Indexing (CBMI)_, Bordeaux France, July 2007. [(PDF)](http://www.public.asu.edu/~gwichern/CBMI07.pdf)
2. Gordon Wichern, J. Xue, H. Thornburg, and A. Spanias, “Distortion-aware query-by-example of environmental sounds,” in _Proc. of IEEE Workshop on Applications of Signal Processing to Audio and Acoustics (WASPAA)_, New Paltz, NY, October 2007. [(PDF)](http://www.public.asu.edu/~gwichern/WASPAA07.pdf)
3. J. Xue, Gordon Wichern, H. Thornburg, and A. Spanias, “Fast query-by-example of environmental sounds via robust and efficient cluster-based indexing,” in _Proc. of IEEE International Conference on Acoustics Speech and Signal Processing (ICASSP)_, Las Vegas, NV, April 2008. [(PDF)](http://www.public.asu.edu/~gwichern/cluster_ICASSP08.pdf)

Additionally, work on Sirens is supported by the [National Science Foundation](http://www.nsf.gov/) under Grants NSF IGERT DGE-05-04647 and NSF CISE Research Infrastructure 04-03428.

# Copyright
Copyright 2007-2009 Brandon Mechtley, Gordon Wichern, Alex Fink, and Harvey Thornburg. Sirens is licensed under the [GNU LGPL](http://creativecommons.org/licenses/LGPL/2.1/). See COPYING.LESSER for details. If you would like to use Sirens for a commercial application or have other difficulties with its licensing restrictions, please contact [Brandon Mechtley on Github](http://www.github.com/plant/).
