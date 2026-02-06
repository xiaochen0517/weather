project('weather', 'c',
        version : '1.0.0',
        default_options : ['warning_level=3', 'c_std=c11'])

curl = dependency('libcurl', required : true, allow_fallback : true)
jansson = dependency('jansson', required : true)
openssl = dependency('openssl', required : true)

sources = [
        'src/main.c',
        'src/api.c',
        'src/utils.c',
        'src/jwt.c',
        'src/parser.c',
]

weather = executable(
        'weather',
        sources,
        dependencies : [curl, jansson, openssl],
        include_directories : include_directories('src'),
        install : true
)

test('test', weather)