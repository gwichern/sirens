import os, sys

source_prefix = 'source/'

# Prefix option for alternate install path.
AddOption('--prefix', default = '/usr/local', dest = 'prefix', type = 'string', nargs = 1, action = 'store', metavar = 'DIR', help = 'installation prefix')
AddOption('--shared', action = 'store_true', dest = 'shared', help = 'use to build a shared library')
AddOption('--debug-symbols', action = 'store_true', dest = 'debug_symbols', help = 'build in debug symbols')

# STK macros.
stk_flags = ""
arch = os.uname()[0]

if arch == 'Darwin':
	stk_flags = '-D__MACOSX_CORE__'
else:
	if arch == 'Linux':
		if os.path.isfile('/proc/asound'):
			stk_flags = '-D__LINUX_ALSA__'
		else:
			stk_flags = '-D__LINUX_OSS__'
	
	
		if sys.byteorder == 'little':
			stk_flags = stk_flags + " -D__LITTLE_ENDIAN__"

other_flags = ''
if GetOption('debug_symbols'):
	other_flags = '-g'

# Header files to install.
install_feature_headers = [
	'features/SpectralCentroid.h', 
	'features/SpectralSparsity.h',
	'features/TemporalSparsity.h',
	'features/TransientIndex.h',
	'features/Harmonicity.h', 
	'features/Loudness.h',
]

install_stk_headers = [
	'stk/FileRead.h', 
	'stk/FileWvIn.h', 
	'stk/Thread.h', 
	'stk/WvIn.h',
	'stk/Stk.h'
]

install_headers = [
	'SegmentationParameters.h',
	'CircularArray.h', 
	'FeatureSet.h',
	'Segmenter.h',
	'Feature.h',
	'Sound.h',
	'Sirens.h'
]

for i in range(0, len(install_feature_headers)):
	install_feature_headers[i] = source_prefix + install_feature_headers[i]

for i in range(0, len(install_stk_headers)):
	install_stk_headers[i] = source_prefix + install_stk_headers[i]

for i in range(0, len(install_headers)):
	install_headers[i] = source_prefix + install_headers[i]

# Source to compile.
compile_source = Glob(os.path.join(source_prefix, '*.cpp'))
compile_source.extend(Glob(os.path.join(source_prefix, 'support/*.cpp')))
compile_source.extend(Glob(os.path.join(source_prefix, 'features/*.cpp')))
compile_source.extend(Glob(os.path.join(source_prefix, 'stk/*.cpp')))

# Create the environment, add necessary flags.
environment = Environment(CC = 'gcc')

try:
	environment.Append(CPPFLAGS = os.environ['CPPFLAGS'] + ' '+ stk_flags + ' ' + other_flags)
	print "Using CPPFLAGS: " + os.environ['CPPFLAGS']
except KeyError:
	environment.Append(CPPFLAGS = stk_flags + ' ' + other_flags)

environment.Append(PREFIX = GetOption('prefix'))

library = environment.Library('sirens', compile_source) 

all_features_example = environment.Program('examples/all_features', 'examples/all_features.cpp', LIBS = ['sirens', 'fftw3', 'sndfile'], LIBPATH = '.')
segmentation_example = environment.Program('examples/segmentation', 'examples/segmentation.cpp', LIBS = ['sirens', 'fftw3', 'sndfile'], LIBPATH = '.')

environment.Install('$PREFIX/lib', library)

environment.Install('$PREFIX/include/sirens', install_headers)
environment.Install('$PREFIX/include/sirens/stk', install_stk_headers)
environment.Install('$PREFIX/include/sirens/features', install_feature_headers)
environment.Alias('install', ['$PREFIX/lib', '$PREFIX/include/sirens', '$PREFIX/include/sirens/stk', '$PREFIX/include/sirens/features'])
