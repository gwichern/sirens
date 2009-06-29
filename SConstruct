import os

source_prefix = 'source/'

install_headers = ['CircularArray.h', 'Feature.h', 'HarmonicityFeature.h', 
	'LoudnessFeature.h', 'Sound.h', 'SpectralCentroidFeature.h', 
	'SpectralSparsityFeature.h', 'TemporalSparsityFeature.h',
	'TransientIndexFeature.h']

for i in range(0, len(install_headers)):
	install_headers[i] = source_prefix + install_headers[i]

compile_source = Glob(os.path.join(source_prefix, '*.cpp'))
compile_stk_source = Glob(os.path.join(source_prefix, 'stk/*.cpp'))
compile_source.extend(compile_stk_source)

environment = Environment(CC = 'gcc', CPPDEFINES = '__OS_MACOSX__', CPPATH = os.environ['CPPATH'])
library = environment.Library('sirens', compile_source)
environment.Install('/usr/local/lib', library)
environment.Install('/usr/local/include/sirens', install_headers)
environment.Alias('install', ['/usr/local/lib', '/usr/local/include/sirens'])
