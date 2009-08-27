import os, sys

source_prefix = 'source/'

# Append the source prefix to header files.
def append_source_prefix(header_list):
	return map(lambda x: os.path.join(source_prefix, x), header_list)

# Options.
AddOption('--prefix', default = '/usr/local', dest = 'prefix', type = 'string', nargs = 1, action = 'store', metavar = 'DIR', help = 'installation prefix')
AddOption('--shared', action = 'store_true', dest = 'shared', help = 'use to build a shared library')
AddOption('--debug-symbols', action = 'store_true', dest = 'debug_symbols', help = 'build in debug symbols')

other_flags = ''
if GetOption('debug_symbols'):
	other_flags = '-g'

# Headers.
install_feature_headers = append_source_prefix([
	'features/SpectralCentroid.h', 
	'features/SpectralSparsity.h',
	'features/TemporalSparsity.h',
	'features/TransientIndex.h',
	'features/Harmonicity.h', 
	'features/Loudness.h',
])

install_headers = append_source_prefix([
	'SegmentationParameters.h',
	'RetrievalModel.h',
	'CircularArray.h', 
	'FeatureSet.h',
	'Segmenter.h',
	'Feature.h',
	'Sound.h',
	'Thread.h',
	'Sirens.h'
])

# Source.
compile_source = Glob(os.path.join(source_prefix, '*.cpp'))
compile_source.extend(Glob(os.path.join(source_prefix, 'support/*.cpp')))
compile_source.extend(Glob(os.path.join(source_prefix, 'features/*.cpp')))

# Environment.
environment = Environment(CC = 'gcc')

try:
	environment.Append(CPPFLAGS = os.environ['CPPFLAGS'] + ' ' + other_flags)
	print "Using CPPFLAGS: " + os.environ['CPPFLAGS']
except KeyError:
	environment.Append(CPPFLAGS = other_flags)

environment.Append(PREFIX = GetOption('prefix'))

library = environment.Library('sirens', compile_source) 

# Examples.
segmentation_example = environment.Program('examples/segmentation', 'examples/segmentation.cpp', LIBS = ['sirens', 'fftw3', 'sndfile'], LIBPATH = '.')
retrieval_example = environment.Program('examples/retrieval', 'examples/retrieval.cpp', LIBS = ['sirens', 'fftw3', 'sndfile'], LIBPATH = '.')
features_example = environment.Program('examples/features', 'examples/features.cpp', LIBS = ['sirens', 'fftw3', 'sndfile'], LIBPATH = '.')
# Install library.
environment.Install('$PREFIX/lib', library)
environment.Install('$PREFIX/include/sirens', install_headers)
environment.Install('$PREFIX/include/sirens/features', install_feature_headers)
environment.Alias('install', ['$PREFIX/lib', '$PREFIX/include/sirens', '$PREFIX/include/sirens/features'])
