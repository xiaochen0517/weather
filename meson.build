project('weather',
        'c',
        version : '1.0.0',
        default_options : ['warning_level=3', 'c_std=c11'])

curl_dep = dependency('libcurl', required : true, allow_fallback : true)
jansson_dep = dependency('jansson', required : true)
openssl_dep = dependency('openssl', required : true)

sources = [
        'src/main.c',
        'src/api.c',
        'src/utils.c',
        'src/jwt.c',
        'src/parser.c',
        'src/output.c',
]

weather = executable(
        'weather',
        sources,
        dependencies : [curl_dep, jansson_dep, openssl_dep],
        include_directories : include_directories('src'),
        install : true
)

test('test', weather)