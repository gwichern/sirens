import os

source_prefix = 'source/'

install_headers = ['CircularArray.h', 'Feature.h', 'HarmonicityFeature.h', 
	'LoudnessFeature.h', 'Sound.h', 'SpectralCentroidFeature.h', 
	'SpectralSparsityFeature.h', 'TemporalSparsityFeature.h',
	'TransientIndexFeature.h']
	
install_stk_headers = ['stk/FileRead.h', 'stk/FileWvIn.h', 
	'stk/Stk.h', 'stk/Thread.h', 'stk/WvIn.h']

for i in range(0, len(install_headers)):
	install_headers[i] = source_prefix + install_headers[i]

for i in range(0, len(install_stk_headers)):
	install_stk_headers[i] = source_prefix + install_stk_headers[i]
	
compile_source = Glob(os.path.join(source_prefix, '*.cpp'))
compile_stk_source = Glob(os.path.join(source_prefix, 'stk/*.cpp'))
compile_source.extend(compile_stk_source)

AddOption('--prefix', default = '/usr/local', dest = 'prefix', type = 'string', nargs = 1, action = 'store', metavar = 'DIR', help = 'installation prefix')
AddOption('--include-directory', dest = 'include', type = 'string', nargs = 1, action = 'store', metavar = 'DIR', help = 'additional include directories')

environment = Environment(CC = 'gcc', CPPDEFINES = '__OS_MACOSX__')
environment.Append(CPPPATH = GetOption('include'))
environment.Append(PREFIX = GetOption('prefix'))

library = environment.Library('sirens', compile_source)
environment.Install('$PREFIX/lib', library)
environment.Install('$PREFIX/include/sirens', install_headers)
environment.Install('$PREFIX/include/sirens/stk', install_stk_headers)
environment.Alias('install', ['$PREFIX/lib', '$PREFIX/include/sirens', '$PREFIX/include/sirens/stk'])
