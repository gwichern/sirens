# Recommended Sirens Tasks
There are a number of tasks that can help improve Sirens and make it more useful to a broader audience. This document lists a few of these tasks.

## Modularity
Features need to be hard-coded within Sirens right now. A more appropriate model would be to use a common plugin format such as [LADSPA](http://www.ladspa.org/) or [Vamp](http://www.vamp-plugins.org/) and allow users of Sirens to select their own features. Feature currently in Sirens can be written as plugins of these formats. One potential downside to this (I believe) is that it is more difficult to reuse a global FFT in these cases, but keeping the sample vs. spectral features distinction is still a possibility, though a bit confusing for Vamp plugins that already compute their own spectra.

Additionally, the segmentation algorithm could be implemented as a plugin of one of these formats, with a simple wrapper function to interpret the modes the plugin would output. 

## Exceptions
None of this code throws exceptions. More error-checking needs to be done.

## Tests
There are currently no unit tests. 

## Feature extraction
1. **Algorithm documentation** - More complex features, such as Spectral Centroid, Transient Index, and Harmonicity need better code documentation. 

## Segmentation
1. **Algorithm documentation** - The algorithms used for segmentation could be more appropriately documented.
2. **Parameter documentation** - The parameters are not sufficiently documented as of yet.
3. **Viterbi beam search** - Currently, the most optimal mode sequence for segmentation is found by applying the full Viterbi algorithm to the network, which has complexity O(3<sup>F + 2</sup>N), where F is the number of featues and N is the length of the sound in frames. The beam search approximation is much more efficient (O(F<sup>2</sup>N)) and probably sufficient for segmentation.
4. **Parameter learning** - Techniques for parameter learning can help reduce the complexity of using Sirens segmentation. Currently, each feature has 12 parameters that need to be set. New environments or recording devices require retuning these parameters. Gordon Wichern is working on techniques for learning these automatically. Another, less sophisticated, potential solution includes creating an application to estimate these from examples (e.g. measuring features' covariances when active/inactive).

## Retrieval
1. **Algorithm documentation** - The algorithms used for retrieval could be more appropriately documented.
2. **Matrix optimization** - Currently, the retrieval code is the only part of Sirens that resorts to using a matrix library (uBLAS). Many matrices are passed by value, and some of the computations could be made more efficient either by abandoning uBLAS or using more advanced features in uBLAS such as ranges and slices and different types of matrices. 
