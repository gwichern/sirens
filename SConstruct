import os, sys, SCons.Util

source_prefix = 'source/'

# Append the source prefix to header files.
def append_source_prefix(header_list):
	return map(lambda x: os.path.join(source_prefix, x), header_list)

# Options.
AddOption('--prefix', default = '/usr/local', dest = 'prefix', type = 'string', nargs = 1, action = 'store', metavar = 'DIR', help = 'installation prefix')
AddOption('--shared', action = 'store_true', dest = 'shared', help = 'use to build a shared library')
AddOption('--debug-symbols', action = 'store_true', dest = 'debug_symbols', help = 'build in debug symbols')

# Headers.
install_segmentation_headers = append_source_prefix([
	'segmentation/SegmentationParameters.h',
	'segmentation/Segmenter.h'
])

install_retrieval_headers = append_source_prefix([
	'retrieval/FeatureComparator.h',
	'retrieval/SoundComparator.h'
])

install_feature_headers = append_source_prefix([
	'features/SpectralCentroid.h', 
	'features/SpectralSparsity.h',
	'features/TemporalSparsity.h',
	'features/TransientIndex.h',
	'features/Harmonicity.h', 
	'features/Loudness.h',
])

install_headers = append_source_prefix([
	'CircularArray.h', 
	'FeatureSet.h',
	'Feature.h',
	'Sound.h',
	'Thread.h',
	'Sirens.h'
])

# Source.
compile_source = Glob(os.path.join(source_prefix, '*.cpp'))
compile_source.extend(Glob(os.path.join(source_prefix, 'support/*.cpp')))
compile_source.extend(Glob(os.path.join(source_prefix, 'features/*.cpp')))
compile_source.extend(Glob(os.path.join(source_prefix, 'retrieval/*.cpp')))
compile_source.extend(Glob(os.path.join(source_prefix, 'segmentation/*.cpp')))

# Environment.
environment = Environment(CC = 'gcc')

if GetOption('debug_symbols'):
	environment['CCFLAGS'] += SCons.Util.CLVar('-g')

if os.environ.has_key('CC'):
	environment['CC'] = os.environ['CC']
if os.environ.has_key('CFLAGS'):
	environment['CCFLAGS'] += SCons.Util.CLVar(os.environ['CFLAGS'])
if os.environ.has_key('CXX'):
	environment['CXX'] = os.environ['CXX']
if os.environ.has_key('CXXFLAGS'):
	environment['CXXFLAGS'] += SCons.Util.CLVar(os.environ['CXXFLAGS'])
if os.environ.has_key('LDFLAGS'):
	environment['LINKFLAGS'] += SCons.Util.CLVar(os.environ['LDFLAGS'])

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
environment.Install('$PREFIX/include/sirens/retrieval', install_retrieval_headers)
environment.Install('$PREFIX/include/sirens/segmentation', install_segmentation_headers)

environment.Alias('install', ['$PREFIX/lib', '$PREFIX/include/sirens', '$PREFIX/include/sirens/features', '$PREFIX/include/sirens/retrieval', '$PREFIX/include/sirens/segmentation'])
