project('weather', 'c',
        version : '1.0.0',
        default_options : ['warning_level=3', 'c_std=c11'])

cjson = dependency('libcjson', fallback : ['cjson', 'cjson_dep'], required : true)

sources = ['src/main.c']

weather = executable('weather', sources, dependencies : [cjson], install : true)

test('test', weather)