import os

source_prefix = 'source/'
stk_flags = ""

if os.uname()[0] == 'Darwin':
	stk_flags = '-D__MACOSX_CORE__'

install_feature_headers = [
	'features/SpectralCentroid.h', 
	'features/SpectralSparsity.h',
	'features/TemporalSparsity.h',
	'features/TransientIndex.h',
	'features/Harmonicity.h', 
	'features/Loudness.h',
	'features/Feature.h'
]

install_stk_headers = [
	'stk/FileRead.h', 
	'stk/FileWvIn.h', 
	'stk/Thread.h', 
	'stk/WvIn.h',
	'stk/Stk.h'
]

install_headers = [
	'CircularArray.h', 
	'Sound.h'
]

for i in range(0, len(install_feature_headers)):
	install_feature_headers[i] = source_prefix + install_feature_headers[i]

for i in range(0, len(install_stk_headers)):
	install_stk_headers[i] = source_prefix + install_stk_headers[i]

for i in range(0, len(install_headers)):
	install_headers[i] = source_prefix + install_headers[i]

compile_source = Glob(os.path.join(source_prefix, '*.cpp'))
compile_source.extend(Glob(os.path.join(source_prefix, 'support/*.cpp')))
compile_source.extend(Glob(os.path.join(source_prefix, 'features/*.cpp')))
compile_source.extend(Glob(os.path.join(source_prefix, 'stk/*.cpp')))

AddOption('--prefix', default = '/usr/local', dest = 'prefix', type = 'string', nargs = 1, action = 'store', metavar = 'DIR', help = 'installation prefix')

environment = Environment(CC = 'gcc')

try:
	environment.Append(CPPFLAGS = os.environ['CPPFLAGS'] + stk_flags)
except KeyError:
	environment.Append(CPPFLAGS = stk_flags)

environment.Append(PREFIX = GetOption('prefix'))

library = environment.Library('sirens', compile_source)
environment.Install('$PREFIX/lib', library)
environment.Install('$PREFIX/include/sirens', install_headers)
environment.Install('$PREFIX/include/sirens/stk', install_stk_headers)
environment.Install('$PREFIX/include/sirens/features', install_feature_headers)
environment.Alias('install', ['$PREFIX/lib', '$PREFIX/include/sirens', '$PREFIX/include/sirens/stk', '$PREFIX/include/sirens/features'])
