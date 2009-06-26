import os.path

source_prefix = 'source/'

headers = ['CircularArray.h', 'Feature.h', 'HarmonicityFeature.h', 
	'LoudnessFeature.h', 'Sound.h', 'SpectralCentroidFeature.h', 
	'SpectralSparsityFeature.h', 'TemporalSparsityFeature.h',
	'TransientIndexFeature.h']

headers = [os.path.join(source_prefix, header) for header in headers]

environment = Environment(CC = 'gcc', CPPDEFINES = '__OS_MACOSX__')
library = environment.Library('sirens', Glob(os.path.join(source_prefix, '*.cpp')))
environment.Install('/usr/local/lib', library)
environment.Install('/usr/local/include/sirens', headers)
environment.Alias('install', ['/usr/local/lib', '/usr/local/include/sirens'])
